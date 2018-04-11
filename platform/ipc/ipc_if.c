/******************************************************************************
Copyright (C), 2014-2024, C-Data Tech. Co., Ltd.
  文 件 名   : ipc_if.c
  作    者   : jh.liang
  版 本 号   : 1.0.0 
  生成日期   : 2015年3月13日
  功能描述   :  此文件定义IPC接口实现函数
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
#include <unistd.h>

IPC_IF_MODULE_INFO   gIfCtl;
/*因为在模块中都是多线程,这里提供对收发包的计数上锁*/
static pthread_mutex_t gSnMutex= PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
static pthread_mutex_t gSendSynMutex= PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/*modInit提供初始化标志,gModInitMutex提供对初始化标志的上锁*/
pthread_mutex_t gModInitMutex ;
int modInit = 0;/*进程内部多线程间的同步*/

pthread_mutex_t gSdkInitMutex ;
int sdkInited = 0;/*bcm.user和各子模块间的进程同步*/

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
    
    /*因为子代理在后台启动会关闭stdout/stdin/stderr,这里重新打开*/
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
    fprintf(stderr, "RA=0x%08llx\n", (long long unsigned int)ucontext->uc_mcontext.gregs[31]);
    for (i = 0; i < NGREG; i++)
    {
          if (i%2 == 0)
          fprintf(stderr, "\n");
          fprintf(stderr, "%s = 0x%08llx          ", mips_reg[i], (long long unsigned int)ucontext->uc_mcontext.gregs[i]);
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

#if DEFUNC("消息")

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

#if DEFUNC("同步消息")
/******************************************************************
*因为在模块注册的时候同时注册了一个接收线  *
*程的回调函数,而回调函数中不限制对此函数的 *
*调用,从而此两个线程有并发的可能.                           *
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
    int            Ret, RecLen;
    char           *pRecMsg;
    fd_set         fds;
    struct timeval timout;
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

#if DEFUNC("异步消息")

static ULONG ipc_if_send_asynmsg(
                USHORT usDstMo, 
                char   *pAppMsgSend,
                USHORT usLenSend,
                UCHAR  ucMsgType)
{
    IPC_HEAD  *pIpcHead;
    int       Ret;
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
    //设置回应不需要再新构造头部信息
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
        
    //构造回应包
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
    
    //消息ID 最高位(APP_MSG_ACK_BIT)为1表示是该消息的回应包
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

#if DEFUNC("模块与事件注册")

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

#if DEFUNC("信号量")

#if 0
linux下一切文件皆设备

消息队列这些IPC对象，systemv是让linux内核持久化(保存和管理)
IPC使用同一套函数。查看IPC 使用 ipcs tool
Ipcs 和 ipcs -l
删除ipc消息队列命令：
ipcrm msg msqid

通过路径获取键值key，各个应用app统一的获取key，也可以通过一个种子文件
key=ftok("./tmp/",'a');
export SEEDFILE=”/home/ltf/etc/seedfile”,所有的app读环境变量，getenv(“SEEDFILE”)

信号量和P、V原语由Dijkstra（迪杰斯特拉）提出
信号量
互斥：P、V在同一个进程中
同步：P、V在不同进程中
信号量值含义
S>0：S表示可用资源的个数
S=0：表示无可用资源，无等待进程
S<0：|S|表示等待队列中进程个数

struct semaphore
{
    int value; 
    pointer_PCB queue;
}
若 value 为 1 相当于一个竞争锁

P原语 
P(s)
{
	s.value = s.value--;
	if (s.value < 0)
	{
	该进程状态置为等待状状态
	将该进程的PCB插入相应的等待队列s.queue末尾
	}
}

V原语 
V(s)
{
	s.value = s.value++;
	if (s.value < =0)
	{
	唤醒相应等待队列s.queue中等待的一个进程
	改变其状态为就绪态
	并将其插入就绪队列
	}
}

struct semid_ds {
	struct ipc_perm sem_perm;  /* Ownership and permissions */
	time_t	        sem_otime; /* Last semop time */
	time_t	        sem_ctime; /* Last change time */
	unsigned short  sem_nsems; /* No. of semaphores in set */
};

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

