/******************************************************************************
Copyright (C), 2014-2024, C-Data Tech. Co., Ltd.
  �� �� ��   : ipc_if.c
  ��    ��   : jh.liang
  �� �� ��   : 1.0.0 
  ��������   : 2015��3��13��
  ��������   :  ���ļ�����IPC�ӿ�ʵ�ֺ���
******************************************************************************/

#include <sys/socket.h>
#include <sys/un.h>
#include "pthread.h"
#include "ipc_if.h"
#include "ipc_private.h"
#include "stdio.h"
#include "malloc.h"
#include "stdlib.h" 
#include <fcntl.h>

IPC_IF_MODULE_INFO   gIfCtl;
/*��Ϊ��ģ���ж��Ƕ��߳�,�����ṩ���շ����ļ�������*/
static pthread_mutex_t gSnMutex= PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
static pthread_mutex_t gSendSynMutex= PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/*modInit�ṩ��ʼ����־,gModInitMutex�ṩ�Գ�ʼ����־������*/
pthread_mutex_t gModInitMutex ;
int modInit = 0;/*�����ڲ����̼߳��ͬ��*/

pthread_mutex_t gSdkInitMutex ;
int sdkInited = 0;/*bcm.user�͸���ģ���Ľ���ͬ��*/

char mips_reg[][10]={
    "0  ($ 0)","AT ($ 1)","v0 ($ 2)","v1 ($ 3)",
    "a0 ($ 4)","a1 ($ 5)","a2 ($ 6)","a3 ($ 7)",
    "t0 ($ 8)", "t1 ($ 9)","t2 ($10)", "t3 ($11)",
    "t4 ($12)", "t5 ($13)","t6 ($14)", "t7 ($15)",
    "s0 ($16)", "s1 ($17)","s2 ($18)", "s3 ($19)",
    "s4 ($20)", "s5 ($21)","s6 ($22)", "s7 ($23)",
    "t8 ($24)", "t9 ($25)", "k0 ($26)","k1 ($27)",
    "gp ($28)", "sp ($29)","fp ($30)", "ra ($31)"
};

static void signal_segv(int signum, siginfo_t* info, void* ptr) 
{
    static const char *si_codes[3] = {"", "SEGV_MAPERR", "SEGV_ACCERR"};
    char    buf[256] = {0};
    int     count;
    size_t  i;
    module_killed_info module_killed_event;
    ucontext_t         *ucontext = (ucontext_t*)ptr;
    
    /*��Ϊ�Ӵ����ں�̨������ر�stdout/stdin/stderr,�������´�*/
    stderr = fopen("/dev/console","w");
    count = readlink( "/proc/self/exe", buf, 256 );
    buf[ count ] = '\0';
    fprintf(stderr, "path:%s\n",buf);
    fprintf(stderr, "\nSegmentation Fault!\n");
    fprintf(stderr, "info.si_signo = %d\n", signum);
    fprintf(stderr, "info.si_errno = %d\n", info->si_errno);
    fprintf(stderr, "info.si_code = %d (%s)\n", info->si_code, si_codes[info->si_code]);
    fprintf(stderr, "info.si_addr = %p\n", info->si_addr);

    //fprintf(stderr, "EPC=0x%08llx\n",ucontext->uc_mcontext.pc);
    fprintf(stderr, "RA=0x%08llx\n",ucontext->uc_mcontext.gregs[31]);
    for (i = 0; i < NGREG; i++)
    {
          if (i%2 == 0)
          fprintf(stderr, "\n");
          fprintf(stderr, "%s = 0x%08llx          ", mips_reg[i], ucontext->uc_mcontext.gregs[i]);
    }
    
    fprintf(stderr,"\n");
    module_killed_event.signum = signum;
    strncpy(module_killed_event.module_name,buf, sizeof(module_killed_event.module_name));
    memcpy(&module_killed_event.info,info, sizeof(siginfo_t));
    memcpy(&module_killed_event.ucontext, ucontext,sizeof(ucontext_t));
    ipc_if_release_event(IPC_EVENT_MODULE_KILLED, (char *)&module_killed_event, sizeof(module_killed_info));
    usleep(10);
    exit((0-signum));
}

int setup_sigsegv() 
{
    struct sigaction action;
    
    memset(&action, 0, sizeof(action));//
    
    action.sa_sigaction = signal_segv;
    action.sa_flags = SA_SIGINFO;
    
    if (sigaction(SIGSEGV, &action, NULL) < 0) 
    {
        perror("sigaction");
        return 0;
    }

    return 1;
}

#ifndef SIGSEGV_NO_AUTO_INIT
static void __attribute((constructor)) init_sigsegv(void) 
{
    setup_sigsegv();
}
#endif

#if DEFUNC("��Ϣ")

int ipc_if_init()
{     
    memset(&gIfCtl, 0, sizeof(IPC_IF_MODULE_INFO));
    
    gIfCtl.Registered    = FALSE;
    gIfCtl.gInitFlag     = TRUE;
    gIfCtl.CmdRecDataBuf = (char*)malloc(IPC_MSG_MAX_LENGTH);
    gIfCtl.AckRecDataBuf = (char*)malloc(IPC_MSG_MAX_LENGTH);

    if (!gIfCtl.CmdRecDataBuf || !gIfCtl.AckRecDataBuf)
    {
        return IPC_MEM_LACK;
    }

    return IPC_SUCCESS;
}

