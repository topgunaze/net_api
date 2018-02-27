/**************************************************************
 * �ļ�����:  loadExpo.h
 * ��    ��:  tonson tang
 * ��    ��:  2015.07.05
 * �ļ�����:  ϵͳ����ģ�����ӿ�ͷ�ļ�
 * ��    ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
  
**************************************************************/

#ifndef __LOAD_EXPO_H__
#define __LOAD_EXPO_H__


#include "tf_types.h"
#include "tf_flash_images.h"

#define LOAD_MAX_FILE_NAME_LEN		    64
#define LOAD_MAX_PATH_LEN			    128
#define LOAD_USER_NAME_LEN			    32
#define LOAD_PASSWORD_LEN			    32


/* ����Ŀ��MPU�ļ� */
#define LOAD_LOCAL_FILE_MPU               "/tmp/mpuImage"
#define LOAD_LOCAL_FILE_OLT               "/tmp/olt/mpuImage"
#define LOAD_LOCAL_FILE_ONU               "/tmp/onu/mpuImage"


/* ���������ļ� */
#define LOAD_LOCAL_FILE_CFG               "/tmp/cfg.tar"
#define LOAD_LOCAL_FILE_CFG_DIR           "/tmp/cfg/"

/* ��־�ļ� */
#define LOG_TMP_FILE                      "/var/log/messages"

/* ���䶯�� */
#define LOAD_FILE_TRANSFER_NULL         1
#define LOAD_FILE_TRANSFER_GET          2
#define LOAD_FILE_TRANSFER_PUT          3
#define LOAD_FILE_TRANSFER_HALT         4

/* ����״̬ */
#define LOAD_TRANSFER_STATUS_INVALID		0
#define LOAD_TRANSFER_STATUS_IDLE			1
#define LOAD_TRANSFER_STATUS_IN_PROGRESS    2
#define LOAD_TRANSFER_STATUS_SUCCESS        3
#define LOAD_TRANSFER_STATUS_FAIL			4

/* ����Э������ */
#define LOAD_PROTOCOL_FTP				1
#define LOAD_PROTOCOL_TFTP				2

/* MPU�������� */
#define LOAD_MPU_ACTION_NULL            0
#define LOAD_MPU_ACTION_UPGRADE         1
#define LOAD_MPU_ACTION_BACKUP          2

/* MPU������״̬ */
#define LOAD_MPU_SUMMARY_STATS_IDLE         0
#define LOAD_MPU_SUMMARY_STATS_IN_PROGRESS  1
#define LOAD_MPU_SUMMARY_STATS_SUCCESS      2
#define LOAD_MPU_SUMMARY_STATS_FAIL         3
#define LOAD_MPU_SUMMARY_STATS_PARTIAL      4

/* MPU�ļ�����״̬ */
#define LOAD_MPU_STATS_IDLE         0
#define LOAD_MPU_STATS_IN_PROGRESS  1
#define LOAD_MPU_STATS_SUCCESS      2
#define LOAD_MPU_STATS_FAIL         3

/* MPU�ļ���ʧ��ԭ�� */
#define LOAD_FAIL_REASON_NULL           0
#define LOAD_FAIL_REASON_CHK_HRDVER     1
#define LOAD_FAIL_REASON_CHK_LENGTH     2
#define LOAD_FAIL_REASON_CHK_CRC        3
#define LOAD_FAIL_REASON_ERASE_FAIL     4
#define LOAD_FAIL_REASON_READ_FAIL      5
#define LOAD_FAIL_REASON_WRITE_FAIL     6


/* enable structure packing */
#if defined (__GNUC__)
#define	PACKED	__attribute__((packed))
#else
#pragma pack(1)
#define	PACKED
#endif

typedef struct
{
	UINT8	protocolType;					                /* ����Э�����ͣ�ftp(1),tftp(2) */
	UINT32	serverIpAddress;							    /* ip */
	char	ftpUserName[LOAD_USER_NAME_LEN+1];			    /* ftp�û��� */
	char	ftpPassword[LOAD_PASSWORD_LEN+1];			    /* ftp���� */
	char 	remoteFilePath[LOAD_MAX_PATH_LEN+1];	        /* Զ���ļ��� */
	char	localFilePath[LOAD_MAX_PATH_LEN+1];	            /* �����ļ��� */
	char	transmitAction;								    /* ���䶯��:noOperation(1), get(2), put(3), halt(4) */
}PACKED IPC_LOAD_FILE_TRANSMIT_T;


typedef struct
{
    UINT8   transmitStatus;                                 /* ����״̬ idle(1), inProgress(2), success(3), failure(4) */
    UINT8   transmitProgress;                               /* ������� �ٷֱ� 0-100 */
}PACKED IPC_LOAD_TRANSMIT_STATUS_T;


typedef struct
{
	UINT8	operationAction;								/* ����:noOperation(1), upgrade(2), backup(3) */
    char	imageFileName[LOAD_MAX_PATH_LEN+1];				/* �����ļ����� */
}PACKED IPC_LOAD_MPU_ACTION_T;


typedef struct
{
    UINT8   summaryStatus;                                  /* ��״̬:idle(1), inProgress(2), success(3), failure(4), partial(5) */
    UINT32  totalProgress;                                  /* �ܽ����� �ٷֱ� 0-100*/ 
    UINT8   numOfFiles;                                     /* �ļ�����*/
    struct {
        UINT8   fileType;                                   /* �ļ����� boot, kernel, os, config*/
        UINT8   fileVer[4];                                 /* �汾��Ϣ */
        UINT8   buildDate[6];                               /* ����ʱ�� yy-mm-dd hh:min:ss*/
        UINT32  fileSize;                                   /* �ļ���С */
        UINT32  fileCrc32;                                  /* CRC32 */
        UINT8   upgradeStatus;                              /* ����״̬:idle(1), inProgress(2), success(3), failure(4) */
        UINT8   upgradeProgress;                            /* �������� �ٷֱ� 0-100 */
        UINT8   filedReason;                                /* ʧ��ԭ�� */
    }file[TF_FILE_BOX_MAX_FILES];						    
}PACKED IPC_LOAD_MPU_STATUS_T;


/* disable structure packing */
#undef PACKED
#if !defined (__GNUC__)
#pragma pack()
#endif


int loadFileTransmitSet(IPC_LOAD_FILE_TRANSMIT_T *pLoad);
int loadFileTransmitGet(IPC_LOAD_FILE_TRANSMIT_T *pLoad);
int loadFileTransmitStatusGet(IPC_LOAD_TRANSMIT_STATUS_T *pStatus);

int loadMpuActionSet(UINT8 action, char *fileName);
int loadMpuOperationGet(IPC_LOAD_MPU_ACTION_T *pCfg);
int loadMpuOperationStatusGet(IPC_LOAD_MPU_STATUS_T *pSt);



#endif  /* __LOAD_EXPO_H__ */

