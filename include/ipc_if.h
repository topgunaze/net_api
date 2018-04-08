/******************************************************************************
Copyright (C), 2014-2024, C-Data Tech. Co., Ltd.
  文 件 名   : ipc_if.h
  作    者   : jh.liang
  版 本 号   : 1.0.0 
  生成日期   : 2015年3月13日
  功能描述   : IPC模块相关变量及宏定义
******************************************************************************/



#ifndef IPC_IF_H
#define IPC_IF_H

#include "ipc_public.h"
#include "pthread.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "errno.h"
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <signal.h>
#include <ucontext.h>
#include <dlfcn.h>
#include <sys/msg.h>



#ifndef DEFUNC
#define DEFUNC(x) 1
#endif

#define ERR_EXIT(str) perror(str);
#define ERR_PRINTF(str) printf();

#define NO_WAIT                   (0)
#define WAIT_FOREVER              (-1)

#define DEBUG

#ifdef DEBUG
#define ipc_debug_printf(format, ...) \
do{ \
  fprintf(stderr, format, ##__VA_ARGS__); \
}while(0)
#else
#define ipc_debug_printf(format, ...) 
#endif

extern pthread_mutex_t gModInitMutex ;
extern int modInit;
extern pthread_mutex_t gSdkInitMutex ;
extern int sdkInited;

/*最大消息长度*/
#define  IPC_APP_MSG_MAX_LENGTH  8092
/*定义最大模块名长度*/
#define  IPC_MODULE_NAME_MAX_LENGTH   (4*32-1)
/*定义socket路径最大长度*/
#define IPC_SOCKET_PATH_MAX_LENGTH    (4*32-1)
#define MAX_ERROR_MSG_LEN 256

/*定义IPC头部，一共占用24个字节*/
typedef struct
{
    USHORT     usMagic;    
    USHORT     Ver:4,      
               RCode:12;   
    USHORT     usSn;       
    USHORT     usDataLen;  
    UCHAR      ucSrcMo;   
    UCHAR      ucDstMo;    
    UCHAR      ucMsgType;  
    UCHAR      SynFlag:2,  
               NewIpcH:2;  
                    
    ULONG      ulRes0;     
    ULONG      ulRes1;
    ULONG      ulRes2;
}IPC_HEAD;

typedef struct 
{
    int       signum;
    char      module_name[32];
    siginfo_t info;
    ucontext_t ucontext;
}module_killed_info;

#define IPC_MSG_MAX_LENGTH (IPC_APP_MSG_MAX_LENGTH+sizeof(IPC_HEAD))

/*定义回调函数格式*/
typedef ULONG (*IPC_MSG_CALLBACK) (char *pMsgIn, ULONG ulMsgLen, UCHAR ucMsgType, UCHAR ucSrcMo);

//定义接口层使用的全局数据信息
typedef struct
{
    ULONG     gInitFlag;
    BOOL      Registered;
    UCHAR     ucSrcMo;   
    char      NameMo[IPC_MODULE_NAME_MAX_LENGTH];
    char      SoAckPath[IPC_SOCKET_PATH_MAX_LENGTH+1]; 
    char      SoCmdPath[IPC_SOCKET_PATH_MAX_LENGTH+1]; 

    struct sockaddr_un AckSoAddr;
    struct sockaddr_un CmdSoAddr;
    struct sockaddr_un IpcSoAddr; 

    int    AckSockFd; 
    int    CmdSockFd; 

    //接收同步响应的缓冲
    char* AckRecDataBuf;
    //接收命令消息的缓冲
    char* CmdRecDataBuf;

    USHORT  usMsgSn;//发送消息的序列序列号
    IPC_MSG_CALLBACK    pIfCallBack;
}IPC_IF_MODULE_INFO;

union semun 
{
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                            (Linux-specific) */
};

typedef struct shm_struct
{
    char buf[1024];
}shm_struct; 

//IPC返回值定义
typedef enum
{
    IPC_SUCCESS=0,    
    IPC_TIMEOUT=1,    
    IPC_REC_INVALID_MSG=2, 
    IPC_INVALID_MODULE=3,
    IPC_NOT_INIT=4, 
    IPC_INVALID_HEAD=5, 
    IPC_NULL_PTR=6,
    IPC_MEM_LACK=7,
    IPC_SOCK_CREATE_FAIL=8, 
    IPC_SRC_MO_NAME_INVALID=9,
    IPC_SOCKET_SENDTO_FAIL=10, 
    IPC_CREATE_THREAD_FAIL=11,
    IPC_OBJ_EXIST=12,
    IPC_INVALID_EID=13,
    IPC_REC_MSG_IPC_HEAD=14,
    IPC_CMD_MODULE_LOOP=15,
    IPC_EVENT_NOT_ENGAGED=16,
    IPC_REG_DUP=17, 
    IPC_SEND_BLOCK=18,
    IPC_MOD_NOT_REG=19, 
    IPC_COMMON_FAIL=1000, 
    
}IPC_IF_RET_CODE;

typedef enum
{
    IPC_MSG_CMD,            
    IPC_MSG_ACK,
    
    IPC_MSG_ECHO,
    
    IPC_MSG_REG_M,          
    IPC_MSG_REG_M_ACK,
    
    IPC_MSG_DISREG_M,      
    IPC_MSG_DISREG_M_ACK,
    
    IPC_MSG_NOTIFY,
    
    IPC_EVENT_ENGAGE,       
    IPC_EVENT_ENGAGE_ACK,
    
    IPC_EVENT_DIS_ENGAGE,  
    IPC_EVENT_DIS_ENGAGE_ACK,
    
    IPC_EVENT_RELEASE,
    
    IPC_MSG_TYPE_NUMS_OF,
}IPC_MSG_TYPE;

#define LOCK(mutex) pthread_mutex_lock(&mutex) 
#define UNLOCK(mutex) pthread_mutex_unlock(&mutex) 

/*申明对外的接口函数*/
ULONG ipc_if_free(void* pToFree);
int  ipc_if_init(void);

ULONG ipc_if_reg_module(UCHAR srcMo,char *pNameMo,IPC_MSG_CALLBACK pCallBack);
ULONG ipc_if_disreg_module();

ULONG ipc_if_engage_event(UCHAR EventId);
ULONG ipc_if_disengage_event(UCHAR EventId);

ULONG ipc_if_send_ack(UCHAR ucDstMo,char * PRecMsg,short RetCode,char *pAckData,USHORT AckDataLen);
ULONG ipc_if_release_event(UCHAR EventId,char *pDataSend,USHORT usLenSend);

UCHAR ipc_if_get_thismoid();

ULONG ipc_if_get_cmd_result(
                unsigned short dstModuleID,
                short          MsgID,
                char           *cmd,
                int            cmdlen,
                char           *rcvbuf,
                int            buflen,
                short          *retCode);

ULONG ipc_if_exe_cmd(unsigned short dstModuleID, short MsgID, char *cmddata, int cmdlen, short *retCode);

int ipc_key_get(char *path, int sub_key);
int ipc_sem_create(key_t key);
int ipc_sem_open(key_t key);
int ipc_sem_setval(int semid, int val);
int ipc_sem_getval(int semid, int val);
int ipc_sem_del(int semid);
int ipc_sem_p(int semid);
int ipc_sem_v(int semid);
int ipc_shm_create(key_t key, size_t size);
int ipc_shm_open(key_t key);
void* ipc_shm_map(int shmid);
int ipc_shm_unmap(void *p_addr);
int ipc_shm_del(int shmid);
int shm_test(void);

typedef enum
{
    MQ_CTRL_RX_SYN_REQ_MSG,
    MQ_CTRL_RX_SYN_ACK_MSG,
    MQ_CTRL_RX_ASYN_REQ_MSG,
    MQ_CTRL_RX_ASYN_ACK_MSG,
    MQ_CTRL_TX_MSG,

    MQ_NODE_RX_SYN_REQ_MSG,
    MQ_NODE_RX_SYN_ACK_MSG,
    MQ_NODE_RX_ASYN_REQ_MSG,
    MQ_NODE_RX_ASYN_ACK_MSG,
    MQ_NODE_TX_MSG,

    MQ_CTRL_TX_NODE_RX_MSG,
    MQ_CTRL_RX_NODE_TX_MSG,

    MQ_MAX_NUM_OF,
}MQ_SUB_KEY_E;

int ipc_mq_create (key_t key);
int ipc_mq_open(key_t key);
int ipc_mq_size_set(int msg_id, unsigned int size);
int ipc_mq_out (
            int             mq_id, 
            long            type, 
            void            *p_data, 
            unsigned int    size, 
            int             block_flag,
            unsigned int    *p_copied);
int ipc_mq_out_timeout (
                int             mq_id, 
                long            type, 
                void            *p_data, 
                unsigned int    size, 
                unsigned int    timeout, /* ms  */
                unsigned int    *p_copied);
int ipc_mq_in (
            int           mq_id, 
            void          *p_data, 
            unsigned int  size, 
            int           block_flag);

#ifdef DEBUG
      #define debuginfo(fmt, args...)	 printf(fmt, ## args)
#else
      #define debuginfo(fmt, args...)
#endif

#endif  //IPC_IF_H

