/*****************************************************************************
  Copyright (C), 2014-2024, C-Data Tech. Co., Ltd.
  �ļ���:  ipc_private.h
  ����:jh.liang
  �汾:1.0.0          
  ����:2014/12
  ����: ���ļ�����IPCģ��������

*****************************************************************************/


#ifndef IPC_PRIVATE_H
#define IPC_PRIVATE_H

#include "ipc_public.h"

//���嵱ǰ��IPCͷ���汾��
#define IPC_HEAD_CUR_VER    1;
//����IPCͷ����ħ����
#define IPC_HEAD_MAGIC    0xC0D1

//��������̵߳�ȱʡ��ջ��С
#define IPC_REC_CMD_DEFAULT_STACKSIZE 32 * 1024
/*
 * ���Ȳ���
 */
#define SCHED_OTHER		0
#define SCHED_FIFO		1
#define SCHED_RR		2

//����IPC����·��
#define IPC_MAIN_REC_SO_PATH    "/tmp/IpcKer"

//����socket·����ʼ�ַ���
#define  IPC_IF_CMD_SO_PATH_PRE  "/tmp/IpcCmd"
#define  IPC_IF_ACK_SO_PATH_PRE  "/tmp/IpcAck"

#define IPC_APP_MEM_LEN_TO_ACT_LEN(l)    (sizeof(IPC_HEAD)+l)
#define IPC_APP_MEM_OFFSET(addr)  (((char*)addr)+sizeof(IPC_HEAD))
#define IPC_APP_MEM_TO_IPC(addr)  (((char*)addr)-sizeof(IPC_HEAD))

#define    IPC_SYN_SELECT_TIMEOUT_SEC    60    


typedef enum
{
    IPC_SYN_MSG,  //ͬ����Ϣ
    IPC_ASYN_MSG //�첽��Ϣ
}IPC_ASYN_FLAG;

//����IPC�ڲ����ظ�IPC�ӿڵ�ͬ��������
typedef enum
{
    IPC_REG_M_ACK, 
    IPC_REG_M_NAK,     
    IPC_DISREG_M_ACK,   
    IPC_DISREG_M_NAK,  
    
    IPC_ENGAGE_EVENT_ACK, 
    IPC_ENGAGE_EVENT_NAK, 

    IPC_DISENGAGE_EVENT_ACK, 
    IPC_DISENGAGE_EVENT_NAK, 
    
}IPC_TO_IF_ACK_NAK;

//����ע����Ϣ��ʽ
typedef struct
{
    UCHAR    ucSrcMo;   
    UCHAR    ucRes0;    
    USHORT   usRes1;
    ULONG     ulRes2;
    char         NameMo[IPC_MODULE_NAME_MAX_LENGTH+1];
    char        SoAckPath[IPC_SOCKET_PATH_MAX_LENGTH+1];
    char        SoCmdPath[IPC_SOCKET_PATH_MAX_LENGTH+1];
}IPC_REG_MODULE_MSG_INFO;

//����ע��ģ�����Ϣ��ʽ
typedef struct
{
    UCHAR    ucSrcMo;   
    UCHAR    ucRes0;    
    USHORT   usRes1;
    ULONG     ulRes2;
}IPC_DIS_REG_MODULE_MSG_INFO;

//����/ȡ�������¼�����Ϣ��ʽ
typedef struct
{
    UCHAR    ucSrcMo;   
    UCHAR    ucEventId;    
    USHORT   usRes0;
    ULONG     ulRes1;
}IPC_EVENT_CMD_MSG_INFO;

//����ͨ����Ӧ��Ϣ��ʽ
typedef struct
{
    UCHAR    ucSrcMo;   
    UCHAR    ucRetCode; 
    USHORT  usRes0;
    ULONG    ulRes1;
}IPC_COMMON_REG_ACK_INFO;

#endif //IPC_PRIVATE_H