static  void ipc_if_ack_sockpath_create()
{
    sprintf(gIfCtl.SoAckPath, "%s_%d", IPC_IF_ACK_SO_PATH_PRE, gIfCtl.ucSrcMo);
}

static  void ipc_if_cmd_sockpath_create()
{
    sprintf(gIfCtl.SoCmdPath, "%s_%d", IPC_IF_CMD_SO_PATH_PRE, gIfCtl.ucSrcMo);
}

static BOOL ipc_if_snValid(USHORT usOrgSn, char *pMsg)
{
    IPC_HEAD *pIpcHead = (IPC_HEAD *)pMsg;
     
    return (usOrgSn == pIpcHead->usSn) ? TRUE : FALSE;
}

static void ipc_if_reset()
{
    close(gIfCtl.AckSockFd);
    close(gIfCtl.CmdSockFd);
    
    ipc_if_init();
}

static USHORT ipc_get_seqNo()
{
    USHORT sn;

    LOCK(gSnMutex);
    gIfCtl.usMsgSn++;
    sn = gIfCtl.usMsgSn;
    UNLOCK(gSnMutex);
    
    return sn;
}

UCHAR ipc_if_get_thismoid()
{
    if(gIfCtl.gInitFlag == TRUE)
        return gIfCtl.ucSrcMo;
        
    return 0;
}

static void *ipc_if_alloc(ULONG ulLenth)
{
    char      *pAlloc;
    IPC_HEAD  *pIpcHead;

    pAlloc = (char*)malloc(IPC_APP_MEM_LEN_TO_ACT_LEN(ulLenth));
    if(pAlloc == NULL)
        return NULL;
        
    pIpcHead = (IPC_HEAD*)pAlloc;
    
    pIpcHead->usMagic = IPC_HEAD_MAGIC;
    pIpcHead->Ver     = IPC_HEAD_CUR_VER;
    pIpcHead->NewIpcH = TRUE;
    
    return IPC_APP_MEM_OFFSET(pAlloc);
}

ULONG ipc_if_free(void* pToFree)
{
    IPC_HEAD *pIpcHead = (IPC_HEAD*)IPC_APP_MEM_TO_IPC(pToFree);
    
    if(pIpcHead->usMagic != IPC_HEAD_MAGIC)
    {
        printf("ipc_if_free IPC HEAD MAGIC error\r\n");
        return IPC_INVALID_HEAD;
    }

    pIpcHead->usMagic = 0;
    free(pIpcHead);
    
    return IPC_SUCCESS;
}

#endif

#if DEFUNC("ͬ����Ϣ")
/******************************************************************
*��Ϊ��ģ��ע���ʱ��ͬʱע����һ��������  *
*�̵Ļص�����,���ص������в����ƶԴ˺����� *
*����,�Ӷ��������߳��в����Ŀ���.                           *
*******************************************************************/
static ULONG ipc_if_send_synmsg(
                USHORT  usDstMo,
                char    *pAppMsgSend,
                USHORT  usLenSend,
                UCHAR   ucMsgType,
                char    **pMsgRec, 
                USHORT  *pLenRec)
{
    IPC_HEAD       *pIpcHead, *pRecIpcHead;
    int            Ret, RecLen, i;
    char           *pRecMsg;
    fd_set         fds;
    struct timeval timout;
    UCHAR          *pData;
    USHORT         usSn;
    
    if((pAppMsgSend == NULL) || (pLenRec == NULL) || (pMsgRec == NULL))
    {
        return IPC_NULL_PTR;
    }
    
    *pMsgRec=NULL;
    *pLenRec=0;

    pIpcHead = (IPC_HEAD*)IPC_APP_MEM_TO_IPC(pAppMsgSend);

    if(pIpcHead->usMagic != IPC_HEAD_MAGIC)
    {
        return IPC_INVALID_HEAD;
    }

    LOCK(gSendSynMutex);

    usSn = ipc_get_seqNo();
    
    pIpcHead->usSn      = usSn;
    pIpcHead->ucMsgType = ucMsgType;
    pIpcHead->ucSrcMo   = gIfCtl.ucSrcMo;
    pIpcHead->ucDstMo   = usDstMo;
    pIpcHead->usDataLen = usLenSend;
    pIpcHead->SynFlag   = IPC_SYN_MSG;
    pIpcHead->RCode     = IPC_SUCCESS;

    do
    {
        Ret = sendto(gIfCtl.AckSockFd, pIpcHead, (sizeof(IPC_HEAD) + usLenSend),
                     0, (struct sockaddr*)&gIfCtl.IpcSoAddr, sizeof(struct sockaddr));
    }while(Ret == -1 && errno == EINTR);
    
    if (Ret < 0)
    {
        perror("sendto Syn msg FAIL");
        UNLOCK(gSendSynMutex); 
        return IPC_SOCKET_SENDTO_FAIL;
    }
    
    for(;;)
    {
        FD_ZERO(&fds);
        FD_SET(gIfCtl.AckSockFd, &fds);
        timout.tv_sec  = IPC_SYN_SELECT_TIMEOUT_SEC;
        timout.tv_usec = 0;

        do 
        {
            Ret = select(gIfCtl.AckSockFd + 1, &fds, NULL, NULL, &timout);
        }while(Ret == -1 && errno == EINTR);

        if(Ret < 0)
        {
            UNLOCK(gSendSynMutex); 
            return IPC_SOCKET_SENDTO_FAIL;
        }
        
        if(Ret == 0)
        {
            printf("ipc if :waiting ACK time out  while send syn msg at line :%d.src=%d dst=%d\n",
                    __LINE__, gIfCtl.ucSrcMo, usDstMo);
            UNLOCK(gSendSynMutex);
            return IPC_TIMEOUT;
        }

        do
        {
            RecLen = recvfrom(gIfCtl.AckSockFd, gIfCtl.AckRecDataBuf, IPC_MSG_MAX_LENGTH, 
                              0, NULL, NULL);
        }while(RecLen == -1 && errno == EINTR);

        if (RecLen == -1)
        {
            perror("IPC if : recvfrom");
            continue;  
        }

        if(FALSE == ipc_if_snValid(usSn, gIfCtl.AckRecDataBuf))
        {
            printf("IPC if : Invalid ACK while send syn msg at line :%d.\n", __LINE__);
            continue;
        }
        
        break;        
    }
    
    if(RecLen < sizeof(IPC_HEAD))
    {
        printf("IPC if :Receive shortly ACK msg while send syn msg at line :%d.\n", __LINE__);
        UNLOCK(gSendSynMutex);
        return IPC_REC_INVALID_MSG;
    }
    
    pRecIpcHead = (IPC_HEAD*)gIfCtl.AckRecDataBuf;
    if(pRecIpcHead->usMagic != IPC_HEAD_MAGIC)
    {
        printf("IPC if:Syn sending :Rec Ack with Invalid IPC Head at line:%d\r\n", __LINE__);
        UNLOCK(gSendSynMutex); 
        return IPC_REC_MSG_IPC_HEAD;
    }
    
    if(pRecIpcHead->RCode != IPC_SUCCESS)
    {
        printf("IPC if:Syn sending :Rec IPC layer FAIL at line:%d, code=%d .src=%d dst=%d\r\n",__LINE__,pRecIpcHead->RCode,gIfCtl.ucSrcMo,usDstMo);
        UNLOCK(gSendSynMutex); 
        return pRecIpcHead->RCode;
    }

    pRecMsg = (char*)malloc(RecLen);
    if(pRecMsg == NULL)
    {
        UNLOCK(gSendSynMutex);
        return IPC_MEM_LACK;
    }
    
    memcpy(pRecMsg, gIfCtl.AckRecDataBuf, RecLen);
    *pMsgRec = IPC_APP_MEM_OFFSET(pRecMsg);
    *pLenRec = RecLen - sizeof(IPC_HEAD);

    UNLOCK(gSendSynMutex); 
    return IPC_SUCCESS;
}