nsems:信号集中信号量的个数
semflg: 由九个权限标志构成，它们的用法和创建文件时使用的mode模式标志是一样的

int semget(key_t key, int nsems, int semflg);

semnum:信号集中信号量的序号
cmd:将要采取的动作（有三个可取值）
最后一个参数根据命令不同而不同

int semctl(int semid, int semnum, int cmd, ...);

sops:指向一个结构数值的指针
sembuf结构体：
struct sembuf {
    short sem_num; 
    short sem_op; 
    short sem_flg; 
};
sem_num是信号量的编号。
sem_op是信号量一次PV操作时加减的数值，一般只会用到两个值，
一个是“-1”，也就是P操作，等待信号量变得可用；
另一个是“+1”，也就是V操作，发出信号量已经变得可用.
sem_flag的两个取值是IPC_NOWAIT或SEM_UNDO（当进程退出时，自动恢复信号量，即自动执行进程中此前的对应p的v操作）

nsops:信号量的个数

int semop(int semid, struct sembuf *sops, unsigned nsops);

#endif

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

int ipc_sem_p(int semid)
{
    int	       ret;
    struct sembuf sp = {0, -1, 0};

    do
    {
        ret =  semop(semid, &sp, 1); //第三个参数是信号量的参数 IPC_NOWAIT and SEM_UNDO
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
        ret = semop(semid, &sp, 1); //第三个参数是信号量的参数
    }while(ret == -1 && errno == EINTR);

    if (ret == -1)
        ERR_EXIT("semctl");
        
    return ret;
}

void ipc_sem_sync_test1()
{
	int     semid;
    key_t   key = ipc_key_get("/etc", 20);

    semid = ipc_sem_create(key);
    if (semid == -1)
    {
        semid = ipc_sem_open(key);
        if (semid == -1)
        {
            return;
        }
    }

    ipc_sem_setval(semid, 1);

    while(1)
    {
        ipc_sem_p(semid);
        printf("this is ctrl\r\n");
        //sleep(1);
        ipc_sem_v(semid);
        sleep(0);
    }

    ipc_sem_del(semid);
}

void ipc_sem_sync_test2()
{
    int     semid;
    key_t   key = ipc_key_get("/etc", 20);

    semid = ipc_sem_create(key);
    if (semid == -1)
    {
        semid = ipc_sem_open(key);
        if (semid == -1)
        {
            return ;
        }
    }

    ipc_sem_setval(semid, 1);

    while(1)
    {
        ipc_sem_p(semid);
        sleep(1);
        ipc_sem_v(semid);
    }

    ipc_sem_del(semid); 
}

#if 0
1.客户端给服务端发报文
2.服务端把接收的服务次数，写共享内存
共享内存中读数据和写数据
3. 服务器初始化时创建共享内存和信号量，服务器的各个子进程通过进程互斥的方式去操作共享内存。
#endif
       
#endif

#if DEFUNC("共享内存")

#if 0
#include <sys/ipc.h>
#include <sys/shm.h>

struct shmid_ds {
	struct ipc_perm shm_perm;    /* Ownership and permissions */
	size_t	     shm_segsz;   /* Size of segment (bytes) */
	time_t	     shm_atime;   /* Last attach time */
	time_t	     shm_dtime;   /* Last detach time */
	time_t	     shm_ctime;   /* Last change time */
	pid_t		 shm_cpid;    /* PID of creator */
	pid_t		 shm_lpid;    /* PID of last shmat(2)/shmdt(2) */
	shmatt_t	 shm_nattch;  /* No. of current attaches */
	...
};

