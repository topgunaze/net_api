/**************************************************************
 * 文件名称:  
 * 作           者:  
 * 日           期:  
 * 文件描述:  
 * 版           本:  V1.00
 * 修改历史:  
 *     <修改者>   <时间>    <版本 >   <描述>
**************************************************************/

#ifndef __NET_H__
#define __NET_H__

#include "adaptor_net_if.h"
#include "adaptor_types.h"

#define CTRL_STATE_FD_TIMEOUT    4
#define CTRL_STATE_HB_LOSE_CNT   5

//#pragma pack(1)

typedef enum
{
    CTRL_DEVICE_STATE_INIT        = 0,
    CTRL_NET_WORK_SYNCHRONOUS     = 1,//业务面开工

    CTRL_NET_STATE_NUM_OF
}CTRL_NET_STATE;

//网络映射
typedef struct
{
    NET_MUTUX mutex;
    NET_PARA  net_param;       
    UINT8     slot_id;
    UINT8     is_used;
    UINT32    timeout_stamp;
}CTRL_NET_MAP;

typedef struct
{
    UINT8   valid[NET_CFG_ITEM_NUM/8];   //表项同步有效
    
}CTRL_CFG_STATE;

typedef struct
{
    NET_MUTUX          mutex;                           //同步状态锁
    NET_MUTUX          sync_lock;                       //同步锁       
    CTRL_CFG_STATE  gbl_cfg[NET_CFG_TYPE_NUM_OF];//表项有效

}CTRL_GBL_CFG_STATE;

#define CFG_SYNC_DATA_TOTAL_LEN    (sizeof(CTRL_CFG_STATE)* NET_CFG_TYPE_NUM_OF)
#define CFG_SYNC_DATA_LEN          sizeof(CTRL_CFG_STATE)

//TF卡槽位信息
typedef struct
{
    NET_MUTUX        mutex;
    CTRL_NET_STATE   state;
    UINT8            map_id;
    UINT8            online_num[SYS_MAX_EXIST_PORT_NUM];
    UINT8            hb_timeout_cnt;                     //心跳超时计数器
    CTRL_CFG_STATE   cfg_sync[NET_CFG_TYPE_NUM_OF];  //全局配置表项状态
}CTRL_STATE;

//#pragma pack()

UINT32 
ctrl_net_mempool_init();

UINT32 
ctrl_net_task_init(void);

UINT32 
ctrl_net_ev_init(void);

UINT32 
ctrl_net_asyn_operation( 
                                IN const NET_MSG_TYPE   msg_type,
                                IN const UINT64             vif_info, 
                                IN const NET_MSG_CMD    cmd_info, 
                                IN const void               *pvar, 
                                IN const UINT32             len);

UINT32 
ctrl_net_syn_operation( 
                                IN NET_MSG_TYPE msg_type,
                                IN UINT64           vif_info, 
                                IN NET_MSG_CMD  cmd_info, 
                                IN const void       *pvar, 
                                IN const UINT32     pvar_len,
                                OUT void            *param,
                                IN  const UINT32    param_len,
                                OUT UINT32          *state);                                    

void
ctrl_net_state_init(void);

void
ctrl_net_map_init(void);

uint32_t
ctrl_net_state_working(uint8_t slot_id);

uint8_t
ctrl_net_port_onu_online_get(
                            const uint8_t         slot_id,
                            const uint8_t         port_id);
                           
unsigned int
ctrl_net_cfg_is_valid(
                            const uint8_t   type_idx,
                            const uint16_t  item_idx);
                            
unsigned int
ctrl_net_cfg_valid_clear(
                            const uint8_t   type_idx,
                            const uint16_t  item_idx);
                            
unsigned int
ctrl_net_cfg_valid_set(
                            const uint8_t   type_idx,
                            const uint16_t  item_idx);
                            
void
ctrl_net_cfg_init(void);

unsigned int
ctrl_net_cfg_sync_trylock(void);

void
ctrl_net_cfg_sync_lock(void);

void
ctrl_net_cfg_sync_unlock(void);

unsigned int
ctrl_net_cfg_sync_valid_set(
                            const uint8_t   slot_id,
                            const uint8_t   type_id,
                            const uint16_t  item_id);
unsigned int
ctrl_net_cfg_sync_valid_clear(
                            const uint8_t   slot_id,
                            const uint8_t   type_id,
                            const uint16_t  item_id);

void
ctrl_net_cfg_sync_invalid_set(const uint8_t slot_id);

unsigned int 
ctrl_net_cfg_is_consistent(const uint8_t slot_idx);

unsigned int
ctrl_net_cfg_type_is_consistent(
                                        const uint8_t slot_idx, 
                                        const uint8_t   type_idx);

unsigned int
ctrl_net_cfg_item_group_is_consistent(
                                                const uint8_t slot_idx, 
                                                const uint8_t type_idx, 
                                                const uint8_t group_idx);
                                                
#endif