ULONG ipc_if_get_cmd_result(
                unsigned short dstModuleID,
                short          MsgID,
                char           *cmd,
                int            cmdlen,
                char           *rcvbuf,
                int            buflen,
                short          *pRetCode)
{
    int         ret=0;
    IPC_APP_MSG *pSend;
    USHORT      msgLen;
    IPC_APP_MSG *pRcv;
    USHORT      rcvLen;

    msgLen = sizeof(IPC_APP_MSG) + cmdlen;
    pSend = (IPC_APP_MSG*)ipc_if_alloc(msgLen);
    if (pSend == NULL)
    {
        printf("error occured!allocate memory fail! MsgID=%d\n", MsgID); 
        return IPC_NULL_PTR;
    }
    
    pSend->MsgHead.MsgID   = MsgID;
    pSend->MsgHead.DataLen = cmdlen;
    if (cmd)
        memcpy(pSend->data,cmd,cmdlen);
    
    ret = ipc_if_send_synmsg(dstModuleID, (char*)pSend, msgLen, IPC_MSG_CMD, (char **)&pRcv, &rcvLen);
    ipc_if_free(pSend);
    if (ret == IPC_INVALID_MODULE)
    {
        printf("error occured! invalid module:%d\r\n", dstModuleID); 
        return ret;
    }
    else if (ret == IPC_SOCKET_SENDTO_FAIL)
    {
        printf("error occured!send message fail! MsgID=%d\r\n", MsgID); 
        return ret;
    }
    else if (ret == 0)
    {
        if ( (rcvLen - sizeof(IPC_APP_MSG)) != buflen)
        {
            printf("error occured!receive bad message. MsgID=%d, rcvLen=%d expLen=%d\n", 
                   MsgID, rcvLen - sizeof(IPC_APP_MSG), buflen); 
            ipc_if_free(pRcv);
            return IPC_REC_INVALID_MSG;
        }        
  
        memcpy(rcvbuf, pRcv->data, buflen);
        
        if(pRetCode)
        {
            *pRetCode = pRcv->MsgHead.RetCode;
        }
        
        ipc_if_free(pRcv);
    }
    else
    {
        printf("error occured!ipc return error . MsgID=%d\n", MsgID); 
        return IPC_SOCKET_SENDTO_FAIL;
    }

    return IPC_SUCCESS;
}

