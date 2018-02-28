/**************************************************************
* 文件名称: 
* 文件描述:
* 版           本: 
* 修改历史:
*     <修改者>     <时间>      <版本 >     <描述>

**************************************************************/

#include "adaptor_errno.h"

#if DEFUNC("全局错误码转换接口")

unsigned int 
rc2info_convert(unsigned int rc_val, rc_info *p_rc_info)
{
    if (!p_rc_info)
    {
        return MDW_NET_RC_PARAM_NULL;
    }
    
    if (rc_val>=RETURNCODE_MAX_VALUE)
    {
        return MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }
    
    p_rc_info->obj_id = (rc_val/RETURNCODE_BASE)/RETURNCODE_BASE;

    switch(p_rc_info->obj_id)
    {
        case OBJ_DRV_SDK:case OBJ_MDW_LIBEV:case OBJ_MDW_SQL:
        case OBJ_MDW_NET:
        case OBJ_APP_ALARM:
            {
                //暂未使用 
            }
            break;

        case OBJ_DRV_ZK:
            {
                p_rc_info->sub_obj_id.olt_id = (rc_val/RETURNCODE_BASE)%RETURNCODE_BASE;
            }
            break;

        case OBJ_DRV_FK:
            {
                p_rc_info->sub_obj_id.onu_id = (rc_val/RETURNCODE_BASE)%RETURNCODE_BASE;
            }
            break;

        default:
            printf("obj id: %d not in range!!!\n", p_rc_info->obj_id);
            return MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    p_rc_info->err_id = rc_val%RETURNCODE_BASE;
  
    return 0;
}

unsigned int 
info2rc_convert(unsigned int *p_rc_val, rc_info *p_rc_info)
{
    if (!p_rc_val || !p_rc_info)
    {
        return MDW_NET_RC_PARAM_NULL;
    }

    if (p_rc_info->obj_id >= RETURNCODE_BASE || p_rc_info->err_id >= RETURNCODE_BASE || 
        *(unsigned int*)&p_rc_info->sub_obj_id >= RETURNCODE_BASE)
    {
        return MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    *p_rc_val = p_rc_info->err_id;
    *p_rc_val += p_rc_info->obj_id*RETURNCODE_BASE*RETURNCODE_BASE;
    
    switch(p_rc_info->obj_id)
    {
        case OBJ_DRV_SDK:case OBJ_MDW_LIBEV:case OBJ_MDW_SQL:
        case OBJ_MDW_NET:
            {
                //暂未使用 
            }
            break;
        
        case OBJ_APP_ZK:
           {
               *p_rc_val += p_rc_info->sub_obj_id.olt_id * RETURNCODE_BASE;
           }
           break;    
        
        case OBJ_APP_ALARM:
           {
               *p_rc_val += p_rc_info->sub_obj_id.profile_id * RETURNCODE_BASE;
           }
           break;
        
           default:
               printf("obj id: %d not in range!!!\n", p_rc_info->obj_id);
               return MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    return 0;
}

#endif

