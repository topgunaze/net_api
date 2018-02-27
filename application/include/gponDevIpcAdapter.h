/**************************************************************
 * �ļ�����:  gtfDevIpcAdapter.h
 * ��           ��:  keith.gong
 * ��           ��:  2015.05.30
 * �ļ�����:  GTF  IPC  �����
 * ��           ��:  V1.00
 * �޸���ʷ:
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/

#ifndef __GTF_DEV_IPC_ADAPTER_H__
#define __GTF_DEV_IPC_ADAPTER_H__

#pragma pack(1)

#define MANUFACTURE_MAX_MODULE_NUM 10 /* ÿ��ģ�鳧�̷�Ĭ��ģ����� */

typedef struct SUPPORT_TRX_S
{
    char *p_manufacturer;
    UINT8 default_type;
    struct 
    {
        char *p_module_name;
        char *p_spec;
        char *p_version;
        UINT8 type;
    }module[MANUFACTURE_MAX_MODULE_NUM];
}SUPPORT_TRX_S;


#pragma pack()

#endif /* __GTF_DEV_IPC_ADAPTER_H__ */