ULONG ipc_if_exe_cmd(
            unsigned short dstModuleID,
            short          MsgID, 
            char           *cmddata,
            int            cmdlen, 
            short          *pRetCode)
{
    int           ret;
    IPC_APP_MSG   *pSend;
    USHORT        msgLen;
    IPC_APP_MSG   *pRcv;
    USHORT        rcvLen;

    msgLen = sizeof(IPC_APP_MSG) + cmdlen;
    pSend  = (IPC_APP_MSG*)ipc_if_alloc(msgLen);
    if (pSend == NULL)
    {
        printf("\r\nerror occured!allocate memory fail! MsgID=%d\n", MsgID);
        return -1;
    }
    
    pSend->MsgHead.MsgID   = MsgID;
    pSend->MsgHead.DataLen = cmdlen;
    memcpy(pSend->data, cmddata, cmdlen);
    
    ret = ipc_if_send_synmsg(dstModuleID, (char*)pSend, msgLen, IPC_MSG_CMD, (char **)&pRcv, &rcvLen);
    ipc_if_free(pSend);
    if (ret == IPC_INVALID_MODULE)
    {
        printf("error occured!%d not in running!MsgID=%d", dstModuleID, MsgID); 
        return ret;
    }
    else if (ret == IPC_SOCKET_SENDTO_FAIL)
    {
        printf("\r\nerror occured!send message fail! MsgID=%d\n", MsgID); 
        return ret;
    }
    else if (ret == 0)
    {
        if(pRetCode)
        {
            *pRetCode = pRcv->MsgHead.RetCode;
        }
        
        ipc_if_free(pRcv);
    }
    else
    {
        printf("\r\nerror occured!ipc return error ! MsgID=%d\n", MsgID); 
        return ret;
    }

    return IPC_SUCCESS;
}

#endif

#if DEFUNC("�첽��Ϣ")

static ULONG ipc_if_send_asynmsg(
                USHORT usDstMo, 
                char   *pAppMsgSend,
                USHORT usLenSend,
                UCHAR  ucMsgType)
{
    IPC_HEAD  *pIpcHead;
    int       AddrLen, Ret;
    USHORT    usSn;

    if(pAppMsgSend == NULL)
    {
        return IPC_NULL_PTR;
    }
    
    pIpcHead = (IPC_HEAD*)IPC_APP_MEM_TO_IPC(pAppMsgSend);
    if(pIpcHead->usMagic != IPC_HEAD_MAGIC)
    {
        return IPC_INVALID_HEAD;
    }
    
    if(pIpcHead->NewIpcH == TRUE)
    {
        usSn = ipc_get_seqNo(gIfCtl);
        
        pIpcHead->usSn      = usSn;
        pIpcHead->ucMsgType = ucMsgType;
        pIpcHead->ucSrcMo   = gIfCtl.ucSrcMo;
        pIpcHead->ucDstMo   = usDstMo;
        pIpcHead->usDataLen = usLenSend;
        pIpcHead->SynFlag   = IPC_ASYN_MSG;
        pIpcHead->RCode     = IPC_SUCCESS;
    }
    else
    {
        pIpcHead->ucMsgType = ucMsgType;
        pIpcHead->usDataLen = usLenSend;
        pIpcHead->RCode     = IPC_SUCCESS;
    }   

    do
    {
        Ret = sendto(gIfCtl.CmdSockFd, pIpcHead, (sizeof(IPC_HEAD) + usLenSend),
                     0, (struct sockaddr*)&gIfCtl.IpcSoAddr, sizeof(struct sockaddr));
    }while(Ret == -1 && errno == EINTR);
    
    if (Ret < 0)
    {
        perror("sendto Asyn msg FAIL");
        return IPC_SOCKET_SENDTO_FAIL;
    }

    return IPC_SUCCESS;
}

static ULONG ipc_if_preack(char *pRecMsg, char *pSendMsg)
{
    IPC_HEAD *pIpcRecHead, *pIpcSendHead;

    if((pRecMsg == NULL) || (pSendMsg == NULL))
    {
        return IPC_NULL_PTR;
    }
    
    pIpcRecHead = (IPC_HEAD*)IPC_APP_MEM_TO_IPC(pRecMsg);
    if(pIpcRecHead->usMagic != IPC_HEAD_MAGIC)
    {
        return IPC_INVALID_HEAD;
    }
    
    pIpcSendHead = (IPC_HEAD*)IPC_APP_MEM_TO_IPC(pSendMsg);
    pIpcSendHead->SynFlag   = pIpcRecHead->SynFlag;
    pIpcSendHead->usSn      = pIpcRecHead->usSn;
    pIpcSendHead->ucSrcMo   = pIpcRecHead->ucDstMo;
    pIpcSendHead->ucDstMo   = pIpcRecHead->ucSrcMo;
    pIpcSendHead->ucMsgType = IPC_MSG_ACK;
    //���û�Ӧ����Ҫ���¹���ͷ����Ϣ
    pIpcSendHead->NewIpcH   = FALSE;

    return IPC_SUCCESS;
}

ULONG ipc_if_send_ack(
            UCHAR   ucDstMo,
            char    *PRecMsg,
            short   RetCode,
            char    *pAckData,
            USHORT  AckDataLen)
{
    IPC_APP_MSG *pAckMsg = NULL;
    USHORT      AckMsgLen;
    int         ret = 0;
        
    //�����Ӧ��
    AckMsgLen = sizeof(IPC_APP_MSG) + AckDataLen;
    pAckMsg   = (IPC_APP_MSG*)ipc_if_alloc(AckMsgLen);
    if (pAckMsg == NULL) 
    {
        printf("ipc_if_alloc allocate memory fail!\r\n");    
        ret = -1;
        goto end;
    }

    if (ipc_if_preack(PRecMsg, (char*)pAckMsg)) 
    {
        printf("ipc_if_preack fail!\r\n");    
        ret = -1;
        goto end;
    }      
    
    //��ϢID ���λ(APP_MSG_ACK_BIT)Ϊ1��ʾ�Ǹ���Ϣ�Ļ�Ӧ��
    pAckMsg->MsgHead.MsgID   = ((IPC_APP_MSG*)PRecMsg)->MsgHead.MsgID | APP_MSG_ACK_BIT;
    pAckMsg->MsgHead.RetCode = RetCode;
    pAckMsg->MsgHead.DataLen = AckDataLen;
    if (AckDataLen) 
    {
        bcopy(pAckData, pAckMsg->data, AckDataLen);
    }
    
    if (ipc_if_send_asynmsg(ucDstMo, (char*)pAckMsg, AckMsgLen, IPC_MSG_ACK)) 
    {
        printf("IPC send ack fail!\n");    
        ret = -1;
        goto end;
    }
    
end:        
    if (pAckMsg)
        ipc_if_free(pAckMsg);
        
    return ret;
}

