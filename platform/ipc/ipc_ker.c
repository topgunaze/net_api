/******************************************************************************
Copyright (C), 2014-2024, C-Data Tech. Co., Ltd.
  �� �� ��   : ipc_ker.c
  ��    ��   : jh.liang
  �� �� ��   : 1.0.0 
  ��������   : 2015��3��13��
  ��������   : IPC���Ľ���������
******************************************************************************/


#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <tf_syslog.h>
#include <stdarg.h>
#include "pthread.h"
#include "stdio.h"
#include "malloc.h"
#include "ipc_if.h"
#include "ipc_private.h"
#include "ipc_ker.h"

IPC_KER_CTL         gIpcKerCtl;
IPC_KER_MO_CTL      gIpcKerMoCtl[SYS_MAX_MODULE_NUM];
IPC_KER_EVENT_LIST  *gIpcEventCtlHead[IPC_EVENT_MAX];

ULONG ipc_ker_insert_event_node(IPC_KER_EVENT_LIST **pHead,IPC_KER_EVENT_LIST *pNew)
{
    IPC_KER_EVENT_LIST  *pCur;
    if(*pHead==NULL)
    {
        *pHead=pNew;
        return IPC_SUCCESS;
    }
    pCur=*pHead;
    
    for(;;)
    {
        if(pCur->ucMo==pNew->ucMo)
        {
            return IPC_OBJ_EXIST;
        }
        if(pCur->pNext==NULL)
            break;
        pCur=pCur->pNext;
    }
    pNew->pPre=pCur;
    pCur->pNext =pNew;
    return IPC_SUCCESS;
}
void ipc_ker_del_event_node(IPC_KER_EVENT_LIST **pHead,IPC_KER_EVENT_LIST *pDel)
{
    IPC_KER_EVENT_LIST  *pPre,*pNext;
    
    if(*pHead==NULL)
    {
        return;
    }
    if((pDel->pPre==NULL)&&(pDel->pNext==NULL))
    {
        *pHead=NULL;
        return;
    }
    if(pDel->pPre==NULL)
    {
        pNext=pDel->pNext;
        pNext->pPre=NULL;
        *pHead=pNext;
        return;
    }
    if(pDel->pNext==NULL)
    {
        pPre=pDel->pPre;
        pPre->pNext=NULL;
        return;
    }
    pPre=pDel->pPre;
    pNext=pDel->pNext;
    
    pPre->pNext=pDel->pNext;
    pNext->pPre=pPre;    
}

IPC_KER_EVENT_LIST *ipc_ker_lookup_event_node(IPC_KER_EVENT_LIST *pHead,UCHAR ucMo)
{
    IPC_KER_EVENT_LIST  *pCur=pHead;
    
    while(pCur!=NULL)
    {
        if(pCur->ucMo==ucMo)
            return pCur;
        pCur=pCur->pNext;
    }
    return NULL;
}