int shmget(key_t key, size_t size, int shmflg);
void *shmat(int shmid, const void *shmaddr, int shmflg);
int shmdt(const void *shmaddr);
int shmctl(int shmid, int cmd, struct shmid_ds *buf);

删除共享内存：
ipcrm shm shmid

解除引用 unmap (shmdt) -> 删除 shmctl
进程退出：
解除引用 unmap
使用了引用计数，不像只有打开关闭状态的api如close，如果引用计数不为1的共享内存被删除，
将会使引用计数减一并key变为0，当所有使用共享内存的进程都退出时，共享内存才能删掉。
如果共享内存被别的进程占用，删除共享内存，则不会立马删除，出现一个现象，
key值变为0即为私有，若其他进程再想从原来key获取对应的共享内存shmid会报错。
只有，连接共享内存的所有进程都死掉，linux才会把共享内存删除。

#endif

/*
key        shmid      owner     perms      bytes      nattch     status      
0x00002234 131073     wbm01     666        68         0    
*/
int ipc_shm_create(key_t key, size_t size)
{  
    //用法和创建文件时使用的mode模式标志是一样
    int shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666); 
    if (shmid == -1)
    {
        //相当于打开文件，文件不存
        perror("shmget");
    }

    return shmid;
}
    
int ipc_shm_open(key_t key)
{
    int shmid = shmget(key, 0, 0);
    if (shmid == -1)
        perror("shmget");
       
    return shmid;
}

void* ipc_shm_map(int shmid)
{
/*
shmaddr为NULL，核心自动选择一个地址
shmaddr不为NULL且shmflg无SHM_RND标记，则以shmaddr为连接地址。
shmaddr不为NULL且shmflg设置了SHM_RND标记，则连接的地址会自动向下调整为SHMLBA的整数倍。公式：shmaddr - (shmaddr % SHMLBA)
shmflg=SHM_RDONLY，表示连接操作用来只读共享内存
*/
    void *p = shmat(shmid, NULL, 0);//SHM_RND和SHM_RDONLY
    if (p == (void *)-1)
    {
        perror("shmget");
        return NULL;
    }

    return p;
}

int ipc_shm_unmap(void *p_addr)
{
    //将共享内存段与当前进程脱离不等于删除共享内存段
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
    //buf:指向一个保存着共享内存的模式状态和访问权限的数据结构
    int ret = shmctl(shmid, IPC_RMID, NULL);
    if (ret < 0)
    {
        perror("shmctl delete");
        ret = errno;
    }

    return ret;
}

int shm_test(void)
{
    key_t 		key;
    int   		shm_id, ret;
    shm_struct  *p_text; 

    key = ipc_key_get("/etc", 20);
    if (key == -1)
        return -1;
        
    shm_id = ipc_shm_create(key, sizeof(shm_struct));
    if (shm_id == -1)
    {
        shm_id = ipc_shm_open(key);
        if (shm_id == -1)
        {
            return -1;
        }
    }

    p_text = ipc_shm_map(shm_id);
    if (!p_text)
        return -1;

    printf("read shm id %d, %s\r\n", shm_id, p_text->buf);

	strcpy(p_text->buf, "this is a shm wirte test");

	printf("write shm id %d, %s\r\n", shm_id, p_text->buf);

	sleep(10);

    ret = ipc_shm_unmap((void*)p_text);
    if (ret)
        return -1;
    
    ret = ipc_shm_del(shm_id);
    if (ret)
        return -1;
        
    return 0;
}

int sem_test(void)
{
    key_t key = ipc_key_get("/etc", 20);

    int semid;

    semid = ipc_sem_create(key);
    if (semid == -1)
    {
        semid = ipc_sem_open(key);
        if (semid == -1)
        {
            return semid;
        }
    }

    ipc_sem_setval(semid, 1);

    while(1)
    {
        ipc_sem_p(semid);
        printf("this is ctrl\r\n");
        //sleep(1);
        ipc_sem_v(semid);
        sleep(0);
    }

    ipc_sem_del(semid); 
}