ULONG ipc_if_release_event(UCHAR EventId, char *pDataSend, USHORT usLenSend)
{
    IPC_EVENT_R_INFO *pEventMsg;
    int              Ret;

    if((pDataSend == NULL) && (usLenSend != 0))
    {
        return IPC_NULL_PTR;
    }

    pEventMsg = (IPC_EVENT_R_INFO*)ipc_if_alloc(sizeof(IPC_EVENT_R_INFO) + usLenSend);
    if(pEventMsg == NULL)
        return IPC_MEM_LACK;
    
    pEventMsg->EventMsgHead.ucEventId = EventId;
    pEventMsg->EventMsgHead.usLen     = usLenSend;

    if (pDataSend)
        memcpy(pEventMsg->data, pDataSend, usLenSend);
    
    Ret = ipc_if_send_asynmsg(MODULE_IPC, (char*)pEventMsg, (sizeof(IPC_EVENT_R_INFO) + usLenSend),
                              IPC_EVENT_RELEASE);
    
    ipc_if_free(pEventMsg);

    return Ret;
}

#endif

#if DEFUNC("task")

static void ipc_if_reccmd_thread(void* arg)
{
    int            Ret;
    int            RecLen;
    char           *pRecMsg;
    IPC_HEAD       *pIpcHead;
    char           *pAppData;
    USHORT         usAppDataLen;
    UCHAR          ucType;
    UCHAR          ucSrcMo;
    int            i;
    UCHAR          *pDebug;
    fd_set         fds;
    struct timeval timout;
 
    for(;;)
    {
        FD_ZERO(&fds);
        FD_SET(gIfCtl.CmdSockFd, &fds);
        
        timout.tv_sec = IPC_SYN_SELECT_TIMEOUT_SEC;
        timout.tv_usec = 0;

        do
        {
            Ret = select(gIfCtl.CmdSockFd + 1, &fds, NULL, NULL, &timout);  
        }while(Ret == -1 && errno == EINTR);

        if(Ret < 0)
        {
            perror("ipc_if select error");
        }
        
        if(Ret == 0)
        {
            continue;
        }

        do
        {
            RecLen = recvfrom(gIfCtl.CmdSockFd, gIfCtl.CmdRecDataBuf, IPC_MSG_MAX_LENGTH,
                              0, NULL, NULL);
        }while(RecLen == -1 && errno == EINTR);

        if (RecLen == -1)
        {
            perror("IPC if : recvfrom");
            continue;
        }

        if(RecLen <= sizeof(IPC_HEAD))
        {
            continue;
        }

        pRecMsg = (char*)malloc(RecLen);
        if(pRecMsg == NULL)
        {
            perror("malloc");
            continue;
        }
        
        memcpy(pRecMsg, gIfCtl.CmdRecDataBuf, RecLen);
        
        pIpcHead     = (IPC_HEAD*)pRecMsg;
        ucType       = pIpcHead->ucMsgType;
        ucSrcMo      = pIpcHead->ucSrcMo;
        pAppData     = IPC_APP_MEM_OFFSET(pRecMsg);
        usAppDataLen = RecLen - sizeof(IPC_HEAD);

        if(gIfCtl.pIfCallBack)
        {
            gIfCtl.pIfCallBack(pAppData, usAppDataLen, ucType, ucSrcMo);
        }
    }
}

static ULONG  ipc_if_cmdsock_task_init()
{
    pthread_t          thread;
    pthread_attr_t     attr;
    struct sched_param sched;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
    pthread_attr_setstacksize(&attr, IPC_REC_CMD_DEFAULT_STACKSIZE);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM );
    
    if (pthread_create(&thread, &attr, (void*)ipc_if_reccmd_thread, NULL) < 0)
    {
        perror("pthread_create");
        return IPC_COMMON_FAIL;
    }
    
    sched.sched_priority = 50;
    pthread_setschedparam(thread, SCHED_RR, &sched);

    return IPC_SUCCESS;
}

#endif

#if DEFUNC("ģ�����¼�ע��")