void ipc_ker_init(void)
{
    int i;
    int rcvbuf = 64*1024;
    
    memset(&gIpcKerCtl,0,sizeof(IPC_KER_CTL));
    memset(gIpcKerMoCtl,0,sizeof(IPC_KER_MO_CTL)*SYS_MAX_MODULE_NUM);
    
    for(i=0;i<IPC_EVENT_MAX;i++)
    {
        gIpcEventCtlHead[i]=NULL;
    }
    
    strcpy(gIpcKerCtl.KerSoPath,IPC_MAIN_REC_SO_PATH);

    gIpcKerCtl.KerSoAddr.sun_family=AF_UNIX;
    sprintf(gIpcKerCtl.KerSoAddr.sun_path,"%s",gIpcKerCtl.KerSoPath);
    if ((gIpcKerCtl.KerSockFd= socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        ipc_debug_printf("IPC kernel  Create socket Error.\r\n");
        return;
    }
    if (setsockopt(gIpcKerCtl.KerSockFd,SOL_SOCKET,SO_RCVBUF,&rcvbuf,sizeof(rcvbuf)) < 0) {
        ipc_debug_printf("set SO_RCVBUF Error.\r\n");
    }

    unlink(gIpcKerCtl.KerSoPath);
    bind(gIpcKerCtl.KerSockFd,(void*)&gIpcKerCtl.KerSoAddr,sizeof(gIpcKerCtl.KerSoAddr));
}

ULONG ipc_ker_prepare_ackhead(char * pRecMsg, char *pSendMsg,UCHAR ucType,USHORT usRet)
{
    IPC_HEAD  *pIpcRecHead,*pIpcSendHead;

    if((pRecMsg==NULL)||(pSendMsg==NULL))
    {
        return IPC_NULL_PTR;
    }
    
    pIpcRecHead=(IPC_HEAD  *)IPC_APP_MEM_TO_IPC(pRecMsg);

    if(pIpcRecHead->usMagic!=IPC_HEAD_MAGIC)
    {
        ipc_debug_printf("IPC Kernel Warning: invalid IPC head at line %d.\r\n",__LINE__);
        return IPC_INVALID_HEAD;
    }
    
    pIpcSendHead=(IPC_HEAD  *)IPC_APP_MEM_TO_IPC(pSendMsg);
    pIpcSendHead->SynFlag=pIpcRecHead->SynFlag;
    pIpcSendHead->usSn    =pIpcRecHead->usSn;
    pIpcSendHead->ucSrcMo=pIpcRecHead->ucDstMo;
    pIpcSendHead->ucDstMo=pIpcRecHead->ucSrcMo;
    pIpcSendHead->ucMsgType=ucType;
    pIpcSendHead->RCode = usRet;
    pIpcSendHead->usMagic=IPC_HEAD_MAGIC;
    pIpcSendHead->Ver =IPC_HEAD_CUR_VER;

    return IPC_SUCCESS;
}

int  ipc_ker_send_data(UCHAR ucDstMo,char *pSend,USHORT usSendLen,struct sockaddr_un *to)
{
    int SoAddrLen;  
    int ret;
    SND_BLOCK_MSG *pmsg;
       
    if(pSend==NULL)
        return 0;
    if (!gIpcKerMoCtl[ucDstMo].IsReged)
        return -1;
    if (gIpcKerMoCtl[ucDstMo].sndwouldblock)
    {
        goto lblqueuemsg;
    }
    
    SoAddrLen=sizeof(struct sockaddr_un);
    ret = sendto(gIpcKerMoCtl[ucDstMo].sndsockfd,pSend,usSendLen,0,(void*)to,SoAddrLen);
    if (ret < 0)
    {
       if (errno==EAGAIN)
           goto lblqueuemsg;
       else
           ipc_debug_printf("ipcker sendto error.errno:%d, errmsg:%s.\n", errno, strerror(errno));
    }
    return ret;

lblqueuemsg:        
   gIpcKerMoCtl[ucDstMo].sndwouldblock = 1;
   ipc_debug_printf("sendto module:%d name:%s would block, to add to queue.\n", ucDstMo, gIpcKerMoCtl[ucDstMo].NameMo);

   pmsg = malloc(sizeof(SND_BLOCK_MSG)+usSendLen);
   if (!pmsg)
   {
       ipc_debug_printf("allocate SND_BLOCK_MSG memory failuer!\n");
       ret = -1;
   }
   else
   {
       pmsg->next = NULL;
       pmsg->datalen = usSendLen;
       memcpy(&pmsg->dstaddr, to, sizeof(struct sockaddr_un));
       memcpy(pmsg->pData, pSend, usSendLen);
       
       if (gIpcKerMoCtl[ucDstMo].BlockMsgTail)
       {
           gIpcKerMoCtl[ucDstMo].BlockMsgTail->next = pmsg;
           gIpcKerMoCtl[ucDstMo].BlockMsgTail = pmsg;
       }
       else
       {
           gIpcKerMoCtl[ucDstMo].BlockMsgHead = pmsg;
           gIpcKerMoCtl[ucDstMo].BlockMsgTail = pmsg;
       }
       gIpcKerCtl.blockmsgcount++;
       ret = 0;
   }           

   return ret;
}

static int set2nonblock(int fd)
{
    int flags;
    
    if((flags=fcntl(fd, F_GETFL, 0))<0)
    {
        ipc_debug_printf("fcntl f_getfl error\n");
        return -1;
    };
    if(fcntl(fd, F_SETFL, flags | O_NONBLOCK)<0)
    {
        ipc_debug_printf("fcntl f_setfl error\n");
        return -1;
    }
    
    return 0;
};

static void clear_snd_resource(UCHAR  ucMo)
{
    if (gIpcKerMoCtl[ucMo].sndsockfd >= 0)
    {
        close(gIpcKerMoCtl[ucMo].sndsockfd);
        gIpcKerMoCtl[ucMo].sndsockfd = -1;
    }
    if (gIpcKerMoCtl[ucMo].sndwouldblock)
    {
        SND_BLOCK_MSG * cur;
        while (gIpcKerMoCtl[ucMo].BlockMsgHead)
        {
            cur = gIpcKerMoCtl[ucMo].BlockMsgHead;
            gIpcKerMoCtl[ucMo].BlockMsgHead = gIpcKerMoCtl[ucMo].BlockMsgHead->next;
            free(cur);
            gIpcKerCtl.blockmsgcount--;
        }
        gIpcKerMoCtl[ucMo].sndwouldblock = 0;
        gIpcKerMoCtl[ucMo].BlockMsgHead = NULL;
        gIpcKerMoCtl[ucMo].BlockMsgTail = NULL;
    }
}

ULONG ipc_ker_regmod_proc(IPC_REG_MODULE_MSG_INFO *pRegInfo,char *pAck, USHORT  *pAckLen)
{
    UCHAR  ucMo;
    IPC_COMMON_REG_ACK_INFO *pRegAck;
    int sndbuf = 64*1024;

    if((pRegInfo==NULL)||(pAck==NULL)||(pAckLen==NULL))
        return IPC_NULL_PTR;

    ucMo=pRegInfo->ucSrcMo;
    if(gIpcKerMoCtl[ucMo].IsReged==TRUE)
    {
        ipc_debug_printf("IPC Kernel Warning: source module %d has been registered.\r\n",ucMo);
        clear_snd_resource(ucMo);
        gIpcKerMoCtl[ucMo].IsReged==FALSE;
    }
    
    ipc_debug_printf("IPC Ker:to be registered module %d,name:%s.\r\n",ucMo,pRegInfo->NameMo);

    if ((gIpcKerMoCtl[ucMo].sndsockfd= socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        ipc_debug_printf("Create socket Error.\r\n");
        return IPC_SOCK_CREATE_FAIL;
    }
    if (setsockopt(gIpcKerMoCtl[ucMo].sndsockfd,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf)) < 0) 
    {
        ipc_debug_printf("set SO_SNDBUF Error.\n");
    }
    if (set2nonblock(gIpcKerMoCtl[ucMo].sndsockfd) < 0)
    {
        close(gIpcKerMoCtl[ucMo].sndsockfd);
        gIpcKerMoCtl[ucMo].sndsockfd = -1;
        return IPC_SOCK_CREATE_FAIL;
    }
    
    strcpy(gIpcKerMoCtl[ucMo].NameMo,pRegInfo->NameMo);
    
    gIpcKerMoCtl[ucMo].AckSoAddr.sun_family=AF_UNIX;
    sprintf(gIpcKerMoCtl[ucMo].AckSoAddr.sun_path,"%s", pRegInfo->SoAckPath);
    
    gIpcKerMoCtl[ucMo].CmdSoAddr.sun_family=AF_UNIX;
    sprintf(gIpcKerMoCtl[ucMo].CmdSoAddr.sun_path,"%s", pRegInfo->SoCmdPath);
    gIpcKerMoCtl[ucMo].IsReged=TRUE;

    pRegAck=(IPC_COMMON_REG_ACK_INFO*)pAck;
    pRegAck->ucSrcMo=pRegInfo->ucSrcMo;
    pRegAck->ucRetCode=IPC_REG_M_ACK;
    *pAckLen=sizeof(IPC_COMMON_REG_ACK_INFO);

    return IPC_SUCCESS;
}

ULONG ipc_ker_disregmod_proc(IPC_DIS_REG_MODULE_MSG_INFO *pDisRegInfo,char *pAck, USHORT  *pAckLen)
{
    UCHAR  ucMo;
    IPC_COMMON_REG_ACK_INFO *pRegAck;

    if((pDisRegInfo==NULL)||(pAck==NULL)||(pAckLen==NULL))
        return IPC_NULL_PTR;

    ucMo=pDisRegInfo->ucSrcMo;
    if(gIpcKerMoCtl[ucMo].IsReged==FALSE)
    {
        ipc_debug_printf("IPC Kernel Warning: source module %d has been disregistered.\r\n",ucMo);
    }
    else 
    {
         ipc_debug_printf("to be disregister module %d ,name:%s.\r\n",ucMo, gIpcKerMoCtl[ucMo].NameMo);
    }
    
    pRegAck=(IPC_COMMON_REG_ACK_INFO*)pAck;
    pRegAck->ucSrcMo=ucMo;
    pRegAck->ucRetCode=IPC_DISREG_M_ACK;
    *pAckLen=sizeof(IPC_COMMON_REG_ACK_INFO);

    return IPC_SUCCESS;
    
}

ULONG ipc_ker_event_enage_proc(IPC_EVENT_CMD_MSG_INFO *pEngage,char *pAck,USHORT  *pAckLen)
{
    UCHAR  ucMo;
    UCHAR  ucEId;
    ULONG  ulRet;
    IPC_KER_EVENT_LIST *pAdd;
    IPC_COMMON_REG_ACK_INFO *pEvAck;

    if((pEngage==NULL)||(pAck==NULL)||(pAckLen==NULL))
        return IPC_NULL_PTR;    

    ucMo=pEngage->ucSrcMo;
    ucEId=pEngage->ucEventId;
    pEvAck=(IPC_COMMON_REG_ACK_INFO*)pAck;
    if((ucEId >= IPC_EVENT_MAX)||(ucEId <= IPC_EVENT_BASE))
    {
        pEvAck->ucSrcMo=ucMo;
        pEvAck->ucRetCode=IPC_ENGAGE_EVENT_NAK;
        ipc_debug_printf("IPC Kernel Warning: invalid event id:%d to be engaged by module %d.\r\n",ucEId,ucMo);
        return IPC_INVALID_EID;
    }

    pAdd=(IPC_KER_EVENT_LIST*)malloc(sizeof(IPC_KER_EVENT_LIST));
    memset(pAdd,0,sizeof(IPC_KER_EVENT_LIST));
    pAdd->ucMo=ucMo;    
	/*�¼�idʹ�������±�����ʾ*/
    ulRet = ipc_ker_insert_event_node(&(gIpcEventCtlHead[ucEId]),pAdd);

    if(ulRet==IPC_OBJ_EXIST)
    {
        ipc_debug_printf("IPC Kernel Warning: Eid:%d has been already engaged by  module %d.\r\n",ucEId,ucMo);
        free(pAdd);
        pEvAck->ucSrcMo=ucMo;
        pEvAck->ucRetCode=IPC_ENGAGE_EVENT_ACK;
        return IPC_SUCCESS;        
    }        
    if(IPC_SUCCESS!=ulRet)
    {
        pEvAck->ucSrcMo=ucMo;
        pEvAck->ucRetCode=IPC_ENGAGE_EVENT_NAK;
        ipc_debug_printf("IPC Kernel Warning: insert event Nod FAIL. Eid:%d source module %d.\r\n",ucEId,ucMo);
        return IPC_COMMON_FAIL;    
    }
    else
    {
        pEvAck->ucSrcMo=ucMo;
        pEvAck->ucRetCode=IPC_ENGAGE_EVENT_ACK;
        return IPC_SUCCESS;    
    }
}

ULONG ipc_ker_event_disenage_proc(IPC_EVENT_CMD_MSG_INFO *pEngage,char *pAck,USHORT  *pAckLen)
{
    UCHAR  ucMo;
    UCHAR  ucEId;
    IPC_COMMON_REG_ACK_INFO *pEvAck;
    IPC_KER_EVENT_LIST *pDel;

    if((pEngage==NULL)||(pAck==NULL)||(pAckLen==NULL))
        return IPC_NULL_PTR;    

    ucMo=pEngage->ucSrcMo;
    ucEId=pEngage->ucEventId;
    pEvAck=(IPC_COMMON_REG_ACK_INFO*)pAck;
    if((ucEId >= IPC_EVENT_MAX)||(ucEId <= IPC_EVENT_BASE))
    {
        pEvAck->ucSrcMo=ucMo;
        pEvAck->ucRetCode=IPC_DISENGAGE_EVENT_NAK;
        ipc_debug_printf("IPC Kernel Warning: invalid event id:%d to be disengaged by module %d.\r\n",ucEId,ucMo);
        return IPC_INVALID_EID;
    }
    pDel=ipc_ker_lookup_event_node(gIpcEventCtlHead[ucEId],ucMo);
    if(pDel!=NULL)
    {
        ipc_ker_del_event_node(&(gIpcEventCtlHead[ucEId]),pDel);
        free(pDel);
    }
    pEvAck->ucSrcMo=ucMo;
    pEvAck->ucRetCode=IPC_DISENGAGE_EVENT_ACK;

    return IPC_SUCCESS;    
}

ULONG ipc_ker_event_release_proc(IPC_EVENT_R_INFO *pEv)
{
    UCHAR ucEId;
    UCHAR ucMo;
    USHORT usToLen;
    IPC_HEAD *pIpc;
    IPC_KER_EVENT_LIST *pEvDst;
    int ReleaseCnt;

    if(pEv==NULL)
    {
        return IPC_NULL_PTR;
    }
    ucEId=pEv->EventMsgHead.ucEventId;
    if((ucEId >= IPC_EVENT_MAX)||(ucEId <= IPC_EVENT_BASE))
    {
        ipc_debug_printf("IPC Kernel Warning: invalid event id:%d to be released by module %d.\r\n",ucEId,ucMo);
        return IPC_INVALID_EID;
    }
    pEvDst=gIpcEventCtlHead[ucEId];
    if(pEvDst==NULL)
    {
        ipc_debug_printf("IPC Kernel Warning: no moduele engage such event(ID %d).\r\n",ucEId);
        return IPC_EVENT_NOT_ENGAGED;    
    }
    usToLen=sizeof(IPC_HEAD)+sizeof(IPC_EVENT_R_INFO)+(pEv->EventMsgHead.usLen);
    
    pIpc=(IPC_HEAD*)IPC_APP_MEM_TO_IPC(pEv);
    ReleaseCnt=0;
    pIpc->ucSrcMo=MODULE_IPC;
    while(pEvDst!=NULL)
    {
        ucMo=pEvDst->ucMo;
        if(gIpcKerMoCtl[ucMo].IsReged==FALSE)
        {
            pEvDst=pEvDst->pNext;
            continue;
        }
        
        pIpc->ucDstMo=ucMo;
        ipc_ker_send_data(ucMo,(char*)pIpc,usToLen,
            &gIpcKerMoCtl[ucMo].CmdSoAddr);
        pEvDst=pEvDst->pNext;
        ReleaseCnt++;
    }
    ipc_debug_printf("IPC Kernel : release event( Id %d) to total %d dest module . \r\n",ucEId,ReleaseCnt);
    
    return IPC_SUCCESS;
}

ULONG ipc_ker_cmd_msg_proc(char *pMsg,UCHAR type)
{
    IPC_HEAD  *pIpc;
    UCHAR    ucDstMo;
    USHORT usToLen;
    USHORT usSentLen;

    if(pMsg==NULL)
    {
        return IPC_NULL_PTR;
    }
    pIpc=(IPC_HEAD*)IPC_APP_MEM_TO_IPC(pMsg);
    ucDstMo=pIpc->ucDstMo;
    
    if(gIpcKerMoCtl[ucDstMo].IsReged==FALSE)
    {
        return IPC_INVALID_MODULE;
    }
    if(pIpc->usMagic!=IPC_HEAD_MAGIC)
    {
        return IPC_INVALID_HEAD;
    }

    if((type==IPC_MSG_CMD)&&(ucDstMo==pIpc->ucSrcMo))
    {
        ipc_debug_printf("IPC kernel error:cmd msg from module %d to himself.\r\n",pIpc->ucSrcMo,pIpc->ucDstMo);
        return IPC_CMD_MODULE_LOOP;
    }
    
    usToLen=pIpc->usDataLen+sizeof(IPC_HEAD);
    usSentLen = ipc_ker_send_data(ucDstMo,(char*)pIpc,usToLen,
            &gIpcKerMoCtl[ucDstMo].CmdSoAddr);
    if(usToLen!=usSentLen)
    {
        if (usSentLen == 0)
            return IPC_SEND_BLOCK;
        else
            return IPC_SOCKET_SENDTO_FAIL;
    }
    return IPC_SUCCESS;
    
}

ULONG ipc_ker_ack_msg_proc(char *pMsg,UCHAR type)
{
    IPC_HEAD  *pIpc;
    UCHAR    ucDstMo;
    USHORT usToLen;
    USHORT usSentLen;
    
    if(pMsg==NULL)
    {
        return IPC_NULL_PTR;
    }
    pIpc=(IPC_HEAD*)IPC_APP_MEM_TO_IPC(pMsg);
    ucDstMo=pIpc->ucDstMo;
    if(gIpcKerMoCtl[ucDstMo].IsReged==FALSE)
    {
        return IPC_INVALID_MODULE;
    }
    if(pIpc->usMagic!=IPC_HEAD_MAGIC)
    {
        return IPC_INVALID_HEAD;
    }
    ipc_debug_printf("IPC kernel ACK from module %d to module %d,Sn=%d.\r\n",
            pIpc->ucSrcMo,pIpc->ucDstMo,pIpc->usSn);

    if((type==IPC_MSG_ACK)&&(ucDstMo==pIpc->ucSrcMo))
    {
        ipc_debug_printf("IPC kernel error:ACK msg from module %d to himself.\r\n",pIpc->ucSrcMo,pIpc->ucDstMo);
        return IPC_CMD_MODULE_LOOP;
    }
    usToLen=pIpc->usDataLen+sizeof(IPC_HEAD);
    usSentLen = ipc_ker_send_data(ucDstMo,(char*)pIpc,usToLen,
            &gIpcKerMoCtl[ucDstMo].AckSoAddr);
    if(usToLen != usSentLen)
    { 
        if (usSentLen == 0)
            return IPC_SEND_BLOCK;
        else
            return IPC_SOCKET_SENDTO_FAIL;
    }
    return IPC_SUCCESS;

}

static void ipc_ker_resend_blockmsg(void)
{
    SND_BLOCK_MSG *curmsg;
    UCHAR    ucDstMo;
    int ret;
    int blocktime;
    struct timeval timout;

    for (ucDstMo=0; ucDstMo<SYS_MAX_MODULE_NUM; ucDstMo++)
    {
        if (!gIpcKerMoCtl[ucDstMo].sndwouldblock)
            continue;

        blocktime = 0;
        while (gIpcKerMoCtl[ucDstMo].BlockMsgHead)
        {
            curmsg = gIpcKerMoCtl[ucDstMo].BlockMsgHead;          
            ret = sendto(gIpcKerMoCtl[ucDstMo].sndsockfd, curmsg->pData, curmsg->datalen, 0, 
                               (void*)&curmsg->dstaddr, sizeof(struct sockaddr_un));
            if (ret < 0 && errno==EAGAIN)
            {
                blocktime++;
                gIpcKerMoCtl[ucDstMo].retry++;
                if (gIpcKerMoCtl[ucDstMo].retry <= 6)/* max:6s */
                {
                    break;
                }
                gIpcKerMoCtl[ucDstMo].retry = 0;
                gIpcKerMoCtl[ucDstMo].sndwouldblock = 1;
                gIpcKerMoCtl[ucDstMo].BlockMsgHead = gIpcKerMoCtl[ucDstMo].BlockMsgHead->next;
                free(curmsg);
                gIpcKerCtl.blockmsgcount--;
                break;
            }
            blocktime = 0;
            
            if (ret < 0)
                ipc_debug_printf("resend fail.error no:%d\n", errno);
            else if (ret != curmsg->datalen)
                ipc_debug_printf("resend fail.sended length:%d less than data length:%d\n", ret, curmsg->datalen);
            else
                ipc_debug_printf("resend success\n");
            
            gIpcKerMoCtl[ucDstMo].BlockMsgHead = gIpcKerMoCtl[ucDstMo].BlockMsgHead->next;
            free(curmsg);
            gIpcKerCtl.blockmsgcount--;
            gIpcKerMoCtl[ucDstMo].retry = 0;
        }
        /*��Դ�ģ��ΪĿ���������Ϣ�������,����������־����������*/
        if (gIpcKerMoCtl[ucDstMo].BlockMsgHead == NULL)
        {
            gIpcKerMoCtl[ucDstMo].BlockMsgTail = NULL;
            gIpcKerMoCtl[ucDstMo].sndwouldblock = 0;
        }
    }

}
/******************************************************************
*ipcker    ��һ��������ģ�������У�������ʵӦ��*
*ģ��֮���ͨ�ŵ���Ӧ����IPC���ͨ��+Ӧ����*
*IPC��ͨ�ţ���ipc����������Ϣ��ת����                    *
******************************************************************�*/
int main(int argc,char **argv)
{
    fd_set  fds;
    int AddrLen;
    int Ret;
    int RecLen;  
    struct sockaddr_un CliAddr;
    struct timeval timout;
    IPC_HEAD *pIpc;
    char    *pRecData;
    char    *pSendData;
    USHORT  usSendLen;
    USHORT  usActuSendLen;
    ULONG    ulRet;
    int i;
    UCHAR *pData;
    struct timeval last_time;
    struct timeval now_time;
    
    ipc_ker_init();

    if (gettimeofday(&last_time, NULL) < 0)
    {
        ipc_debug_printf("gettimeofday Error!\n");
    }   
    
    for(;;)
    {
        AddrLen=sizeof(CliAddr);
        memset(&CliAddr,0,sizeof(CliAddr));
        
        FD_ZERO(&fds);
        FD_SET(gIpcKerCtl.KerSockFd, &fds);
        if (gIpcKerCtl.blockmsgcount>0)
            timout.tv_sec=1;
        else
            timout.tv_sec=IPC_SYN_SELECT_TIMEOUT_SEC;
        timout.tv_usec=0;

        Ret=select(gIpcKerCtl.KerSockFd + 1, &fds, NULL, NULL, &timout); 
        if(Ret<0)
        {
            ipc_debug_printf("IPC kernel select error.\r\n");
            continue;
        }
        if(Ret==0)
        {
            ipc_debug_printf("IPC kernel select timeout.\r\n");
            if (gIpcKerCtl.blockmsgcount>0)
            {
                ipc_ker_resend_blockmsg();
                gettimeofday(&last_time, NULL);
            }
            continue;
        }
/***********************************************************************
*���´�������Դ˶�ʱ�������¼����������          *
*1���ڴ˶�ʱ�������¼�����,���֮ǰ�б�����       *
*����Ϣ,���Ȱ�֮ǰ����������Ϣ����                              *  
*2�����û�б���������Ϣ����ǰ�������¼�   *
************************************************************************/
        if (gIpcKerCtl.blockmsgcount>0)
        {
            int elapse;
            gettimeofday(&now_time, NULL);
            elapse = (now_time.tv_sec - last_time.tv_sec)*1000 + (now_time.tv_usec - last_time.tv_usec) / 1000;
            if (elapse > 500)
            {
                ipc_ker_resend_blockmsg();
                last_time = now_time;
            }
        }

        RecLen=recvfrom(gIpcKerCtl.KerSockFd,gIpcKerCtl.RecDataBuf,
            IPC_KER_MSG_MAX_LENGTH,0,(void*)&(CliAddr),(socklen_t *)&AddrLen);
        if(RecLen<=sizeof(IPC_HEAD))
        {
            ipc_debug_printf("IPC kernel receive bad packet.lenght %d is too small.\r\n",RecLen); 
            continue;
        }
        
        pIpc = (IPC_HEAD*)gIpcKerCtl.RecDataBuf;
        pRecData = gIpcKerCtl.RecDataBuf+sizeof(IPC_HEAD);
        pSendData = gIpcKerCtl.SndDataBuf+sizeof(IPC_HEAD);
        switch (pIpc->ucMsgType)
        {
            case IPC_MSG_REG_M:/*ģ��ע��*/
                ipc_debug_printf("IPC kernel rec module register msg.\r\n");
                ulRet=ipc_ker_regmod_proc((IPC_REG_MODULE_MSG_INFO*)pRecData,pSendData,&usSendLen);
                if(IPC_SUCCESS!=ulRet)
                {
                    ipc_debug_printf("IPC Kernel process module register FAIL.\r\n");
                    break;
                }
                if(IPC_SUCCESS!=ipc_ker_prepare_ackhead(pRecData,pSendData,IPC_MSG_REG_M_ACK,ulRet))
                {
                    ipc_debug_printf("IPC Kernel prepair IPC head of register ACK FAIL.\r\n");
                    break;
                }
                usActuSendLen=ipc_ker_send_data(((IPC_REG_MODULE_MSG_INFO*)pRecData)->ucSrcMo,gIpcKerCtl.SndDataBuf,
                         usSendLen+sizeof(IPC_HEAD),&CliAddr);
                if(usActuSendLen!=(usSendLen+sizeof(IPC_HEAD)))
                {
                    if (usActuSendLen != 0)
                        ipc_debug_printf("IPC Kernel send fewer data length of register ACK.\r\n");
                }
                break;
                
            case IPC_MSG_DISREG_M:
                ipc_debug_printf("IPC kernel rec module dis register msg.\r\n");
                ulRet=ipc_ker_disregmod_proc((IPC_DIS_REG_MODULE_MSG_INFO*)pRecData,
                       pSendData,&usSendLen);
                if(IPC_SUCCESS!=ulRet)
                {
                    ipc_debug_printf("IPC Kernel process module disregister FAIL.\r\n");
                    break;
                }
                if(IPC_SUCCESS!=ipc_ker_prepare_ackhead(pRecData,pSendData,IPC_MSG_DISREG_M_ACK,ulRet))
                {
                    ipc_debug_printf("IPC Kernel prepair IPC head of register ACK FAIL.\r\n");
                    break;
                }
                usActuSendLen=ipc_ker_send_data(((IPC_DIS_REG_MODULE_MSG_INFO*)pRecData)->ucSrcMo,gIpcKerCtl.SndDataBuf,
                         usSendLen+sizeof(IPC_HEAD),&CliAddr);
                if(usActuSendLen!=(usSendLen+sizeof(IPC_HEAD)))
                {
                    if (usActuSendLen != 0)
                        ipc_debug_printf("IPC Kernel send fewer data length of disregister ACK.\r\n");
                }        
                
                {
                    UCHAR  ucMo = ((IPC_DIS_REG_MODULE_MSG_INFO*)pRecData)->ucSrcMo;
                    if(gIpcKerMoCtl[ucMo].IsReged)
                    {
                        clear_snd_resource(ucMo);
                        gIpcKerMoCtl[ucMo].IsReged==FALSE;
                        memset(gIpcKerMoCtl+ucMo,0,sizeof(IPC_KER_MO_CTL));                    
                    }                
                }
                
                break;
                
            case IPC_EVENT_ENGAGE:
                ipc_debug_printf("IPC kernel rec engage event msg.\r\n");
                ulRet = ipc_ker_event_enage_proc((IPC_EVENT_CMD_MSG_INFO*)pRecData,pSendData,&usSendLen);
                if(IPC_SUCCESS!=ulRet)
                {
                    ipc_debug_printf("IPC Kernel process event engage FAIL ,code=%lu .\r\n",ulRet);                    
                }
                if(IPC_SUCCESS!=ipc_ker_prepare_ackhead(pRecData,pSendData,
                        IPC_EVENT_ENGAGE_ACK,ulRet))
                {
                    ipc_debug_printf("IPC Kernel prepair IPC head of event engage ACK FAIL.\r\n");
                    break;
                }                
                usActuSendLen=ipc_ker_send_data(((IPC_EVENT_CMD_MSG_INFO*)pRecData)->ucSrcMo,gIpcKerCtl.SndDataBuf,
                         usSendLen+sizeof(IPC_HEAD),&CliAddr);
                if(usActuSendLen!=(usSendLen+sizeof(IPC_HEAD)))
                {
                    if (usActuSendLen != 0)
                        ipc_debug_printf("IPC Kernel send fewer data length of event engage ACK.\r\n");
                }
                break;
                
            case IPC_EVENT_DIS_ENGAGE:
                ipc_debug_printf("IPC kernel rec dis engage event msg.\r\n");
                ulRet=ipc_ker_event_disenage_proc((IPC_EVENT_CMD_MSG_INFO*)pRecData,pSendData,&usSendLen);
                
                if(IPC_SUCCESS!=ipc_ker_prepare_ackhead(pRecData,pSendData,
                        IPC_EVENT_DIS_ENGAGE,ulRet))
                {
                    ipc_debug_printf("IPC Kernel prepair IPC head of event dis engage ACK FAIL.\r\n");
                    break;
                }
                usActuSendLen=ipc_ker_send_data(((IPC_EVENT_CMD_MSG_INFO*)pRecData)->ucSrcMo,gIpcKerCtl.SndDataBuf,
                         usSendLen+sizeof(IPC_HEAD),&CliAddr);
                if(usActuSendLen!=(usSendLen+sizeof(IPC_HEAD)))
                {
                    if (usActuSendLen != 0)
                        ipc_debug_printf("IPC Kernel send fewer data length of event engage ACK.\r\n");
                }                
                break;
                
            case IPC_EVENT_RELEASE:
                ipc_debug_printf("IPC kernel rec release event msg.\r\n");
                ipc_ker_event_release_proc((IPC_EVENT_R_INFO*)pRecData);
                break;
                
            case IPC_MSG_CMD:
            case IPC_MSG_ECHO:
            case IPC_MSG_NOTIFY:
                ipc_debug_printf("IPC kernel rec  msg type:%d.\r\n",pIpc->ucMsgType);
                ulRet=ipc_ker_cmd_msg_proc(pRecData,pIpc->ucMsgType);
                if ((ulRet!=IPC_SUCCESS ) && (ulRet != IPC_SEND_BLOCK))
                {
                    ipc_debug_printf("IPC kernel forward cmd msg FAIL.\r\n");
                    if (pIpc->SynFlag == IPC_SYN_MSG) /*ֻ��ͬ����Ϣ����Ҫ��Ӧ*/
                    {
                        IPC_HEAD  *pIpc = (IPC_HEAD*)IPC_APP_MEM_TO_IPC(pRecData);
                        ipc_ker_prepare_ackhead(pRecData,pSendData,
                            IPC_MSG_ACK,ulRet);
                        ipc_ker_send_data(pIpc->ucSrcMo,gIpcKerCtl.SndDataBuf,
                             sizeof(IPC_HEAD),&CliAddr);
                    }
                }
                break;
/****************************************************
��������е�ack��Ϣ���ŵ���ACKSoAddr,
����Ϊ�첽��Ϣ�Ĵ��������ص�ACK
����app�б����˵�
*****************************************************/                
            case IPC_MSG_ACK:
                ipc_debug_printf("IPC kernel rec ack msg.\r\n");
                ulRet=ipc_ker_ack_msg_proc(pRecData,pIpc->ucMsgType);
                if ((ulRet!=IPC_SUCCESS) && (ulRet != IPC_SEND_BLOCK))
                {
                    ipc_debug_printf("IPC kernel forward ack  msg FAIL.\r\n");
                }
                
                break;

            default :
                ipc_debug_printf("IPC kernel warning: rec unknown msg.\r\n");

                break;
        }
        ipc_debug_printf("ipcker to process msg end.\n");//for test only.
        
    }

}