#endif

#if DEFUNC("消息队列")

#if 0
每个数据块都被认为是有一个类型，接收者进程接收的数据块可以有不同的类型值
每个消息的最大长度是有上限的MSGMAX
每个消息队列的总的字节数是有上限的MSGMNB
系统上消息队列的总数也有一个上限MSGMNI
cat /proc/sys/kernel/msgmax 最大消息长度限制,一般 64k
cat /proc/sys/kernel/msgmnb 消息队列总的字节数,一般 64k
cat /proc/sys/kernel/msgmni 消息队列条目数

对比：
管道：流管道		消息：有边界
	  先进先出	          可以后进入、先出来
		                  消息大小三大限制
						  
IPC对象数据结构
内核为每个IPC对象维护一个数据结构
struct ipc_perm {
	key_t          __key;       /* Key supplied to xxxget(2) */
	uid_t          uid;         /* Effective UID of owner */
	gid_t          gid;         /* Effective GID of owner */
	uid_t          cuid;        /* Effective UID of creator */
	gid_t          cgid;        /* Effective GID of creator */
	unsigned short mode;        /* Permissions */
	unsigned short __seq;       /* Sequence number */
};

struct msqid_ds {
	struct ipc_perm msg_perm;     /* Ownership and permissions */
	time_t	        msg_stime;    /* Time of last msgsnd(2) */
 	time_t	        msg_rtime;    /* Time of last msgrcv(2) */
	time_t	        msg_ctime;    /* Time of last change */
	unsigned long   __msg_cbytes; /* Current number of bytes in
						             queue (nonstandard) */
	msgqnum_t	    msg_qnum;     /* Current number of messages
			                                                in queue */
	msglen_t	    msg_qbytes;   /* Maximum number of bytes
                                                allowed in queue */
	pid_t	        msg_lspid;      /* PID of last msgsnd(2) */
	pid_t           msg_lrpid;      /* PID of last msgrcv(2) */
};

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

用来创建和访问一个消息队列
msgflg:由九个权限标志构成，它们的用法和创建文件时使用的mode模式标志是一样的

0666：消息队列存在则返回，不存在返回ENOENT
0666|IPC_CREATE:消息队列存在使用旧的，消息队列不存在则创建。
IPC_CREATE|IPC_EXCL : 若消息队列存在，返回EEXIST错误，反之，创建。单独使用IPC_EXCL无意义。
msgget(IPC_PRIVATE, 0666);
创建一个key为0的消息队列，IPC_PRIVATE(0)表示每次都创建新的共享内存。这样两个进程就不能共享消息队列了，IPC_PRIVATE表示自己使用,
此时IPC_CREATE和IPC_EXCL将被忽律。
创建的消息队列只在自己的家族中使用，不在没有血缘关系的进程间用,fork的可以。
每一次创建的消息队列的msqid不一样，msqid即使传给其他进程，也不能使用。

msgget(key, 0400|IPC_CREATE) -> msgget(key, 0600) 
权限位：创建的权限只有0400， 如果按照0600打开消息队列，则会失败。
421 rwx

int msgget(key_t key, int msgflg);

消息队列的控制函数
cmd:是将要采取的动作,（有三个可取值）
数据结构：msqid_ds -> ipc_perm， linux通过这两个数据结构管理ipc对象。

int msgctl(int msqid, int cmd, struct msqid_ds *buf);

msgsz:是msgp指向的消息长度，这个长度不含保存消息类型的那个long int长整型
msgflg:控制着当前消息队列满或到达系统上限时将要发生的事情
msgflg=IPC_NOWAIT表示队列满不等待，返回EAGAIN错误。
消息结构在两方面受到制约。首先，它必须小于系统规定的上限值；其次，它必须以一个long int长整数开始，接收者函数将利用这个长整数确定消息的类型
消息结构参考形式如下：
struct msgbuf {
long  mtype;
char mtext[100];
}