ULONG ipc_if_reg_module(UCHAR srcMo, char *pNameMo, IPC_MSG_CALLBACK pCallBack)
{
    IPC_REG_MODULE_INFO     *pRegInfo;
    IPC_COMMON_REG_ACK_INFO *pRegAck;
    char                    *pMsgRec;
    USHORT                  usMsgRecLen;
    int                     Ret, sndbuf = 64*1024;
    
    if(gIfCtl.gInitFlag != TRUE)
    {
        return IPC_NOT_INIT;
    }

    if(gIfCtl.Registered == TRUE)
    {
        return IPC_REG_DUP;
    }
    
    if(strlen(pNameMo) > IPC_MODULE_NAME_MAX_LENGTH)
    {
        return IPC_SRC_MO_NAME_INVALID;
    }

    pRegInfo = (IPC_REG_MODULE_INFO*)ipc_if_alloc(sizeof(IPC_REG_MODULE_INFO));
    if(pRegInfo == NULL)
        return IPC_MEM_LACK;
        
    gIfCtl.ucSrcMo = srcMo;
    strcpy(gIfCtl.NameMo, pNameMo);

    ipc_if_ack_sockpath_create(gIfCtl);/*accept ack*/
    ipc_if_cmd_sockpath_create(gIfCtl);/*accept cmd*/

    gIfCtl.IpcSoAddr.sun_family = AF_UNIX;
    sprintf(gIfCtl.IpcSoAddr.sun_path,"%s", IPC_MAIN_REC_SO_PATH);

    gIfCtl.AckSoAddr.sun_family = AF_UNIX;
    sprintf(gIfCtl.AckSoAddr.sun_path,"%s", gIfCtl.SoAckPath);
    
    if ((gIfCtl.AckSockFd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket Create Error");
        return IPC_SOCK_CREATE_FAIL;
    }
    
    unlink(gIfCtl.SoAckPath);
    
    if (bind(gIfCtl.AckSockFd, (struct sockaddr*)&gIfCtl.AckSoAddr, sizeof(struct sockaddr)) < 0)
    {
        perror("bind Error");
        return IPC_SOCK_CREATE_FAIL;
    }
    
    pRegInfo->ucSrcMo = srcMo;
    strcpy(pRegInfo->NameMo, pNameMo);
    strcpy(pRegInfo->SoAckPath, gIfCtl.SoAckPath);
    strcpy(pRegInfo->SoCmdPath, gIfCtl.SoCmdPath);
    
    for(;;)
    {
        Ret = ipc_if_send_synmsg(MODULE_IPC, (char*)pRegInfo, sizeof(IPC_REG_MODULE_INFO),
                                 IPC_MSG_REG_M, &pMsgRec, &usMsgRecLen);
        if(IPC_SOCKET_SENDTO_FAIL == Ret)
        {
            sleep(2);
        }
        else
        {
            break;
        }
    }
    
    if(Ret != IPC_SUCCESS)
    {
        ipc_if_free(pRegInfo);
        return Ret;
    }
    
    pRegAck = (IPC_COMMON_REG_ACK_INFO*)pMsgRec;
    if((pRegAck->ucSrcMo != srcMo) || (pRegAck->ucRetCode != IPC_REG_M_ACK))
    {
        ipc_if_free(pRegInfo);
        ipc_if_free(pMsgRec);
        return IPC_COMMON_FAIL;
    }
    
    ipc_if_free(pRegInfo);
    ipc_if_free(pMsgRec);

    gIfCtl.CmdSoAddr.sun_family = AF_UNIX;
    sprintf(gIfCtl.CmdSoAddr.sun_path, "%s", gIfCtl.SoCmdPath);
    
    if ((gIfCtl.CmdSockFd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("Create socket Error");
        return IPC_SOCK_CREATE_FAIL;
    }
    
    if (setsockopt(gIfCtl.CmdSockFd, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf)) < 0) 
    {
        printf("set SO_SNDBUF Error.\n");
    }
    
    if (setsockopt(gIfCtl.CmdSockFd, SOL_SOCKET, SO_RCVBUF, &sndbuf, sizeof(sndbuf)) < 0) 
    {
        printf("set SO_RCVBUF Error.\r\n");
    }
    
    unlink(gIfCtl.SoCmdPath);
    
    if (bind(gIfCtl.CmdSockFd, (struct sockaddr*)&gIfCtl.CmdSoAddr, sizeof(struct sockaddr)) < 0)
    {
        perror("bind Error");
        return IPC_SOCK_CREATE_FAIL;
    }
    
    if(pCallBack == NULL)
    {
        gIfCtl.Registered = TRUE;
        return IPC_SUCCESS;
    }
    
    gIfCtl.pIfCallBack = pCallBack;
    if (IPC_SUCCESS != ipc_if_cmdsock_task_init()) 
    {
        printf("Create socket Thread FAIL.\r\n");
        return IPC_CREATE_THREAD_FAIL;
    }

    gIfCtl.Registered = TRUE;
    return IPC_SUCCESS;
}

ULONG ipc_if_disreg_module()
{
    IPC_DIS_REG_MODULE_MSG_INFO *pDisRegMsg;
    IPC_COMMON_REG_ACK_INFO    *pAck;
    char *pMsgRec;
    USHORT usMsgRecLen;    
    int Ret;
    UCHAR srcMo = gIfCtl.ucSrcMo;

    if(gIfCtl.Registered==FALSE)
    {
        return IPC_SUCCESS;
    }
    pDisRegMsg=(IPC_DIS_REG_MODULE_MSG_INFO*)ipc_if_alloc(sizeof(IPC_DIS_REG_MODULE_MSG_INFO));
    if(pDisRegMsg==NULL)
        return IPC_MEM_LACK;
        
    pDisRegMsg->ucSrcMo=srcMo;
    
    Ret=ipc_if_send_synmsg(MODULE_IPC,(char*)pDisRegMsg,sizeof(IPC_DIS_REG_MODULE_MSG_INFO),
        IPC_MSG_DISREG_M,&pMsgRec,&usMsgRecLen);
    if(Ret!=IPC_SUCCESS)
    {
        ipc_if_free(pDisRegMsg);
        return Ret;
    }    
    
    pAck=(IPC_COMMON_REG_ACK_INFO*)pMsgRec;
    if((pAck->ucSrcMo!=srcMo)||(pAck->ucRetCode !=IPC_DISREG_M_ACK))
    {
        ipc_if_free(pDisRegMsg);
        ipc_if_free(pMsgRec);
        return IPC_COMMON_FAIL;
    }
    
    ipc_if_reset();
    
    ipc_if_free(pDisRegMsg);
    ipc_if_free(pMsgRec);
    
    return IPC_SUCCESS;
}

ULONG ipc_if_engage_event(UCHAR EventId)
{
    IPC_EVENT_CMD_MSG_INFO    *pEngageEvent;
    IPC_COMMON_REG_ACK_INFO   *pAck;
    char                      *pMsgRec;
    USHORT                    usMsgRecLen;    
    int                       Ret;
    UCHAR                     srcMo = gIfCtl.ucSrcMo;

    pEngageEvent = (IPC_EVENT_CMD_MSG_INFO*)ipc_if_alloc(sizeof(IPC_EVENT_CMD_MSG_INFO));
    if(pEngageEvent == NULL)
        return IPC_MEM_LACK;
        
    pEngageEvent->ucSrcMo   = srcMo;
    pEngageEvent->ucEventId = EventId;
    
    Ret=ipc_if_send_synmsg(MODULE_IPC, 
                           (char*)pEngageEvent, sizeof(IPC_EVENT_CMD_MSG_INFO), IPC_EVENT_ENGAGE, 
                           &pMsgRec, &usMsgRecLen);
    if(Ret != IPC_SUCCESS)
    {
        ipc_if_free(pEngageEvent);
        return Ret;
    }    
    
    pAck = (IPC_COMMON_REG_ACK_INFO*)pMsgRec;

    if((pAck->ucSrcMo != srcMo) || (pAck->ucRetCode != IPC_ENGAGE_EVENT_ACK))
    {
        ipc_if_free(pEngageEvent);
        ipc_if_free(pMsgRec);
        return IPC_COMMON_FAIL;
    }

    ipc_if_free(pEngageEvent);
    ipc_if_free(pMsgRec);
    
    return IPC_SUCCESS;  
}

ULONG ipc_if_disengage_event(UCHAR EventId)
{
    IPC_EVENT_CMD_MSG_INFO *pEngageEvent;
    IPC_COMMON_REG_ACK_INFO    *pAck;
    char *pMsgRec;
    USHORT usMsgRecLen;    
    int Ret;
    UCHAR srcMo = gIfCtl.ucSrcMo;

    pEngageEvent=(IPC_EVENT_CMD_MSG_INFO*)ipc_if_alloc(sizeof(IPC_EVENT_CMD_MSG_INFO));
    if(pEngageEvent==NULL)
        return IPC_MEM_LACK;
    pEngageEvent->ucSrcMo=srcMo;
    pEngageEvent->ucEventId=EventId;
    
    Ret=ipc_if_send_synmsg(MODULE_IPC,(char*)pEngageEvent,sizeof(IPC_EVENT_CMD_MSG_INFO),
        IPC_EVENT_DIS_ENGAGE,&pMsgRec,&usMsgRecLen);
    if(Ret!=IPC_SUCCESS)
    {
        ipc_if_free(pEngageEvent);
        return Ret;
    }    
    pAck=(IPC_COMMON_REG_ACK_INFO*)pMsgRec;

    if((pAck->ucSrcMo!=srcMo)||(pAck->ucRetCode !=IPC_DISENGAGE_EVENT_ACK))
    {
        ipc_if_free(pEngageEvent);
        ipc_if_free(pMsgRec);
        return IPC_COMMON_FAIL;
    }

    ipc_if_free(pEngageEvent);
    ipc_if_free(pMsgRec);
    
    return IPC_SUCCESS;
}

#endif

#if DEFUNC("�ź���")

int ipc_key_get(char *path, int sub_key)
{
    key_t key;
    
    if ((key = ftok(path, sub_key)) < 0) 
    {
        perror("ftok");
        return -1;
    }

    return key;
}

//�����ź���
int ipc_sem_create(key_t key)
{
    int semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid == -1)
        ERR_EXIT("semget");

    return semid;
}

