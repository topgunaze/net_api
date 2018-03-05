/*****************************************************************************
  Copyright (C), 2014-2024, C-Data Tech. Co., Ltd.
  �ļ���:  ipc_ker.h
  ����:jh.liang
  �汾:1.0.0          
  ����:2014/12
  ����: ���ļ�����IPCģ��������

*****************************************************************************/



#ifndef IPC_KER_H
#define IPC_KER_H

#include "ipc_public.h"
#define IPC_KER_MSG_MAX_LENGTH (IPC_APP_MSG_MAX_LENGTH+sizeof(IPC_HEAD))

typedef struct tag_SND_BLOCK_MSG
{
    struct tag_SND_BLOCK_MSG * next;
    struct sockaddr_un dstaddr;
    USHORT datalen; 
    char pData[0];
}SND_BLOCK_MSG;

/*����IPCģ��ʹ�õ�ȫ����Ϣ,*/
typedef struct 
{
    char                KerSoPath[IPC_SOCKET_PATH_MAX_LENGTH+1];
    struct sockaddr_un  KerSoAddr;
    int                 KerSockFd;
    char 				RecDataBuf[IPC_KER_MSG_MAX_LENGTH]; 
    char 				SndDataBuf[IPC_KER_MSG_MAX_LENGTH]; 
    int 				blockmsgcount;/*��ǰ����������Ϣ��*/
}IPC_KER_CTL;

/*����IPC���������ģ������ݽṹ*/
typedef struct
{
    BOOL       IsReged; 
    char       NameMo[IPC_MODULE_NAME_MAX_LENGTH];
    struct sockaddr_un AckSoAddr;
    struct sockaddr_un CmdSoAddr;
    int sndsockfd;
    int sndwouldblock;/*����������־*/
    SND_BLOCK_MSG *BlockMsgHead; /*�������������뵽���е���Ϣ*/
    SND_BLOCK_MSG *BlockMsgTail;   /*�������������뵽���е���Ϣ*/
    int retry;
}IPC_KER_MO_CTL;

/*���嶩���¼���ģ��*/
typedef struct  IpcEventList
{
    struct  IpcEventList  *pNext;
    struct  IpcEventList   *pPre;
    UCHAR    ucMo;  //ģ���
}IPC_KER_EVENT_LIST;

#endif //IPC_KER_H