int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

msgsz:是msgp指向的消息长度，这个长度不含保存消息类型的那个long int长整型
msgtype:它可以实现接收优先级的简单形式
msgflg:控制着队列中没有相应类型的消息可供接收时将要发生的事

msgtype=0返回队列第一条信息
msgtype>0返回队列第一条类型等于msgtype的消息　
msgtype<0返回队列第一条类型小于等于msgtype绝对值的消息，并且是满足条件的消息类型最小的消息

msgflg=IPC_NOWAIT，队列没有可读消息不等待，返回ENOMSG错误。
msgflg=MSG_NOERROR，消息大小超过msgsz时被截断 
msgtype>0且msgflg=MSG_EXCEPT，接收类型不等于msgtype的第一条消息。

ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);

#endif

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
ipc_mq_recv (
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
ipc_mq_recv_timeout (
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
            ret = ipc_mq_recv(mq_id, type, p_data, size, NO_WAIT, p_copied);
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

    return ipc_mq_recv(mq_id, type, p_data, size, WAIT_FOREVER, p_copied);
}

int
ipc_mq_send (
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

void ipc_mq_send_test()
{
    int         msgq_id;
    ipc_msgbuf  tx_buf;
    int         i = 1;
    key_t       key = ipc_key_get("/etc", 20);

    msgq_id = ipc_mq_create(key);
    if (msgq_id == -1)
    {
        msgq_id = ipc_mq_open(key);
        if (msgq_id == -1)
        {
            return ;
        }
    }
    else
    {
        ipc_mq_size_set(msgq_id, 1024 * 10);
    }

    while(1)
    {
        bzero(&tx_buf, sizeof(ipc_msgbuf));
        tx_buf.mtype = i;
        snprintf(tx_buf.mtext, sizeof(tx_buf.mtext), "this is msg type %ld", tx_buf.mtype);
        ipc_mq_send(msgq_id, &tx_buf, sizeof(ipc_msgbuf)-sizeof(long), WAIT_FOREVER);
        sleep(1);

        ++i;
        i %= 5;
        if (i == 0)
        {
            i = 1;
        }
    }
}

void ipc_mq_recv_test()
{
    int          msgq_id, i = 1;
    ipc_msgbuf   rx_buf;
    unsigned int size;
    key_t        key = ipc_key_get("/etc", 20);

    msgq_id = ipc_mq_create(key);
    if (msgq_id == -1)
    {
        msgq_id = ipc_mq_open(key);
        if (msgq_id == -1)
        {
            return ;
        }
    }
    else
    {
        ipc_mq_size_set(msgq_id, 1024 * 10);
    }

    while(1)
    {
        bzero(&rx_buf, sizeof(ipc_msgbuf));
        rx_buf.mtype = i;

        ipc_mq_recv(msgq_id, rx_buf.mtype, &rx_buf, sizeof(ipc_msgbuf) - sizeof(long), WAIT_FOREVER, &size);
        printf("rec type %ld size %d text %s\r\n", rx_buf.mtype, size, rx_buf.mtext);
        sleep(1);

        ++i;
        i %= 5;
        if (i == 0)
        {
            i = 1;
        }
    }
}

#if 0
1. 
客户端发送服务器消息类型总是1
服务器端回客户端type是对方进程号，客户端接收类型和自己进程号一样的消息
相当于服务器端从消息队列中收消息，然后服务器端分类型回复客户端(通过消息队列)

2. 改进
n个消息队列进行交换有没有产生死锁的可能？
n个客户端向服务端发送消息(本质上是向内核消息队列发送消息)，若消息队列满了服务端回射时会阻塞，造成程序死锁，即使使用非阻塞接口也会阻塞。

#endif

#endif