int ipc_sem_open(key_t key)
{
    int semid = semget(key, 0, 0);
    if (semid == -1)
    	ERR_EXIT("semget");
    	
    return semid;
}

int ipc_sem_setval(int semid, int val)
{
    union semun su;
    int         ret;

    su.val = val;
    ret = semctl(semid, 0, SETVAL, su);
    if (ret == -1)
    	ERR_EXIT("sem_setval");

    return 0;
}

int ipc_sem_getval(int semid, int val)
{
    union semun su;
    int ret;

    su.val = val;
    ret = semctl(semid, 0, GETVAL, su);
    if (ret == -1)
    	ERR_EXIT("sem_setval");

    return 0;
}

int ipc_sem_del(int semid)
{
    int ret;
    ret = semctl(semid, 0, IPC_RMID, 0);
    if (ret == -1)
    	ERR_EXIT("semctl");

    return 0;
}

/*
struct sembuf {
    short sem_num; 
    short sem_op; 
    short sem_flg; 
};
*/

int ipc_sem_p(int semid)
{
    int	       ret;
    struct sembuf sp = {0, -1, 0};

    do
    {
        ret =  semop(semid, &sp, 1); //�������������ź����Ĳ��� IPC_NOWAIT and SEM_UNDO
    }while(ret == -1 && errno == EINTR);

    if (ret == -1)
        ERR_EXIT("semctl");

    return ret;
}

