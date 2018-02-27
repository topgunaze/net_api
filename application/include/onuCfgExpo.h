/**************************************************************
 * �ļ�����:  onuCfgExpo.h
 * ��           ��:  keith.gong
 * ��           ��:  2015.05.22
 * �ļ�����:  ONU ����ģ��ϵͳ��ͷ�ļ�
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/

#ifndef __ONU_CFG_EXPO_H__
#define __ONU_CFG_EXPO_H__


typedef enum
{
    ONUCFG_NO_ERROR,

    /* profile */
    ONUCFG_ERROR_PROFILE_ID_OUT_OF_RANGE,
    ONUCFG_ERROR_PROFILE_TABLE_FULL,
    ONUCFG_ERROR_PROFILE_EXIST,
    ONUCFG_ERROR_PROFILE_NOT_EXIST,
    ONUCFG_ERROR_PROFILE_NOT_FOUND,
    ONUCFG_ERROR_PROFILE_NAME_EXIST,
    ONUCFG_ERROR_PROFILE_MALLOC_FAIL,
    ONUCFG_ERROR_PROFILE_IN_BIND_STATE,
    
    ONUCFG_ERROR_INVALID_ETH_PORT_NUM,
    ONUCFG_ERROR_INVALID_POTS_PORT_NUM,
    ONUCFG_ERROR_INVALID_MOCA_PORT_NUM,
    ONUCFG_ERROR_INVALID_CATV_PORT_NUM,
    
    ONUCFG_ERROR_TF_ID_OUT_OF_RANGE,
    ONUCFG_ERROR_ONU_ID_OUT_OF_RANGE,    
    ONUCFG_ERROR_ETH_ID_OUT_OF_RANGE,
    
    ONUCFG_ERROR_PARAM_ERROR,
}ONUCFG_ERROR_DTE;

typedef enum
{
    ONU_CFG_RESULT_INIT,
    ONU_CFG_RESULT_SUCCESS,
    ONU_CFG_RESULT_FAILED,
}ONU_CFG_RESULT_DTE;


/**************************************************************
 * ��������: Cdt_Gtf_OnuCfg_SrvProfileBindSingleAdd
 * ��           ��: keith.gong
 * ��           ��: 2015��04��22��
 *
 * ��������: 
                            ��Profile
 * �������: 
 * �������: 
 * ������ֵ: 
 * ��           ��: 
**************************************************************/
RTN_STATUS 
Cdt_Gtf_OnuCfg_SrvProfileBindSingleAdd(
                UINT32 profile_id, 
                UINT32 tfId, 
                UINT32 onuId);

RTN_STATUS 
Cdt_Gtf_OnuCfg_SrvProfileBindSingleDel(
                UINT32 profile_id, 
                UINT32 tfId, 
                UINT32 onuId);


#endif /* __ONU_CFG_EXPO_H__ */

