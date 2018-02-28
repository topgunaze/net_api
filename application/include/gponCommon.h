/**************************************************************
 * �ļ�����:  gtfCommon.h
 * ��           ��:  keith.gong
 * ��           ��:  2015.12.16
 * �ļ�����:  
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/


#ifndef __GCOMMON_H__
#define __GCOMMON_H__

#include "sys_debug.h"

typedef enum
{
    ONUCFG_RESULT_INIT,
    ONUCFG_RESULT_SUCCESS,
    ONUCFG_RESULT_FAILED,
    ONUCFG_RESULT_CONFIGING,
}ONUCFG_RESULT_DTE;


typedef enum
{
    GRTN_OK,
    GRTN_ERROR,

    /* ͨ�ô���*/
    GRTN_ERROR_MALLOC,              /* �ڴ治��*/
    GRTN_ERROR_INVALID_PARAM,       /* ��������*/
    GRTN_ERROR_NOT_SUPPORT,
    GRTN_ERROR_TIMEOUT,
    GRTN_CONFIG_REPEAT,
    GRTN_CONFIG_CONFLICT,
    GRTN_CONFIG_SYNC_CONFLICT,
    GRTN_ERROR_OMCI_PROCESS_FAILED,
    
    /* ģ������Դ���*/
    GRTN_DBA_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_DBA_PROFILE_NOT_EXIST,         /* ģ�岻���� 10*/
    GRTN_DBA_PROFILE_BOUND,
    GRTN_DBA_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/
    
    GRTN_SRV_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_SRV_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_SRV_PROFILE_BOUND,
    GRTN_SRV_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/
    GRTN_SRV_PROFILE_USED_AUTOAUTH,
    
    GRTN_LINE_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_LINE_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_LINE_PROFILE_BOUND,             //20
    GRTN_LINE_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/
    GRTN_LINE_PROFILE_USED_AUTOAUTH,

    GRTN_SLA_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_SLA_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_SLA_PROFILE_BOUND,
    GRTN_SLA_PROFILE_NOT_BOUND,
    GRTN_SLA_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/

    GRTN_ALARM_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_ALARM_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_ALARM_PROFILE_BOUND,             //30
    GRTN_ALARM_PROFILE_NOT_BOUND,
    GRTN_ALARM_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/    

    GRTN_OPTICAL_ALARM_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_OPTICAL_ALARM_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_OPTICAL_ALARM_PROFILE_BOUND,
    GRTN_OPTICAL_ALARM_PROFILE_NOT_BOUND,
    GRTN_OPTICAL_ALARM_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/  

    GRTN_CLASSIFICATION_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_CLASSIFICATION_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_CLASSIFICATION_PROFILE_BOUND,             //40
    GRTN_CLASSIFICATION_PROFILE_NOT_BOUND,
    GRTN_CLASSIFICATION_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/

    GRTN_SIPAGENT_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_SIPAGENT_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_SIPAGENT_PROFILE_BOUND,
    GRTN_SIPAGENT_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/

    GRTN_POTS_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_POTS_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_POTS_PROFILE_BOUND,
    GRTN_POTS_PROFILE_TABLE_FULL,        /* ģ�������ﵽ���� 50*/

    GRTN_DIGITMAP_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_DIGITMAP_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_DIGITMAP_PROFILE_BOUND,
    GRTN_DIGITMAP_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/

    GRTN_SIPRIGHT_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_SIPRIGHT_PROFILE_NOT_EXIST,         /* ģ�岻����*/
    GRTN_SIPRIGHT_PROFILE_BOUND,
    GRTN_SIPRIGHT_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/

    GRTN_TRAFFIC_PROFILE_EXIST,             /* ģ���Ѵ���*/
    GRTN_TRAFFIC_PROFILE_NOT_EXIST,         /* ģ�岻���� 60*/
    GRTN_TRAFFIC_PROFILE_BOUND,
    GRTN_TRAFFIC_PROFILE_NOT_BOUND,
    GRTN_TRAFFIC_PROFILE_TABLE_FULL,        /* ģ�������ﵽ����*/

    /* ģ��󶨴���*/
    GRTN_ERROR_ONU_BOUND,
    GRTN_ERROR_ONU_NOT_BOUND,

    /* ONU ״̬*/
    GRTN_ONU_NOT_EXIST,//66
    GRTN_ONU_NOT_ONLINE,
    GRTN_ONU_IDLE,
    GRTN_ONU_BUSY,
    GRTN_ONU_MIB_SYNCED,//70
    GRTN_ONU_MIB_NOT_SYNCED,

    /* ��Ȩ״̬����*/
    GRTN_ERROR_ONURA_ONU_HAS_NOT_REGED,
    GRTN_ERROR_ONURA_ONU_ALREADY_AUTHED,
    GRTN_ERROR_ONURA_AUTH_LIST_TABLE_FULL,
    GRTN_ERROR_ONURA_AUTH_ENTRY_NOT_EXIST,
    GRTN_ERROR_ONURA_AUTH_ENTRY_EXIST,
    GRTN_ERROR_ONURA_AUTH_ENTRY_CONFLLIC,
    GRTN_ERROR_ONURA_AUTH_NO_UNUSED_AUTHID,
    
    /*ע��״̬����*/
    GRTN_ONURA_ERROR_REG_LIST_TABLE_FULL,
    GRTN_ONURA_ERROR_REG_ENTRY_NOT_EXIST,//80
    GRTN_ONURA_ERROR_REG_ENTRY_EXIST, 
    GRTN_ONURA_ERROR_REG_ENTRY_CONFLLIC,
    GRTN_ONURA_ERROR_REG_ENTRY_ID_USED,
    GRTN_ONURA_ERROR_REG_ENTRY_SN_REGED,

    /*ע��״̬����*/
    GRTN_ONURA_ERROR_BLACKLIST_TABLE_FULL,
    GRTN_ONURA_ERROR_BLACKLIST_ENTRY_NOT_EXIST,
    GRTN_ONURA_ERROR_BLACKLIST_ENTRY_CONFLLIC,
    
    /* ����״̬���ô���*/
    GRTN_ONURA_ERROR_SILENCE_FAILED,
    GRTN_ONURA_ERROR_DESILENCE_FAILED,
    GRTN_ONURA_ERROR_OFFLINE_FAILED,//90

    GRTN_NO_ENOUGHT_BW,             /* �������*/

    GRTN_ERROR_ETH_PORT_NOT_EXIST,
    GRTN_ERROR_ETH_PORT_NOT_ADD_TO_THE_VLAN,

	GRTN_ERROR_POTS_PORT_NOT_EXIST,
    GRTN_ERROR_POTS_PORT_USER_EXIST,
    GRTN_ERROR_POTS_PORT_USER_NOT_EXIST,

    GRTN_ONU_IPCONFIG_NOT_CONFIGURED,

    GRTN_ONU_UPGRADE_LOAD_START,
    GRTN_ONU_UPGRADE_LOAD_STOP,
    GRTN_ONU_UPGRADE_LOADING,//100
    GRTN_ONU_UPGRADE_NOT_SELECT,
    GRTN_ONU_UPGRADE_SELECT,
    GRTN_ONU_UPGRADE_DOWNLOAD_FAILED,
    GRTN_ONU_UPGRADE_IMAGE_DOWNLOADING,    
    GRTN_ONU_UPGRADE_OPEN_FAILED,
    GRTN_ONU_UPGRADE_IMAGE_SIZE_ERROR,
    GRTN_ONU_UPGRADE_IMAGE_READED,

    GRTN_ONU_AUTOAUTH_MODE_CONFLICT,
    GRTN_ONU_AUTOAUTH_RULE_ENTRY_EXIST,
    GRTN_ONU_AUTOAUTH_RULE_CONFLICT,//110
    GRTN_ONU_AUTOAUTH_RULE_FULL,
    
    ERTN_ACL_PROFILE_ID_INVALID,
    ERTN_ACL_PROFILE_IN_USE,
    ERTN_ACL_INVALID,
    ERTN_ACL_BIND_FULL,
    ERTN_ACL_BIND_AGAIN,
    ERTN_ACL_BIND_NOT_EXIST,
    ERTN_ACL_BIND_FAILED,

    GRTN_LINE_PROFILE_BOUND_BY_AUTOAUTH,
    GRTN_SRV_PROFILE_BOUND_BY_AUTOAUTH,//120

    PROFILE_RTN_MAX,
}GRTN_CODE;


typedef struct 
{
    const char          *pMsg;
    const char          *pFormat;
}
GERROR_MSG;

typedef struct 
{
    const char          *pMsg;
    const char          *pFormat;
}
GALARM_MSG;

const char *
gerrorcode_print(
                GRTN_CODE ret);

const char *
rc_simple_print(
                unsigned int ret);             

#endif /* __GCOMMON_H__ */