int ipc_sem_v(int semid)
{
    struct sembuf sp = {0, 1, 0};
    int           ret;

    do
    {
        ret = semop(semid, &sp, 1); //�������������ź����Ĳ���
    }while(ret == -1 && errno == EINTR);

    if (ret == -1)
        ERR_EXIT("semctl");
        
    return ret;
}
       
#endif

#if DEFUNC("�����ڴ�")
/*
key        shmid      owner     perms      bytes      nattch     status      
0x00002234 131073     wbm01     666        68         0    
*/
int ipc_shm_create(key_t key, size_t size)
{  
    int shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666); 
    if (shmid == -1)
    {
        //�൱�ڴ��ļ����ļ�����
        perror("shmget");
    }

    return shmid;
}
    
int ipc_shm_open(key_t key)
{
    int shmid = shmget(key, 0, 0);
    if (shmid == -1)
        ERR_EXIT("shmget");
        
    return shmid;
}

void* ipc_shm_map(int shmid)
{
    void *p = shmat(shmid, NULL, 0);//SHM_RND��SHM_RDONLY
    if (p == (void *)-1)
    {
        perror("shmget");
        return NULL;
    }

    return p;
}

int ipc_shm_unmap(void *p_addr)
{
    int ret = shmdt(p_addr);
    
    if (ret == -1)
    {
        perror("shmdt");
        ret = errno;
    }

    return ret;
}

int ipc_shm_del(int shmid)
{
    int ret = shmctl(shmid, IPC_RMID, NULL);
    if (ret < 0)
    {
        perror("shmctl delete");
        ret = errno;
    }

    return ret;
}

#endif

#if DEFUNC("��Ϣ����")

int 
ipc_mq_create (key_t key)
{
    int msg_id;

    msg_id = msgget(key, IPC_CREAT |IPC_EXCL|0666); //421 rwx IPC_PRIVATE
    if(-1 == msg_id)
    {
        perror("msgget");
    }

    return msg_id;
}

int 
ipc_mq_open(key_t key)
{
    int msg_id;

    msg_id = msgget(key, IPC_CREAT|0666);
    if (msg_id == -1)
    {
        perror("msgget");
    }

    return msg_id;
}

int ipc_mq_size_set(int msg_id, unsigned int size)
{
    struct msqid_ds msg_ds;

    if (msgctl(msg_id, IPC_STAT, &msg_ds) == -1)
    {
        perror("msgctl stat");
        return -1;
    }
    
    msg_ds.msg_qbytes = size;

    if(msgctl(msg_id, IPC_SET, &msg_ds) ==  -1)
    {
        perror("msgctl set");
        return -1;
    }

    return 0;    
}

int
ipc_mq_out (
            int             mq_id, 
            long            type, 
            void            *p_data, 
            unsigned int    size, 
            int             block_flag,
            unsigned int    *p_copied)
{
    int msg_flag = (block_flag == WAIT_FOREVER) ? 0 : IPC_NOWAIT;

    if(!p_data || !p_copied) 
    {
        printf("Get msg failed cause some parameter is NULL.\r\n");
        return -1;
    }

    do
    {
        *p_copied = msgrcv(mq_id, p_data, size, type, msg_flag);
    }while(*p_copied == -1 && errno == EINTR);
    
    if (*p_copied == -1)
    {
        perror("msgrcv");//ENOMSG
        *p_copied = 0;
        
        return -1;
    }

    return 0;
}

int
ipc_mq_out_timeout (
                int             mq_id, 
                long            type, 
                void            *p_data, 
                unsigned int    size, 
                unsigned int    timeout, /* ms  */
                unsigned int    *p_copied)
{
    int ret;
    
    if(!p_data || !p_copied) 
    {
        printf("Get msg failed cause some parameter is NULL.\r\n");
        return -1;
    }

    /* Read the socket for data */
    if (timeout)
    {
        int timeloop;
        
        for (timeloop = timeout; timeloop > 0; timeloop = timeloop - 100)
        {
            ret = ipc_mq_out(mq_id, type, p_data, size, NO_WAIT, p_copied);
            if (ret == -1 && errno == EAGAIN)
            {
                usleep(100 * 1000);
            }
            else if (ret == -1)
            {
               break;
            }
            else
            {
                break;
            }
        }

        return ret;
    }

    return ipc_mq_out(mq_id, type, p_data, size, WAIT_FOREVER, p_copied);
}

int
ipc_mq_in (
            int           mq_id, 
            void          *p_data, 
            unsigned int  size, 
            int           block_flag)
{
    int msg_flag = (NO_WAIT == block_flag) ? IPC_NOWAIT : 0, ret;
    
    if (!p_data) 
    {
        printf("Can not put NULL msg into the queue. \r\n");
        return -1;
    }
    
    do
    {
        ret = msgsnd(mq_id, p_data, size, msg_flag);
    }while(ret == -1 && errno == EINTR);
    
    if(ret == -1)
    {
        perror("msgsnd");//EAGAIN
              
        return -1;
    }

    return 0;
}

#endif

