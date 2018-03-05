/**************************************************************
 * �ļ�����:  
 * ��           ��: 
 * ��           ��:  
 * �ļ�����:  
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
 
**************************************************************/

#include "ev.h"
#include "ctrl_net.h"
#include <sys/sysinfo.h>
#include "bit_map.h"

static void
ctrl_net_active_disconnect_clear(EV_P_ struct ev_io *watcher);
  
//������Ϣ(�������������Է����澯�Է�)�첽����ӿ�
static UINT32 
ctrl_net_asyn_req_work_process(NET_MSG *pmsg);

static UINT32 
ctrl_net_asyn_req_hb_process(NET_MSG *pmsg);

static UINT32 
ctrl_net_asyn_req_auto_process(NET_MSG *pmsg);

static UINT32 
ctrl_net_asyn_req_alarm_process(NET_MSG *pmsg);

//������Ӧ��Ϣ(onu/olt/nw���������û�ȡ)���첽����Ԥ���ӿ�
static UINT32 
ctrl_net_asyn_ack_onu_set_process(NET_MSG *pmsg);

static UINT32 
ctrl_net_asyn_ack_onu_get_process(NET_MSG *pmsg);

static UINT32 
ctrl_net_asyn_ack_olt_set_process(NET_MSG *pmsg);

static UINT32 
ctrl_net_asyn_ack_olt_get_process(NET_MSG *pmsg);

static UINT32 
ctrl_net_asyn_ack_nw_set_process(NET_MSG *pmsg);

static UINT32 
ctrl_net_asyn_ack_nw_get_process(NET_MSG *pmsg);

static UINT32 
ctrl_net_syn_req_drv_pull_process(NET_MSG *pmsg);

//ctrl ��Ϣ������Ϣ�ڴ��
static NET_BLK_POOL g_ctrl_tr_mq_mp;
//���ư����Ϣ�ڴ��
static NET_BLK_POOL g_ctrl_net_msg_mp;
//���ư�libev �������ڴ��
static NET_BLK_POOL g_ctrl_watcher_mp;
//���ư�ͬ����Ϣ�����ڴ��
static NET_BLK_POOL g_ctrl_conn_mp;
//���ư�ͬ����Ϣ�ڴ��
static NET_BLK_POOL g_ctrl_conn_msg_mp;

//���ư巢����Ϣ����
static NET_ZC_MQ g_ctrl_tx_mq[NET_MANAGE_MAX_SLOT];
//���ư����ͬ��������Ϣ����
static NET_ZC_MQ g_ctrl_rx_syn_req_mq[NET_MANAGE_MAX_SLOT];
//���ư����ͬ����Ӧ��Ϣ����
static NET_ZC_MQ g_ctrl_rx_syn_ack_mq[NET_MANAGE_MAX_SLOT];
//���ư�����첽������Ϣ����
static NET_ZC_MQ g_ctrl_rx_asyn_req_mq;
//���ư�����첽��Ӧ��Ϣ����
static NET_ZC_MQ g_ctrl_rx_asyn_ack_mq;

//����ӳ��
static CTRL_NET_MAP     g_ctrl_net_map[NET_MANAGE_MAX_SLOT];
//���ư�״̬
static CTRL_STATE       g_ctrl_state[NET_MANAGE_MAX_SLOT];
//���ư�ȫ�ֱ�������״̬
CTRL_GBL_CFG_STATE      g_ctrl_gbl_cfg_state;
//conn ������
static NET_MUTUX        g_ctrl_conn_lock;
//���ư���ҵ���֮���ͬ����Ϣ����
static NET_CONN         *g_ctrl_conn[NET_MANAGE_MAX_SLOT];

//�ú�ֻ�ڱ��ļ�ʹ��
#define SLOT2NET_IO(slot_id)        (g_ctrl_net_map[g_ctrl_state[slot_id].map_id].net_param)
#define SLOT2NET_FD(slot_id)        (g_ctrl_net_map[g_ctrl_state[slot_id].map_id].net_param.watcher->fd)

#if DEFUNC("���ư�ȫ������ͬ���ӿ�")

//�����ַӳ���ʼ��
void
ctrl_net_cfg_init(void)
{ 
    bzero(&g_ctrl_gbl_cfg_state, sizeof(CTRL_GBL_CFG_STATE));
    net_mutex_create(&g_ctrl_gbl_cfg_state.mutex);
    net_mutex_create(&g_ctrl_gbl_cfg_state.sync_lock);

    return ;
}

//ȫ����������Ч��ʾ����
unsigned int
ctrl_net_cfg_valid_set(
                const uint8_t   type_idx,
                const uint16_t  item_idx)
{  
    if (type_idx >= NET_CFG_TYPE_NUM_OF || item_idx >= NET_CFG_ITEM_NUM)
    {
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
    }
    
    net_mutex_lock(&g_ctrl_gbl_cfg_state.mutex);
    bitListSet(g_ctrl_gbl_cfg_state.gbl_cfg[type_idx].valid, (uint32_t)item_idx);
    net_mutex_unlock(&g_ctrl_gbl_cfg_state.mutex);

    return 0;
}

//ȫ����������Ч��ʾ���
unsigned int
ctrl_net_cfg_valid_clear(
                            const uint8_t   type_idx,
                            const uint16_t  item_idx)
{  
    if (type_idx >= NET_CFG_TYPE_NUM_OF || item_idx >= NET_CFG_ITEM_NUM)
    {
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
    }
    
    net_mutex_lock(&g_ctrl_gbl_cfg_state.mutex);
    bitListClr(g_ctrl_gbl_cfg_state.gbl_cfg[type_idx].valid, (uint32_t)item_idx);
    net_mutex_unlock(&g_ctrl_gbl_cfg_state.mutex);

    return 0;
}

//ȫ���������Ƿ���Ч
unsigned int
ctrl_net_cfg_is_valid(
                const uint8_t   type_idx,
                const uint16_t  item_idx)
{  
    if (type_idx >= NET_CFG_TYPE_NUM_OF || item_idx >= NET_CFG_ITEM_NUM)
    {
        return 0;
    }

    return bitListTst(g_ctrl_gbl_cfg_state.gbl_cfg[type_idx].valid, (uint32_t)item_idx);
}

//ȫ�����ð��ͬ��״̬
void
ctrl_net_cfg_sync_lock(void)
{   
    net_mutex_lock(&g_ctrl_gbl_cfg_state.sync_lock);

    return ;     
}

//ȫ���������ڰ��ͬ��
uint32_t
ctrl_net_cfg_sync_trylock(void)
{    
   return net_mutex_trylock(&g_ctrl_gbl_cfg_state.sync_lock);   
}

//ȫ�����ð�ǰ��ͬ��״̬
void
ctrl_net_cfg_sync_unlock(void)
{   
    net_mutex_unlock(&g_ctrl_gbl_cfg_state.sync_lock);

    return ;     
}
    
#endif

#if DEFUNC("���ư�����ӳ��ӿ�")

//�����ַӳ���ʼ��
void
ctrl_net_map_init(void)
{
    int idx;
    
    for (idx = 0; idx < NET_MANAGE_MAX_SLOT; idx++)
    {
        bzero(&g_ctrl_net_map[idx], sizeof(CTRL_NET_MAP));
        net_mutex_create(&g_ctrl_net_map[idx].mutex);
        g_ctrl_net_map[idx].slot_id = NET_COM_PARAM_INVALID;
    }

    return ;
}

//�����ַӳ��ռ��״̬����
static unsigned int
ctrl_net_map_used_set(
                            const uint8_t idx,
                            const uint8_t is_used)
{  
    if (idx >= NET_MANAGE_MAX_SLOT)
    {
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
    }
    
    net_mutex_lock(&g_ctrl_net_map[idx].mutex);
    g_ctrl_net_map[idx].is_used = is_used;
    net_mutex_unlock(&g_ctrl_net_map[idx].mutex);

    return 0;
}

//�����ַӳ��slot
static unsigned int
ctrl_net_map_slot_set(
                            const uint8_t idx,
                            const uint8_t slot_id)
{  
    if (idx >= NET_MANAGE_MAX_SLOT)
    {
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
    }

    net_mutex_lock(&g_ctrl_net_map[idx].mutex);
    g_ctrl_net_map[idx].slot_id = slot_id;
    net_mutex_unlock(&g_ctrl_net_map[idx].mutex);

    return 0;
}

//�����ַӳ��ʱ�������
static unsigned int
ctrl_net_map_time_stamp_set(
                            const uint8_t  idx,
                            const uint32_t time)
{  
    if (idx >= NET_MANAGE_MAX_SLOT)
    {
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
    }

    net_mutex_lock(&g_ctrl_net_map[idx].mutex);
    g_ctrl_net_map[idx].timeout_stamp = time;
    net_mutex_unlock(&g_ctrl_net_map[idx].mutex);

    return 0;
}

//�����ַӳ���ַ����
static unsigned int
ctrl_net_map_net_param_set(
                            uint8_t             idx,
                            const NET_PARA  *p_param)
{  
    if (idx >= NET_MANAGE_MAX_SLOT)
    {
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
    }

    net_mutex_lock(&g_ctrl_net_map[idx].mutex);
    memcpy(&g_ctrl_net_map[idx].net_param, p_param, sizeof(NET_PARA));
    net_mutex_unlock(&g_ctrl_net_map[idx].mutex);

    return 0;
}

//�����ַӳ����м�¼��ȡ
static uint32_t
ctrl_net_map_free_id_get(uint32_t *p_idx)
{
    int idx;

    if (!p_idx)
    {
        return APP_ZK_RC_PARAM_NULL;
    }
    
    //��¼�������fd
    for (idx = 0; idx < NET_MANAGE_MAX_SLOT; idx++)
    {
        if (g_ctrl_net_map[idx].is_used)
            continue;

        break;
    }

    if (idx >= NET_MANAGE_MAX_SLOT)
    {
        return APP_ZK_RC_NET_MAP_IS_FULL;
    }

    *p_idx = idx;
    ctrl_net_map_used_set(idx, NET_COM_PARAM_IS_SET);

    return 0;
}

//�����ַӳ���¼����
static unsigned int
ctrl_net_map_net_param_init(
                    struct ev_loop     *loop,
                    struct ev_io       *watcher,
                    uint8_t            map_id, 
                    struct sockaddr_in *p_addr)
{
    struct sysinfo sys_info;
    NET_PARA       net_param;

    if (map_id >= NET_MANAGE_MAX_SLOT || !p_addr)
    {
        return APP_ZK_RC_PARAM_INVALID;
    }

    net_param.loop    = loop;
    net_param.watcher = watcher;
    memcpy(&net_param.remote_addr, p_addr, sizeof(struct sockaddr_in));
    
    ctrl_net_map_net_param_set(map_id, &net_param);
    
    sysinfo(&sys_info);
    
    ctrl_net_map_time_stamp_set(map_id, sys_info.uptime);
    
    printf("accept time out statmp %ld\r\n", sys_info.uptime + CTRL_STATE_FD_TIMEOUT);

    return 0;
}

#endif

#if DEFUNC("���ư幤��״̬�ӿ�")

//���ư�������״̬��ʼ��

void
ctrl_net_state_init(void)
{
    int slot_id;
    
    for (slot_id = 0; slot_id < NET_MANAGE_MAX_SLOT; ++slot_id)
    {
        bzero(&g_ctrl_state[slot_id], sizeof(CTRL_NET_STATE));
        g_ctrl_state[slot_id].map_id = NET_COM_PARAM_INVALID; 
        net_mutex_create(&g_ctrl_state[slot_id].mutex);
    }

    return ;
}

//���ư�����·״̬����
static unsigned int
ctrl_net_state_set(
                    const uint8_t   slot_id,
                    const uint32_t  state)
{  
    if (slot_id >= NET_MANAGE_MAX_SLOT)
    {
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
    }

    net_mutex_lock(&g_ctrl_state[slot_id].mutex);
    g_ctrl_state[slot_id].state |= state;
    net_mutex_unlock(&g_ctrl_state[slot_id].mutex);

    return 0;
}

//���ư�����·״̬���
static unsigned int
ctrl_net_state_clear(
                            const uint8_t    slot_id,
                            const uint32_t   state)
{     
    if (slot_id >= NET_MANAGE_MAX_SLOT)
    {
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
    }

    net_mutex_lock(&g_ctrl_state[slot_id].mutex);
    g_ctrl_state[slot_id].state &= ~state;
    net_mutex_unlock(&g_ctrl_state[slot_id].mutex);

    return 0;
}

//���ư�����·״̬�ѿ���
uint32_t
ctrl_net_state_working(const uint8_t slot_id)
{   
    uint32_t state;
    
    if (slot_id >= NET_MANAGE_MAX_SLOT)
        return 0;
        
   state = g_ctrl_state[slot_id].state & CTRL_NET_WORK_SYNCHRONOUS;
   
   return (state == CTRL_NET_WORK_SYNCHRONOUS) ?  1 : 0;     
}
/*
//���ư�����·tf������onu����
static unsigned int
ctrl_net_port_onu_online_add(const uint8_t slot_id, const uint8_t port_id)
{  
    if (slot_id >= NET_MANAGE_MAX_SLOT || port_id >= SYS_MAX_EXIST_PORT_NUM)
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
        
    net_mutex_lock(&g_ctrl_state[slot_id].mutex);
    ++g_ctrl_state[slot_id].online_num[port_id];
    net_mutex_unlock(&g_ctrl_state[slot_id].mutex);

    return 0;
}
*/
//���ư�����·tf������onu��Ŀ��ȡ
uint8_t
ctrl_net_port_onu_online_get(
                            const uint8_t slot_id,
                            const uint8_t port_id)
{    
    if (slot_id >= NET_MANAGE_MAX_SLOT || port_id >= SYS_MAX_EXIST_PORT_NUM)
        return 0;
        
    return g_ctrl_state[slot_id].online_num[port_id];
}
/*
//���ư�����·tf������onu��Ŀ�ݼ�
static unsigned int
ctrl_net_port_onu_online_dec(const uint8_t slot_id, const uint8_t port_id)
{
    if (slot_id >= NET_MANAGE_MAX_SLOT || port_id >= SYS_MAX_EXIST_PORT_NUM)
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;

    net_mutex_lock(&g_ctrl_state[slot_id].mutex);
    --g_ctrl_state[slot_id].online_num[port_id];
    net_mutex_unlock(&g_ctrl_state[slot_id].mutex);
    
    return 0;
}
*/
//���ư�����·����ͳ������
static unsigned int
ctrl_net_hb_cnt_set(
                            const uint8_t slot_id,
                            const uint8_t cnt)
{  
    if (slot_id >= NET_MANAGE_MAX_SLOT)
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;

    net_mutex_lock(&g_ctrl_state[slot_id].mutex);
    g_ctrl_state[slot_id].hb_timeout_cnt = cnt;
    net_mutex_unlock(&g_ctrl_state[slot_id].mutex);

    return 0;
}

//���ư�����·����ͳ�Ƶ���
static unsigned int
ctrl_net_hb_cnt_add(const uint8_t slot_id)
{  
    if (slot_id >= NET_MANAGE_MAX_SLOT)
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;

    net_mutex_lock(&g_ctrl_state[slot_id].mutex);
    ++g_ctrl_state[slot_id].hb_timeout_cnt;
    net_mutex_unlock(&g_ctrl_state[slot_id].mutex);

    return 0;
}

//���ư�����·������ʱͳ�ƻ�ȡ
static uint8_t
ctrl_net_hb_cnt_get(const uint8_t slot_id)
{    
    if (slot_id >= NET_MANAGE_MAX_SLOT)
        return NET_COM_PARAM_INVALID;

    return g_ctrl_state[slot_id].hb_timeout_cnt;
}

//���ư�������ӳ������
static unsigned int
ctrl_net_map_id_set(const uint8_t slot_id, const uint8_t map_id)
{  
    if (slot_id >= NET_MANAGE_MAX_SLOT)
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
        
    net_mutex_lock(&g_ctrl_state[slot_id].mutex);
    g_ctrl_state[slot_id].map_id = map_id;
    net_mutex_unlock(&g_ctrl_state[slot_id].mutex);

    return 0;
}

//���ư���ȫ��������ɱ�ʾ����
unsigned int
ctrl_net_cfg_sync_valid_set(
                            const uint8_t   slot_id,
                            const uint8_t   type_id,
                            const uint16_t  item_id)
{  
    if (slot_id >= NET_MANAGE_MAX_SLOT || type_id >= NET_CFG_TYPE_NUM_OF
        || item_id >= NET_CFG_ITEM_NUM)
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;

    net_mutex_lock(&g_ctrl_state[slot_id].mutex);
    //bitListSet(g_ctrl_state[slot_id].cfg_sync[type_id].valid, item_id);
    net_mutex_unlock(&g_ctrl_state[slot_id].mutex);

    return 0;
}

//���ư���ȫ��������ɱ�ʾ���
unsigned int
ctrl_net_cfg_sync_valid_clear(
                            const uint8_t   slot_id,
                            const uint8_t   type_id,
                            const uint16_t  item_id)
{  
    if (slot_id >= NET_MANAGE_MAX_SLOT || type_id >= NET_CFG_TYPE_NUM_OF
        || item_id >= NET_CFG_ITEM_NUM)
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;

    net_mutex_lock(&g_ctrl_state[slot_id].mutex);
    //bitListClr(g_ctrl_state[slot_id].cfg_sync[type_id].valid, item_id);
    net_mutex_unlock(&g_ctrl_state[slot_id].mutex);

    return 0;
}

//���ư���ȫ�������Ƿ����
unsigned int
ctrl_net_cfg_is_sync_valid(                            
                            const uint8_t   slot_id,
                            const uint8_t   type_id,
                            const uint16_t  item_id)
{  
    if (slot_id >= NET_MANAGE_MAX_SLOT || type_id >= NET_CFG_TYPE_NUM_OF
        || item_id >= NET_CFG_ITEM_NUM)
        return 0;
    return 0;    
    //return bitListTst(g_ctrl_state[slot_id].cfg_sync[type_id].valid, item_id);
}

//���ư���ȫ�������Ƿ����
void
ctrl_net_cfg_sync_invalid_set(const uint8_t slot_id)
{  
    if (slot_id >= NET_MANAGE_MAX_SLOT)
        return ;
        
    bzero(g_ctrl_state[slot_id].cfg_sync, CFG_SYNC_DATA_TOTAL_LEN);
}

#endif

#if DEFUNC("ȫ�����ñȶԽӿ�")

//ȫ��������ȶ�
unsigned int 
ctrl_net_cfg_is_consistent(const uint8_t slot_idx)
{
    //�Ƿ���β��漰�ȶ�
    if (slot_idx >= NET_MANAGE_MAX_SLOT)
    {
        return 1;
    }
    
    return !memcmp(g_ctrl_state[slot_idx].cfg_sync, g_ctrl_gbl_cfg_state.gbl_cfg, CFG_SYNC_DATA_TOTAL_LEN);
}

//�������ñȶ�
unsigned int
ctrl_net_cfg_type_is_consistent(
                                        const uint8_t slot_idx, 
                                        const uint8_t type_idx)
{
    //�Ƿ���β��漰�ȶ�
    if (slot_idx >= NET_MANAGE_MAX_SLOT || type_idx >= NET_CFG_TYPE_NUM_OF)
    {
        return 1;
    }

    return !memcmp(&g_ctrl_state[slot_idx].cfg_sync[type_idx], &g_ctrl_gbl_cfg_state.gbl_cfg[type_idx], CFG_SYNC_DATA_LEN);
}

//������������ȶ�
unsigned int
ctrl_net_cfg_item_group_is_consistent(
                                        const uint8_t slot_idx, 
                                        const uint8_t type_idx, 
                                        const uint8_t group_idx)
{
    //�Ƿ���β��漰�ȶ�
    if (slot_idx >= NET_MANAGE_MAX_SLOT || type_idx >= NET_CFG_TYPE_NUM_OF
        || group_idx >= NET_CFG_ITEM_NUM/8)
    {
        return 1;
    }

    return !memcmp(&g_ctrl_state[slot_idx].cfg_sync[type_idx].valid[group_idx], &g_ctrl_gbl_cfg_state.gbl_cfg[type_idx].valid[group_idx], sizeof(uint8_t));
}

#endif

#if DEFUNC("���ư����罨������ӿ�")

//δ��ɽ������ӵ�������Ϣ
static uint32_t
ctrl_net_map_id_get_by_connect(const uint32_t fd, uint8_t *p_map_id)
{
    uint8_t idx;

    for (idx = 0; idx < NET_MANAGE_MAX_SLOT; idx++)
    {
        if (g_ctrl_net_map[idx].net_param.watcher->fd == fd)
        {
            *p_map_id = idx;
            //fd match
            return 0;
        }
    }

    //not match fd, no record impossibility!!!
    return APP_ZK_RC_IMPOSSIBLE_ERROR; 
}

//�Ѿ������֤��������Ϣ
static uint32_t
ctrl_net_map_info_get_by_connect(
                                    const uint32_t  fd, 
                                    uint8_t         *p_slot_id, 
                                    uint8_t         *p_map_id)
{
    uint8_t idx;

    if (!p_slot_id || !p_map_id)
    {
        return APP_ZK_RC_PARAM_NULL;
    }
    
    for (idx = 0; idx < NET_MANAGE_MAX_SLOT; idx++)
    {
        if (g_ctrl_net_map[idx].net_param.watcher->fd == fd)
        {
            //fd match, slot info
            *p_map_id  = idx;
            *p_slot_id = g_ctrl_net_map[idx].slot_id;
            return 0;
        }
    }
    
    //not match fd, no record impossibility!!!
    return APP_ZK_RC_IMPOSSIBLE_ERROR;
}

static uint32_t
ctrl_net_map_conflict_check(const uint8_t slot_id)
{
    if (slot_id >= NET_MANAGE_MAX_SLOT)
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
        
    if (g_ctrl_state[slot_id].map_id != NET_COM_PARAM_INVALID)
        return APP_ZK_RC_NET_MAP_ALREADY_EXIST;
        
    return 0;
}

static uint32_t
ctrl_net_map_process(const uint8_t map_id, const uint8_t slot_id)
{
    uint32_t rc;

    //check slot info conflict
    if ((rc = ctrl_net_map_conflict_check(slot_id)))
    {
        return rc;
    }
    
    ctrl_net_map_id_set(slot_id, map_id);
    ctrl_net_map_slot_set(map_id, slot_id);
    
    return 0;
}

static uint32_t
ctrl_net_work_synch_process(const NET_MSG *pmsg, const uint8_t slot_id)
{
    uint32_t rc;
    uint8_t  map_id;

    if(!pmsg)
    {
        rc = APP_ZK_RC_PARAM_INVALID;
        goto syn_fail;
    }

    ctrl_net_map_id_get_by_connect(pmsg->fd, &map_id);

    if (slot_id >= NET_MANAGE_MAX_SLOT)
    {
        rc = APP_ZK_RC_PARAM_INVALID;
        goto syn_fail;
    }
      
    //���slot����·֮���ӳ��,slot��У����ӳ�������
    if ((rc = ctrl_net_map_process(map_id, slot_id)))
    {
        goto syn_fail;
    }

    //ͬ�������·״̬
    ctrl_net_state_set(slot_id, CTRL_NET_WORK_SYNCHRONOUS);
    
    printf("ctrl rec req work msg net map syn success, the conn slot %d, fd %d\r\n", slot_id, pmsg->fd);

    return rc;
    
syn_fail:

    //�Ͽ���ǰ�������¼�����������������Դ
    if(pmsg)
    {
        close(pmsg->fd);
        ctrl_net_active_disconnect_clear(g_ctrl_net_map[map_id].net_param.loop, 
                                            g_ctrl_net_map[map_id].net_param.watcher);
    }
    
    printf("ctrl rec req work msg net syn map fail reason %d\r\n", rc);
    
    return rc;
}

#endif

#if DEFUNC("���ư������������ӿ�")

static void
ctrl_net_active_disconnect_clear(EV_P_ struct ev_io *watcher)
{
    uint8_t map_id;
    
    //���ͷ�tcp������Դ
    ctrl_net_map_id_get_by_connect(watcher->fd, &map_id);
    //1. �ͷ�����ӳ����Դ
    ctrl_net_map_used_set(map_id, 0);
    //2. �ͷ����Ӷ�Ӧ�Ķ��¼�
    ev_io_stop(loop, watcher);
    net_safe_free(watcher);

    return ;
}

static void
ctrl_net_inactive_disconnect_clear(EV_P_ struct ev_io *watcher)
{
    uint8_t slot_id, map_id;

    //1. ��ն�Ӧslot������״̬  
    ctrl_net_map_info_get_by_connect(watcher->fd, &slot_id, &map_id);
    ctrl_net_state_clear(slot_id, CTRL_NET_WORK_SYNCHRONOUS);
    ctrl_net_cfg_sync_invalid_set(slot_id);
    //2. �ͷ�����ӳ����Դ
    ctrl_net_map_used_set(map_id, NET_COM_PARAM_INIT);
    ctrl_net_map_slot_set(map_id, NET_COM_PARAM_INVALID);
    ctrl_net_map_id_set(slot_id,  NET_COM_PARAM_INVALID);
    //3. �ͷ�tcp������Դ
    close(watcher->fd);
    //4. �ͷ����Ӷ�Ӧ�Ķ��¼�
    ev_io_stop(loop,watcher);
    net_safe_free(watcher);

    return ;
}

#endif

#if DEFUNC("���ư嶨ʱ�¼�����ӿ�")

static void 
ctrl_net_hb_timeout_process(uint8_t idx)
{
    //�Կ�����TF��������������    
    if (ctrl_net_state_working(idx))
    {
        ctrl_net_hb_cnt_add(idx);

        if (ctrl_net_hb_cnt_get(idx) > CTRL_STATE_HB_LOSE_CNT)
        {
            //�澯
            printf("ctrl hb check timeout slot %d!!!\n", idx);
        }
    }

    return ;
}

static void 
ctrl_net_map_timeout_process(uint8_t idx)
{
    struct sysinfo sys_info;
    uint32_t       duration;

    //���ռ�����ӳ�ʱ
    if (g_ctrl_net_map[idx].is_used && 
        g_ctrl_net_map[idx].slot_id == NET_COM_PARAM_INVALID)
    {
        sysinfo(&sys_info);
        
        if (g_ctrl_net_map[idx].timeout_stamp <= sys_info.uptime)
        {
            duration = sys_info.uptime - g_ctrl_net_map[idx].timeout_stamp;
        }
        else
        {
            //in case not possible 
            duration = CTRL_STATE_FD_TIMEOUT;
        }
  
        if (duration >= CTRL_STATE_FD_TIMEOUT)
        {
            //���ӳ�ʱ��Ҫ���
            printf("ctrl net map timeout !!!\r\n"
                   "current time %ld stamp time %u\r\nconn fd %d addr %s port %d\r\n", 
                    sys_info.uptime,
                    g_ctrl_net_map[idx].timeout_stamp,
                    g_ctrl_net_map[idx].net_param.watcher->fd,
                    inet_ntoa(g_ctrl_net_map[idx].net_param.remote_addr.sin_addr),
                    ntohs(g_ctrl_net_map[idx].net_param.remote_addr.sin_port));
         
            close(g_ctrl_net_map[idx].net_param.watcher->fd);
            //��������
            ctrl_net_active_disconnect_clear(g_ctrl_net_map[idx].net_param.loop, 
                                                g_ctrl_net_map[idx].net_param.watcher);
            
        }
    }

    return ;
}

#endif

#if DEF_FUNC("������Ϣ�첽����ӿ�")

//�첽������Ϣ��������
static net_msg_handler ctrl_net_asyn_req_handler[NET_MSG_TYPE_NUM_OF] = {
            NULL,
            ctrl_net_asyn_req_work_process,
            ctrl_net_asyn_req_hb_process,
            NULL, NULL, NULL, NULL, NULL, NULL,
            ctrl_net_asyn_req_auto_process,
            ctrl_net_asyn_req_alarm_process,
            NULL}; 

//�첽��Ӧ��Ϣ��������
static net_msg_handler ctrl_net_asyn_ack_handler[NET_MSG_TYPE_NUM_OF] = {
            NULL,
            NULL, NULL,
            ctrl_net_asyn_ack_onu_set_process,
            ctrl_net_asyn_ack_onu_get_process,
            ctrl_net_asyn_ack_olt_set_process,
            ctrl_net_asyn_ack_olt_get_process,
            ctrl_net_asyn_ack_nw_set_process,
            ctrl_net_asyn_ack_nw_get_process,
            NULL, NULL, NULL}; 

//����ҵ����첽������Ϣ
static UINT32 
ctrl_net_msg_asyn_req_process(NET_MSG *pmsg)
{
    UINT32 rc = 0;

    if (ctrl_net_asyn_req_handler[pmsg->msg_type])
    {
        if ((rc = ctrl_net_asyn_req_handler[pmsg->msg_type](pmsg)))
        {
            printf("%s %s %d error %d!!! \n", __FILE__, __FUNCTION__, __LINE__, rc);
        }

    }
    else
    {
        printf("%s %s %d req ctrl asyn msg handler not support msg type %d!!! \n", __FILE__, __FUNCTION__, __LINE__, 
                pmsg->msg_type);
    }
    
    return rc;
}

//����ҵ����첽��Ӧ��Ϣ
static UINT32 
ctrl_net_msg_asyn_ack_process(NET_MSG *pmsg)
{
    UINT32 rc = 0;

    if (ctrl_net_asyn_ack_handler[pmsg->msg_type])
    {
        if ((rc = ctrl_net_asyn_ack_handler[pmsg->msg_type](pmsg)))
        {
            printf("%s %s %d error %d!!! \n", __FILE__, __FUNCTION__, __LINE__, rc);
        }
    }
    else
    {
        printf("%s %s %d ack ctrl asyn msg not support msg type %d!!! \n", __FILE__, __FUNCTION__, __LINE__, 
                pmsg->msg_type);
    }
    
    return rc;
}

//�����첽������Ϣ��������
static void 
ctrl_net_rx_asyn_req_task(void)
{
    NET_OS_MSG  *p_os_msg;
    NET_MSG     *p_usr_msg;
    UINT32          rc;

    while (1)
    {
        //������Ϣ �ڴ�������ж�����
        rc = net_zc_mq_os_msg_get(&g_ctrl_rx_asyn_req_mq, NET_WAIT_FOREVER, &p_os_msg);
        if (rc)
        {
            printf("%s %s %d error %d!!!\r\n", __FILE__, __FUNCTION__, __LINE__, rc);
            continue;
        }

        p_usr_msg = (NET_MSG*)p_os_msg->data;

        //������Ϣ
        if ((rc = ctrl_net_msg_asyn_req_process(p_usr_msg)))
        {
            printf("%s %s %d error %d!!! \r\n", __FILE__, __FUNCTION__, __LINE__, rc);
        }

        //�ͷŶ�����Ϣ
        net_zc_mq_os_msg_free(p_os_msg);      
    }
    
    return;
}

//����ͬ����Ӧ��Ϣ��������
static void 
ctrl_net_rx_asyn_ack_task(void)
{
    NET_OS_MSG  *p_os_msg;
    NET_MSG     *p_usr_msg;
    UINT32          rc;

    while (1)
    {
        //������Ϣ �ڴ�������ж�����
        rc = net_zc_mq_os_msg_get(&g_ctrl_rx_asyn_ack_mq, NET_WAIT_FOREVER, &p_os_msg);
        if (rc)
        {
            printf("%s %s %d error %d!!!\r\n", __FILE__, __FUNCTION__, __LINE__, rc);
            continue;
        }

        p_usr_msg = (NET_MSG*)p_os_msg->data;

        //������Ϣ
        if ((rc = ctrl_net_msg_asyn_ack_process(p_usr_msg)))
        {
            printf("%s %s %d error %d!!! \r\n", __FILE__, __FUNCTION__, __LINE__, rc);
        }

        //�ͷŶ�����Ϣ
        net_zc_mq_os_msg_free(p_os_msg);      
    }
    
    return;
}

//�첽������Ϣ���������ʼ��
static UINT32 
ctrl_net_rx_asyn_req_task_init()
{
    pthread_t       tid;
    pthread_attr_t  attr;
    UINT32          rc;
    
    //rx queue
    rc = net_zc_mq_create(&g_ctrl_rx_asyn_req_mq, "server_rx_asyn_req_mq");
    if (rc)
    {
        printf("%s %s %d error %d\r\n", __FILE__, __FUNCTION__, __LINE__, rc);
        return APP_ZK_RC_MSG_QUEUE_CREATE;
    }

    //rx task
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
    
    if (pthread_create(&tid, &attr, (void *)ctrl_net_rx_asyn_req_task, NULL))
    {
        printf("%s %s %d error %d %s\n", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
        return APP_ZK_RC_TASK_CREATE;
    }

    return 0;
}

//�첽��Ӧ��Ϣ���������ʼ��
static UINT32 
ctrl_net_rx_asyn_ack_task_init()
{
    pthread_t       tid;
    pthread_attr_t  attr;
    UINT32          rc;
    
    //rx queue
    rc = net_zc_mq_create(&g_ctrl_rx_asyn_ack_mq, "server_rx_asyn_ack_mq");
    if (rc)
    {
        printf("%s %s %d error %d\r\n", __FILE__, __FUNCTION__, __LINE__, rc);
        return APP_ZK_RC_MSG_QUEUE_CREATE;
    }

    //rx task
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
    
    if (pthread_create(&tid, &attr, (void *)ctrl_net_rx_asyn_ack_task, NULL))
    {
        printf("%s %s %d error %d %s\n", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
        return APP_ZK_RC_TASK_CREATE;
    }

    return 0;
}

#endif

#if DEF_FUNC("������Ϣͬ������ӿ�")

//����ҵ���ͬ��������Ϣ������
static net_msg_handler ctrl_net_req_syn_handler[NET_MSG_TYPE_NUM_OF] = {
            NULL,
            NULL, NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL, NULL,
            ctrl_net_syn_req_drv_pull_process}; 

//����ҵ���ͬ��������Ϣ�ӿ�
static UINT32 
ctrl_net_msg_syn_req_process(NET_MSG *pmsg)
{
    UINT32 rc = 0;
    
    if (ctrl_net_req_syn_handler[pmsg->msg_type])
    {
        if ((rc = ctrl_net_req_syn_handler[pmsg->msg_type](pmsg)))
        {
            printf("%s %s %d error %d!!! \n", __FILE__, __FUNCTION__, __LINE__, rc);
        }
    }
    else
    {
        printf("%s %s %d req ctrl syn msg handler not support msg type %d!!! \n", __FILE__, __FUNCTION__, __LINE__, pmsg->msg_type);
    }

    return rc;
}

//����ͬ��������Ϣ��������
static void 
ctrl_net_rx_syn_req_task(void *p_arg)
{
    NET_OS_MSG  *p_os_msg;
    NET_MSG     *p_usr_msg;
    UINT32          rc;
    UINT32          slot_id = (UINT32)p_arg;
    NET_ZC_MQ   *p_mq = &g_ctrl_rx_syn_req_mq[slot_id];
    
    while (1)
    {
        //������Ϣ �ڴ�������ж�����
        rc = net_zc_mq_os_msg_get(p_mq, NET_WAIT_FOREVER, &p_os_msg);
        if (rc)
        {
            printf("%s %s %d error %d slot_id %d!!!\r\n", __FILE__, __FUNCTION__, __LINE__, rc, slot_id);
            continue;
        }

        p_usr_msg = (NET_MSG*)p_os_msg->data;
     
        //������Ϣ
        if ((rc = ctrl_net_msg_syn_req_process(p_usr_msg)))
        {
            printf("%s %s %d error %d slot_id %d!!!\r\n", __FILE__, __FUNCTION__, __LINE__, rc, slot_id);
        }

        //�ͷŶ�����Ϣ
        net_zc_mq_os_msg_free(p_os_msg);   
    }
    
    return;
}

//����ͬ����Ӧ��Ϣ��������
static void 
ctrl_net_rx_syn_ack_task(void *p_arg)
{
    NET_OS_MSG  *p_os_msg;
    NET_MSG     *p_usr_msg;
    UINT32          rc;
    UINT32          slot_id = (UINT32)p_arg;
    NET_ZC_MQ   *p_mq   = &g_ctrl_rx_syn_ack_mq[slot_id];

    while (1)
    {
        //������Ϣ �ڴ�������ж�����
        rc = net_zc_mq_os_msg_get(p_mq, NET_WAIT_FOREVER, &p_os_msg);
        if (rc)
        {
            printf("%s %s %d error %d slot %d!!!\r\n", __FILE__, __FUNCTION__, __LINE__, rc, slot_id);
            continue;
        }

        p_usr_msg = (NET_MSG*)p_os_msg->data;

        //������Ϣ, conn ��slot��Ӧ
        if ((rc = net_conn_msg_syn_ack_process(g_ctrl_conn[slot_id], p_usr_msg)))
        {
            printf("%s %s %d error %d slot %d!!!\r\n", __FILE__, __FUNCTION__, __LINE__, rc, slot_id);
        }

        //�ͷŶ�����Ϣ
        net_zc_mq_os_msg_free(p_os_msg);      
    }
    
    return;
}

//ͬ��������Ϣ���������ʼ��
static UINT32 
ctrl_net_rx_syn_req_task_init()
{
    pthread_t       tid;
    pthread_attr_t  attr;
    UINT32          rc;
    UINT32          slot_id;
    char            name[NET_NAME_LEN];

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
    
    //rx syn req queue & task
    for (slot_id = 0; slot_id < NET_MANAGE_MAX_SLOT; ++slot_id)
    {
        snprintf(name, sizeof(name), "server_rx_syn_req_mq_%d", slot_id);
        
        rc = net_zc_mq_create(&g_ctrl_rx_syn_req_mq[slot_id], name);
        if (rc)
        {
            printf("ctrl create rx syn req mq error %d slot %d\r\n", rc, slot_id);
            return APP_ZK_RC_MSG_QUEUE_CREATE;
        }

        if (pthread_create(&tid, &attr, (void *)ctrl_net_rx_syn_req_task, (void *)slot_id))
        {
            printf("ctrl create rx syn req task error %d %s, slot %d\r\n", errno, strerror(errno), slot_id);
            return APP_ZK_RC_TASK_CREATE;
        }
    }
     
    return 0;
}

//ͬ����Ӧ��Ϣ���������ʼ��
static UINT32 
ctrl_net_rx_syn_ack_task_init()
{
    pthread_t       tid;
    pthread_attr_t  attr;
    UINT32          rc;
    UINT32          slot_id;
    char            name[NET_NAME_LEN];

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
    
    //rx syn ack queue & task
    for (slot_id = 0; slot_id < NET_MANAGE_MAX_SLOT; ++slot_id)
    {        
        snprintf(name, sizeof(name), "server_rx_syn_ack_mq_%d", slot_id);
    
        rc = net_zc_mq_create(&g_ctrl_rx_syn_ack_mq[slot_id], name);
        if (rc)
        {
            printf("ctrl create rx syn ack mq error %d slot %d\r\n", rc, slot_id);
            return APP_ZK_RC_MSG_QUEUE_CREATE;
        }

        if (pthread_create(&tid, &attr, (void *)ctrl_net_rx_syn_ack_task, (void *)slot_id))
        {
            printf("ctrl create rx syn ack task error %d %s, slot %d\r\n", errno, strerror(errno), slot_id);
            return APP_ZK_RC_TASK_CREATE;
        }
    }
 
    return 0;
}

#endif

#if DEF_FUNC("������Ϣ����ӿ�")

//���Ͱ����Ϣ����
static void 
ctrl_net_tx_task(void *p_arg)
{
    NET_OS_MSG  *p_os_msg;
    NET_MSG     *p_msg;
    UINT32          rc;
    UINT32          slot_id = (UINT32)p_arg;
    NET_ZC_MQ   *p_mq   = &g_ctrl_tx_mq[slot_id];

    while(1)
    {
        //�ڴ�������ж�����
        rc = net_zc_mq_os_msg_get(p_mq, NET_WAIT_FOREVER, &p_os_msg);
        if (rc)
        {
            printf("%s %s %d error %d\r\n", __FILE__, __FUNCTION__, __LINE__, rc);
            continue;
        }
        
        p_msg = (NET_MSG*)p_os_msg->data;

        //msg header����������Ԥ��

        if (net_send_alone_packet(SLOT2NET_FD(slot_id), p_msg))
        {
            printf("%s %s %d error!!!\n", __FILE__, __FUNCTION__, __LINE__);  
        }

        //�ͷŶ�����Ϣ
        net_zc_mq_os_msg_free(p_os_msg);
    }

    return;
}

//���Ͱ����Ϣ���к������ʼ��
static UINT32 
ctrl_net_tx_task_init()
{
    pthread_t       tid;
    pthread_attr_t  attr;
    UINT32          rc;
    UINT32          slot_id;
    char            name[NET_NAME_LEN];

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

    //tx queue & task
    for (slot_id = 0; slot_id < NET_MANAGE_MAX_SLOT; ++slot_id)
    {
        snprintf(name, sizeof(name), "server_tx_mq_%d", slot_id);
        
        rc = net_zc_mq_create(&g_ctrl_tx_mq[slot_id], name);
        if (rc)
        {
            printf("ctrl create tx mq error %d slot %d!!!\n", rc, slot_id);
            return APP_ZK_RC_MSG_QUEUE_CREATE;
        }

        if (pthread_create(&tid, &attr, (void *)ctrl_net_tx_task, (void *)slot_id) < 0)
        {
            printf("ctrl create tx task error %d %s, slot %d!!!\n", errno, strerror(errno), slot_id);
            return APP_ZK_RC_TASK_CREATE;
        }
    }
    
    return 0;
}

#endif

#if DEF_FUNC("���зַ�")

//���հ����Ϣ�첽�ַ�����
static void 
ctrl_net_msg_asyn_mq_put(NET_OS_MSG *p_os_msg)
{
    NET_MSG *p_usr_msg = p_os_msg->data; 
    
    switch(p_usr_msg ->direction)
    {
        case NET_MSG_DRICTION_REQUEST:
            net_zc_mq_os_msg_put(&g_ctrl_rx_asyn_req_mq, p_os_msg);
            break;

        case NET_MSG_DRICTION_ACK:
            net_zc_mq_os_msg_put(&g_ctrl_rx_asyn_ack_mq, p_os_msg);
            break;

        default:  
            net_zc_mq_os_msg_free(p_os_msg);      
            printf("%s %s %d ctrl not support msg direction!!! \r\n", __FILE__, __FUNCTION__, __LINE__);
            break;
    }
  
    return ;
}

//���հ��ͬ����Ϣ�ַ�����
static void 
ctrl_net_msg_syn_mq_put(NET_OS_MSG *p_os_msg)
{
    NET_MSG *p_usr_msg = p_os_msg->data;
    UINT8       slot_id    = MSG2SLOT(p_usr_msg);

    switch(p_usr_msg ->direction)
    {
        case NET_MSG_DRICTION_REQUEST:
            net_zc_mq_os_msg_put(&g_ctrl_rx_syn_req_mq[slot_id], p_os_msg);
            break;

        case NET_MSG_DRICTION_ACK:
            net_zc_mq_os_msg_put(&g_ctrl_rx_syn_ack_mq[slot_id], p_os_msg);
            break;

        default:
            net_zc_mq_os_msg_free(p_os_msg);     
            printf("%s %s %d ctrl not support msg direction!!! \r\n", __FILE__, __FUNCTION__, __LINE__);
            break;
    }
  
    return ;
}

//���հ����Ϣ�����������,��κϷ������ϼ���֤
static void 
ctrl_net_rx_mq_dispatch(NET_OS_MSG *p_os_msg)
{
    NET_MSG *p_usr_msg = (NET_MSG*)p_os_msg->data;
    
    if (NET_MSG_ASYN == p_usr_msg->syn_flag)
    {
        ctrl_net_msg_asyn_mq_put(p_os_msg);
    }
    else
    {
        ctrl_net_msg_syn_mq_put(p_os_msg);
    }

    return ;
}

//���հ����Ϣ�����,��κϷ������ϼ���֤
static void 
ctrl_net_rx_mq_put(NET_MSG *p_data)
{
    NET_OS_MSG   *p_msg;
    
    //������bcmos msg�ɶ��н��ն��ͷ�
    p_msg = (NET_OS_MSG*)net_malloc(&g_ctrl_tr_mq_mp, sizeof(NET_OS_MSG));
    if (!p_msg)
    {
        net_safe_free(p_data);
        printf("%s %s %d malloc fail\r\n", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
    
    p_msg->data    = p_data;
    p_msg->size    = p_data->len;//�����е���Ϣ�峤��Ϊ�����Ϣ��Ч����

    //��Ϣ�ַ����У�����������os�ӿ��й�
    ctrl_net_rx_mq_dispatch(p_msg);

    return;
}

//���Ͱ����Ϣ�����
static unsigned int 
ctrl_net_tx_mq_put(NET_MSG *p_data)
{
    NET_OS_MSG  *p_msg;
    //����msg�ڴ��ɶ��н��ն��ͷ�
    p_msg = (NET_OS_MSG*)net_malloc(&g_ctrl_tr_mq_mp, sizeof(NET_OS_MSG));
    if (!p_msg)
    {
        net_safe_free(p_data);
        printf("%s %s %d error\r\n", __FILE__, __FUNCTION__, __LINE__);
        return APP_ZK_RC_MEM_ALLOCATION;
    }

    //data �ֶδ洢�����Ϣ��
    p_msg->data     = p_data;
    p_msg->size     = p_data->len;
    
    net_zc_mq_os_msg_put(&g_ctrl_tx_mq[MSG2SLOT(p_data)], p_msg);
    
    return 0;
}

#endif

#if DEF_FUNC("���繫���ӿ�")

//��Ϣ���ͽӿ�
static unsigned int 
ctrl_net_msg_send(  NET_MSG         *pmsg,
                            const void          *pvar, 
                            const unsigned int  len,
                            const int           state,
                            const int           direction)
{
    NET_MSG      *ptmsg;
    unsigned int     rc;

    if (len)
    {
        ptmsg = net_msg_pack(&g_ctrl_net_msg_mp, pmsg->vif_info, pmsg->cmd_info, pvar, len, len);
    }
    else
    {
        ptmsg = net_msg_pack(&g_ctrl_net_msg_mp, pmsg->vif_info, pmsg->cmd_info, NULL, 0, 0);  
    }

    /*ptmsg �ڴ��ɷ��Ͷ˻��߷��Ͷ��нӿ��ͷ���Ҫ��֤body_size �� lenһ��*/
    if (!ptmsg)
    {
         printf("%s %s %d error\n", __FILE__, __FUNCTION__, __LINE__);
         return APP_ZK_RC_PARAM_NULL;
    }
    
    ptmsg->msg_type  = pmsg->msg_type;
    ptmsg->direction = direction;
    ptmsg->syn_flag  = pmsg->syn_flag;
    ptmsg->corr_tag  = pmsg->corr_tag;
    ptmsg->state     = state;
    
    if ((rc = ctrl_net_tx_mq_put(ptmsg)))
    {
        printf("%s %s %d error\n", __FILE__, __FUNCTION__, __LINE__);
    }

    return rc;
}

static UINT32 
ctrl_net_init(int *p_fd)
{
    struct sockaddr_in  sin;
    int                 addrlen = sizeof(struct sockaddr);
    int                 fd;

    if (!p_fd)
    {
        return APP_ZK_RC_PARAM_NULL;
    }
    
    //��������fd����ȡ�����ַ��Ϣ
    if (net_init(&sin, &fd))
    {
        goto fail;
    }
    
    sin.sin_family = AF_INET;
    sin.sin_port   = htons(SERVER_PORT);
    
    if (bind(fd, (struct sockaddr *)&sin, addrlen) < 0)
    {
        close(fd);
        goto fail;
    } 

    *p_fd = fd;
    
    return 0;
    
fail:
    printf("%s %s %d error %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
    return APP_ZK_RC_NET_INIT;
}

//read �����¼��ص�
static void 
ctrl_net_read_cb(EV_P_ struct ev_io *watcher, int revents)
{  
    NET_MSG   msg_header;
    NET_MSG   *msg = NULL;
    unsigned int rc = EV_ERROR & revents;

    printf("control readcb events:%d\r\n", revents);
      
    if (rc)  
    {  
      printf("ctrl read event error revents %d\r\n", revents);
      
      if ((EV_ERROR^revents) == ERR_EV_LOCAL_FD_CLOSE)
        rc = MDW_NET_RC_LOCAL_DISCONNECT;
        
      goto fail;
    }

    //1. Ԥ��ȡ��Ϣͷ
    if ((rc = net_rec_packet_fix_peek(watcher->fd, &msg_header, NET_MSG_HEAD_LEN)))
    {
        //������������
        goto fail;
    }
    
    //2. ͷ����С��ת��

    //3. ������Ϣ�� 
    msg = net_malloc(&g_ctrl_net_msg_mp, msg_header.len);
    if (!msg)
    {
        printf("%s %s %d errorno %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        goto fail;
    }

    //4. Ԥ��ȡ��Ϣ��
    if ((rc = net_rec_packet_fix_peek(watcher->fd, msg->body, (msg_header.len - NET_MSG_HEAD_LEN))))
    {
        //������������
        goto fail;
    }

    //5. �ӻ�����ȡ��Ϣ����Ϣ���ڴ���˴�С��ת��
    if ((rc = net_rec_packet_fix(watcher->fd, msg, msg_header.len)))
    {
        //��������
        goto fail;
    }

    //6. ͷ����С��ת��

    //7. ��ϢͷУ��
    if (net_msg_verify(msg))
    {
        //ʧ�ܣ�������Ϣ������
        printf("ctrl read cb msg verify error!!!:\n"
                "msg_type: %d\n"
                "cmd_id: %d\ntimeout: %d\norder: %d\n" 
                "vif_info: %lld\n"
                "syn_flag: %d\ncor_tag: %d\ndir: %d\n"
                "state: %d\n"
                "len: %d\n"
                "compress: %d\n"
                "body_size: %d\n",
                msg->msg_type,
                msg->cmd_info.cmd_id,
                msg->cmd_info.timeout,
                msg->cmd_info.order,
                msg->vif_info,
                msg->syn_flag,
                msg->corr_tag,
                msg->direction, 
                msg->state, 
                msg->len,
                msg->compress,
                msg->body_size);
        
        goto fail;
    }

#if 0
    printf("ctrl rx cb msg info no peek:\n"
            "msg_type: %d\n"
            "cmd_id: %d\ntimeout: %d\norder: %d\n" 
            "vif_info:%llu\n"
            "syn_flag: %d\ncor_tag: %d\ndir: %d\n"
            "state: %d\n"
            "len: %d\n"
            "compress: %d\n"
            "size: %d\n",
            msg->msg_type,
            msg->cmd_info.cmd_id,
            msg->cmd_info.timeout,
            msg->cmd_info.order,
            msg->vif_info,
            msg->syn_flag,
            msg->corr_tag,
            msg->direction, 
            msg->state, 
            msg->len,
            msg->compress,
            msg->body_size);
#endif   

    if (net_msg_uncompress_process(&g_ctrl_net_msg_mp, &msg))
    {
          printf("ctrl rx cb uncompress msg fail\r\n");
          goto fail;
    }

    //��¼������Ϣ
    msg->fd = watcher->fd;
    //��Ϣ����ն��л��洦��
    ctrl_net_rx_mq_put(msg); 
    
    return ;
  
fail:

    net_safe_free(msg);
  
    //ҵ���汻������
    if(rc == MDW_NET_RC_REMOTE_DISCONNECT)  
    {  
        printf("ctrl rx cb remote disconnect \r\n");
        ctrl_net_inactive_disconnect_clear(loop, watcher);  
        //�澯
    }
    else
    if (rc == MDW_NET_RC_LOCAL_DISCONNECT)
    {
        //������ƣ�ҵ������������
        printf("ctrl rx cb local disconnect \r\n");
        ctrl_net_active_disconnect_clear(loop, watcher);
    }
    else
    if (rc)
    {
        printf("ctrl read cb msg error!!!\r\n");
    }

    return ;
}

//accept�¼� �Ļص���  
static void 
ctrl_net_accept_cb(EV_P_ struct ev_io *watcher, int revents)  
{  
    struct sockaddr_in  client_addr;  
    socklen_t           client_len = sizeof(client_addr);
    char                ipaddr[20] = {0};
    int                 client_fd; 
    struct ev_io        *w_client;
    uint32_t            map_idx;
   
    //libev�Ĵ�����  
    if(EV_ERROR & revents)  
    {  
      printf("error event in accept\r\n");  
      return;  
    } 
    
    //��ҵ�������ӵ�ev io 
    client_fd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);  
    if (client_fd < 0)  
    {  
      printf("accept error\r\n");  
      return;  
    }  

    //���ʿ���Ԥ���� ��ַ������
    //for debug
    printf("ctrl someone connected. new fd %d\r\nip: %s\r\nport: %d\r\n", 
            client_fd,
            inet_ntop(AF_INET, (void*)&client_addr.sin_addr, ipaddr, sizeof(ipaddr)),
            ntohs(client_addr.sin_port));

    if (ctrl_net_map_free_id_get(&map_idx))
    {
        //û�п��е�slotӳ��id������������
        close(client_fd);
        printf("ctrl not have free connect\r\n");
        return;
    }

    w_client = (struct ev_io*)net_malloc(&g_ctrl_watcher_mp, sizeof(struct ev_io));
    if (!w_client)
    {
        close(client_fd);
        printf("%s %s %d error\r\n", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
    
    //�������¼�
    net_setblock(client_fd, NET_FD_SET_NO_BLOCK);
    ev_io_init(w_client, ctrl_net_read_cb, client_fd, EV_READ);  
    ev_io_start(EV_A_ w_client);

    //��ʼ�����������Ϣ
    ctrl_net_map_net_param_init(EV_A_ w_client, map_idx, &client_addr);

    return;
}

//��ʱ�¼�
static void
ctrl_net_timeout_cb (EV_P_ ev_timer *w, int revents)
{
    uint8_t        idx;
    
    for (idx = 0; idx < NET_MANAGE_MAX_SLOT; idx++)
    {
        ctrl_net_hb_timeout_process(idx);

        //close(g_ctrl_net_map[ctrl_net_map_id_get(idx)].net_param.fd);
        ctrl_net_map_timeout_process(idx);
    }

    return ;
}

static void 
ctrl_net_work_main(void)
{
    struct ev_loop  *loop = ev_default_loop(0);  
    int             fd;  
    ev_io           accept_watcher;  //�����¼�
    ev_timer        timeout_watcher;//��ʱ�������

    //��ʼ�����ư����
    if (ctrl_net_init(&fd))
    {
        return;
    }

    if (net_setblock(fd, NET_FD_SET_NO_BLOCK))
    {
        printf("%s %s %d error\n", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
  
    if (listen(fd, 8) < 0)  
    {  
        printf("%s %s %d errorno %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        return;  
    }

    //��ʼ��conn ȫ����
    net_mutex_create(&g_ctrl_conn_lock);
    
    // ����io�¼�
    ev_io_init(&accept_watcher, ctrl_net_accept_cb, fd, EV_READ);  
    ev_io_start(loop, &accept_watcher);
    //��ʱ�¼�
    ev_timer_init (&timeout_watcher, ctrl_net_timeout_cb, 5, 5);
    ev_timer_start (loop, &timeout_watcher);

    ev_loop(loop, 0); 
}

//�ڴ�س�ʼ��
UINT32 
ctrl_net_mempool_init()
{
    //���շ����߳���Ϣ������Ϣ�ڴ��
    NET_BLK_POOL_PARM net_tr_mq_parm    = {"ctrl_tr_mq_mp",
                                           sizeof(NET_OS_MSG),
                                           NET_MSG_MAX_COUNT};
              
    NET_BLK_POOL_PARM net_msg_parm      = {"ctrl_net_msg_mp",
                                           NET_BUFFER_LEN,//(sizeof(NET_MSG) + NET_BUFFER_LEN),
                                           NET_MSG_MAX_COUNT};

    NET_BLK_POOL_PARM net_watcher_parm  = {"ctrl_net_watcher_mp",
                                           sizeof(union ev_any_watcher),
                                           NET_WATCHER_COUNT};

    NET_BLK_POOL_PARM net_conn_parm     = {"ctrl_net_conn_mp",
                                           sizeof(NET_CONN),
                                           NET_MANAGE_MAX_SLOT};

    NET_BLK_POOL_PARM net_conn_msg_parm = {"ctrl_net_conn_msg_mp",
                                           (sizeof(NET_CONN_MSG)*CTRL_NET_CONN_MSG_NUM),
                                           NET_MANAGE_MAX_SLOT};
    UINT32 rc;
    
    rc = net_blk_pool_create(&g_ctrl_tr_mq_mp,    &net_tr_mq_parm);
    rc = rc ? rc : net_blk_pool_create(&g_ctrl_net_msg_mp,  &net_msg_parm);
    rc = rc ? rc : net_blk_pool_create(&g_ctrl_watcher_mp,  &net_watcher_parm);
    rc = rc ? rc : net_blk_pool_create(&g_ctrl_conn_mp,     &net_conn_parm);
    rc = rc ? rc : net_blk_pool_create(&g_ctrl_conn_msg_mp, &net_conn_msg_parm);

    return rc;
}

UINT32 
ctrl_net_task_init(void)
{
    UINT32 rc;
    //��ʼ��������պͷ�������
    if((rc = ctrl_net_rx_syn_req_task_init()))
    {
        printf("%s %s %d error !!!\n", __FILE__, __FUNCTION__, __LINE__);
        return rc;
    };

    if((rc = ctrl_net_rx_syn_ack_task_init()))
    {
        printf("%s %s %d error !!!\n", __FILE__, __FUNCTION__, __LINE__);
        return rc;
    };

    if((rc = ctrl_net_rx_asyn_req_task_init()))
    {
        printf("%s %s %d error !!!\n", __FILE__, __FUNCTION__, __LINE__);
        return rc;
    };

    if((rc = ctrl_net_rx_asyn_ack_task_init()))
    {
        printf("%s %s %d error !!!\n", __FILE__, __FUNCTION__, __LINE__);
        return rc;
    };

    if((rc = ctrl_net_tx_task_init()))
    {
        printf("%s %s %d error !!!\n", __FILE__, __FUNCTION__, __LINE__);
        return rc;
    };

    return 0;
}

//�����߳������첽�¼������������������
UINT32 
ctrl_net_ev_init(void)
{
    pthread_t      tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
    
    if (pthread_create(&tid, &attr, (void *)ctrl_net_work_main, NULL))
    {
        printf("%s %s %d error %d %s\r\n", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
        return APP_ZK_RC_TASK_CREATE;
    }
    
    return 0;
}

#endif

#if DEF_FUNC("�����첽������Ϣ����ӿ�")

static UINT32 
ctrl_net_asyn_req_work_process(NET_MSG *pmsg)
{     
    uint32_t        rc;
    NET_PHY_INFO phy_info;
    uint32_t        slot_id;
    
    //��ȡ��Ϣ��������
    if((rc = net_vif2phy(pmsg->vif_info, &phy_info)))
    {
        printf("%s %s %d error %d!!! \n", __FILE__, __FUNCTION__, __LINE__, rc);
        return rc;
    }

    slot_id = NET_PHY_INFO_TO_SLOT_ID(phy_info);

    //ͬ�������·״̬
    if ((rc = ctrl_net_work_synch_process(pmsg, slot_id)))
    {
        return rc;
    }
     
    //��Ӧҵ�����ʱ�������� 
    ctrl_net_msg_send(pmsg, NULL, 0, 0, NET_MSG_DRICTION_ACK);
  
    //֪ͨ��������tf�������
    //ʹ�ܿ��ư�cli

    printf("ctrl recv work form fk \r\n");

    return 0;  
}

static UINT32 
ctrl_net_asyn_req_hb_process(NET_MSG *pmsg)
{  
    unsigned int        rc;
    //rc_info         rc_info = {.obj_id = OBJ_APP_STAT};
    NET_PHY_INFO    phy_info;
    uint8_t             slot_id;
    //const NIF_LOAD_INFO *p_load_info;

    if((rc = net_vif2phy(pmsg->vif_info, &phy_info)))
    {
        printf("%s %s %d error %d!!! \n", __FILE__, __FUNCTION__, __LINE__, rc);
        return rc;
    }

    slot_id = NET_PHY_INFO_TO_SLOT_ID(phy_info);

    //��¼״̬�峬ʱ
    ctrl_net_hb_cnt_set(slot_id, 0);
    printf("zk rec heartbeat\r\n");
    
#if 0    
    //��ȡ���帺����Ϣ
    if (pmsg->len - NET_MSG_HEAD_LEN != sizeof(NIF_LOAD_INFO))
    {
       rc_info.err_id = APP_TFALARM_RC_PARAM_ONT_OF_RANGE;
       info2rc_convert(&rc, &rc_info);
       goto end;
    }
    else
    {
        p_load_info = (const NIF_LOAD_INFO *)(void*)pmsg->body;
        //printf("ctrl rec hb slot %d mem load %d%c cpu load %d%c\r\n", 
                //slot_id, 
                //p_load_info->mem_load, '%',
                //p_load_info->cpu_load, '%');
        if (p_load_info->mem_load > 95 || p_load_info->cpu_load > 95)
        {
            printf("ctrl rec hb slot %d mem load %d%c cpu load %d%c\r\n", 
                    slot_id, 
                    p_load_info->mem_load, '%',
                    p_load_info->cpu_load, '%');
        }
    }
#endif

//end:

    return rc;
}

static UINT32 
ctrl_net_asyn_req_auto_process(NET_MSG *pmsg)
{
    pmsg = pmsg;
    //1.��ǰ��¼״̬
    
    return 0;
}

static UINT32 
ctrl_net_asyn_req_alarm_process(NET_MSG *pmsg)
{
    unsigned int       rc;
    //rc_info         rc_info = {.obj_id = OBJ_APP_ALARM};
    NET_PHY_INFO    phy_info;
    //NET_ALARM_INFO  alarm_info;
    //uint8_t             dev_id;
    
#if 0
    printf("ctrl rx alarm msg info:\r\n"
              "msg_type: %d\n"
              "cmd_id: %d\ntimeout: %d\n" 
              "vif_info: %lld\n"
              "syn_flag: %d\ncor_tag: %d\ndir: %d\n"
              "state: %d\n"
              "len: %d\n"
              "body_size: %d\n",
              msg->msg_type,
              msg->cmd_info.cmd_id,
              msg->cmd_info.timeout,
              msg->vif_info,
              msg->syn_flag,
              msg->corr_tag,
              msg->direction, 
              msg->state, 
              msg->len,
              msg->body_size);
 #endif
 
    //��ȡ��Ϣ��������
    if((rc = net_vif2phy(pmsg->vif_info, &phy_info)))
    {
        printf("%s %s %d error %d!!! \n", __FILE__, __FUNCTION__, __LINE__, rc);
        return rc;
    }
    
    //dev_id = NET_PHY_INFO_TO_DEV_ID(phy_info);
/*    alarm_info.slot_id      = NET_PHY_INFO_TO_SLOT_ID(phy_info);
    alarm_info.port_id       = NET_PHY_INFO_TO_ID(phy_info);
    alarm_info.node_id      = NET_PHY_INFO_TO_ONU_ID(phy_info);
    alarm_info.alarm_id     = pmsg->cmd_info.cmd_id;
    alarm_info.alarm_state  = NET_ALARM_STATE_REPORT;*/

    switch(pmsg->cmd_info.cmd_id)
    {
#if 0    
        case NET_MSG_CMD_ALARM_ONU_OFFLINE:
            {
                const NIF_ONU_BASE_INFO_ALARM *p_onu_base_info_alarm;
                if (pmsg->len - NET_MSG_HEAD_LEN != sizeof(NIF_ONU_BASE_INFO_ALARM))
                {
                    rc_info.err_id = APP_TFALARM_RC_PARAM_ONT_OF_RANGE;
                    info2rc_convert(&rc, &rc_info);\
                    goto end;
                }
                else
                {
                    p_onu_base_info_alarm = (const NIF_ONU_BASE_INFO_ALARM *)(void*)pmsg->body;
                }
                
                alarm_info.para_bit_map = NET_ALARM_PARA_TYPE_SLOT | NET_ALARM_PARA_TYPE_TFID | NET_ALARM_PARA_TYPE_ONU_SN;//NET_ALARM_PARA_TYPE_ONU_PWD
                memcpy(alarm_info.phy_addr, p_onu_base_info_alarm->sn, sizeof(alarm_info.phy_addr));
                memcpy(alarm_info.pwd, p_onu_base_info_alarm->pw, sizeof(alarm_info.pwd));

                if (pmsg->cmd_info.cmd_id == NET_MSG_CMD_ALARM_ONU_ONLINE)
                {
                    //printf("ctrl led online alarm!!! pre slot %d, %d num %d\n", alarm_info.slot_id, alarm_info.port_id, ctrl_net_port_onu_online_get(alarm_info.slot_id, alarm_info.port_id));
                    //��һ����������
                    if (!ctrl_net_port_onu_online_get(alarm_info.slot_id, alarm_info.port_id))
                    {

                    }
                    
                    ctrl_net_port_onu_online_add(alarm_info.slot_id, alarm_info.port_id);
                }
                else
                if (pmsg->cmd_info.cmd_id == NET_MSG_CMD_ALARM_ONU_OFFLINE)
                {
                    //printf("ctrl led offline alarm pre slot %d, %d num %d\n", alarm_info.slot_id, alarm_info.port_id, ctrl_net_port_onu_online_get(alarm_info.slot_id, alarm_info.port_id));
                    //ȫ��������Ҫ���
                    ctrl_net_port_onu_online_dec(alarm_info.slot_id, alarm_info.port_id);
                    if (!ctrl_net_port_onu_online_get(alarm_info.slot_id, alarm_info.port_id))
                    {

                    }
                }
            }
            break;
            
        case NET_MSG_CMD_ALARM_ONU_RANGE_FAIL:
            {
                const NIF_ONU_RANGE_FAIL_ALARM *p_onu_range_alarm;
                if (pmsg->len - NET_MSG_HEAD_LEN != sizeof(NIF_ONU_RANGE_FAIL_ALARM))
                {
                    rc_info.err_id = APP_TFALARM_RC_PARAM_ONT_OF_RANGE;
                    info2rc_convert(&rc, &rc_info);\
                    goto end;
                }
                else
                {
                    p_onu_range_alarm = (const NIF_ONU_RANGE_FAIL_ALARM *)(void*)pmsg->body;
                }
                
                alarm_info.para_bit_map = NET_ALARM_PARA_TYPE_SLOT | NET_ALARM_PARA_TYPE_TFID | NET_ALARM_PARA_TYPE_ONU_SN;//NET_ALARM_PARA_TYPE_ONU_PWD
                memcpy(alarm_info.phy_addr, p_onu_range_alarm->onu_base_info.sn, sizeof(alarm_info.phy_addr));
                memcpy(alarm_info.pwd, p_onu_range_alarm->onu_base_info.pw, sizeof(alarm_info.pwd));
                //printf("ctrl onu range fail reasion: %d\n", p_onu_range_alarm->range_fail_reasion);
            }
            break;
            
        case NET_MSG_CMD_ALARM_ONU_ACTIVE_FAIL:
            {
                const NIF_ONU_ACTIVATE_FAIL_ALARM *p_onu_activate_fail;
                if (pmsg->len - NET_MSG_HEAD_LEN != sizeof(NIF_ONU_ACTIVATE_FAIL_ALARM))
                {
                    rc_info.err_id = APP_TFALARM_RC_PARAM_ONT_OF_RANGE;
                    info2rc_convert(&rc, &rc_info);\
                    goto end;
                }
                else
                {
                    p_onu_activate_fail = (const NIF_ONU_ACTIVATE_FAIL_ALARM *)(void*)pmsg->body;
                }
                
                alarm_info.para_bit_map = NET_ALARM_PARA_TYPE_SLOT | NET_ALARM_PARA_TYPE_TFID | NET_ALARM_PARA_TYPE_ONU_SN;//NET_ALARM_PARA_TYPE_ONU_PWD
                memcpy(alarm_info.phy_addr, p_onu_activate_fail->onu_base_info.sn, sizeof(alarm_info.phy_addr));
                memcpy(alarm_info.pwd, p_onu_activate_fail->onu_base_info.pw, sizeof(alarm_info.pwd));
                printf("ctrl onu active fail reasion: %d\n", p_onu_activate_fail->fail_reasion);
            }
            break;
            
        case NET_MSG_CMD_ALARM_ONU_PWD_AUTH_FAIL:
            {
                const NIF_ONU_PASSWORD_AUTH_FAIL_ALARM *p_onu_password_auth_fail;
                if (pmsg->len - NET_MSG_HEAD_LEN != sizeof(NIF_ONU_PASSWORD_AUTH_FAIL_ALARM))
                {
                    rc_info.err_id = APP_TFALARM_RC_PARAM_ONT_OF_RANGE;
                    info2rc_convert(&rc, &rc_info);\
                    goto end;
                }
                else
                {
                    p_onu_password_auth_fail = (const NIF_ONU_PASSWORD_AUTH_FAIL_ALARM *)(void*)pmsg->body;
                }
                
                alarm_info.para_bit_map = NET_ALARM_PARA_TYPE_SLOT | NET_ALARM_PARA_TYPE_TFID | NET_ALARM_PARA_TYPE_ONU_SN;//NET_ALARM_PARA_TYPE_ONU_PWD
                memcpy(alarm_info.phy_addr, p_onu_password_auth_fail->onu_base_info.sn, sizeof(alarm_info.phy_addr));
                memcpy(alarm_info.pwd, p_onu_password_auth_fail->onu_base_info.pw, sizeof(alarm_info.pwd));
                //printf("ctrl onu active fail reasion: %d\n", p_onu_password_auth_fail->range_fail_reasion);
            }
            break;
           
        case NET_MSG_CMD_ALARM_ONU_DF:
            {
                const NIF_ONU_ALARM_STATE *onu_alarm_state;
                if (pmsg->len - NET_MSG_HEAD_LEN != sizeof(NIF_ONU_ALARM_STATE))
                {
                    rc_info.err_id = APP_TFALARM_RC_PARAM_ONT_OF_RANGE;
                    info2rc_convert(&rc, &rc_info);\
                    goto end;
                }
                else
                {
                    onu_alarm_state = (const NIF_ONU_ALARM_STATE *)(void*)pmsg->body;
                }
                
                alarm_info.alarm_state  = onu_alarm_state->onu_alarm_state;
                alarm_info.para_bit_map = NET_ALARM_PARA_TYPE_SLOT | NET_ALARM_PARA_TYPE_TFID | NET_ALARM_PARA_TYPE_ONU_SN;//NET_ALARM_PARA_TYPE_ONU_PWD
                memcpy(alarm_info.phy_addr, onu_alarm_state->onu_base_info.sn, sizeof(alarm_info.phy_addr));
                memcpy(alarm_info.pwd, onu_alarm_state->onu_base_info.pw, sizeof(alarm_info.pwd));
            }
            break; 
            
        case NET_MSG_CMD_ALARM_STATE_CHANGE:
            {
                const NIF_NI_STATE_CHANGE *ni_state_change;
                if (pmsg->len - NET_MSG_HEAD_LEN != sizeof(NIF_NI_STATE_CHANGE))
                {
                    rc_info.err_id = APP_TFALARM_RC_PARAM_ONT_OF_RANGE;
                    info2rc_convert(&rc, &rc_info);\
                    goto end;
                }
                else
                {
                    ni_state_change = (const NIF_NI_STATE_CHANGE*)(void*)pmsg->body;
                }

                //Ŀǰֻ��עni deactive ��ص�״̬�仯
                if (ni_state_change->result == 0)
                {
                    if (ni_state_change->previous_state != 0 && ni_state_change->new_state == 0)
                    {
                        alarm_info.alarm_state = NET_ALARM_STATE_REPORT;
                    }
                    else
                    if (ni_state_change->previous_state == 0 && ni_state_change->new_state == 2)
                    {
                        alarm_info.alarm_state = NET_ALARM_STATE_CLEAR;
                    }
                    else
                    {
                        //other sate change
                        goto end;
                    }
                }
                else
                {
                    //state change not suc
                    goto end;
                }
                
                alarm_info.para_bit_map = NET_ALARM_PARA_TYPE_SLOT | NET_ALARM_PARA_TYPE_TFID;
            }
            break;
            
        case NET_MSG_CMD_ALARM_LOS:
            {
                const uint32_t *ni_los_state;
                if (pmsg->len - NET_MSG_HEAD_LEN != sizeof(uint32_t))
                {
                    rc_info.err_id = APP_TFALARM_RC_PARAM_ONT_OF_RANGE;
                    info2rc_convert(&rc, &rc_info);
                    goto end;
                }
                else
                {
                    ni_los_state = (const uint32_t*)(void*)pmsg->body;
                }
                
                alarm_info.alarm_state  = *ni_los_state;
                alarm_info.para_bit_map = NET_ALARM_PARA_TYPE_SLOT | NET_ALARM_PARA_TYPE_TFID;
            }
            break;
            
        case NET_MSG_CMD_ALARM_HOST_CONNECT_FAIL:
            {
                const NIF_HOST_CONNECTION_FAIL_REASON *p_fail_reasion;
                if (pmsg->len - NET_MSG_HEAD_LEN != sizeof(NIF_HOST_CONNECTION_FAIL_REASON))
                {
                    rc_info.err_id = APP_TFALARM_RC_PARAM_ONT_OF_RANGE;
                    info2rc_convert(&rc, &rc_info);
                    goto end;
                }
                else
                {
                    p_fail_reasion = (const NIF_HOST_CONNECTION_FAIL_REASON*)(void*)pmsg->body;
                }
                //68620�����������ӳɹ� ���
                printf("connect device fail reasion: %d!!!\n", *p_fail_reasion);
                alarm_info.alarm_state  = NET_ALARM_STATE_REPORT;
                alarm_info.para_bit_map = NET_ALARM_PARA_TYPE_SLOT;
            }
            break;
#endif            
        default:
            {
                goto end;
            }
        
    }
    
    //rc = ctrl_alarm_process(&alarm_info);
    
end:
    
    return rc;
}

#endif

#if DEF_FUNC("�����첽��Ӧ��Ϣ����ӿ�")

static UINT32 
ctrl_net_asyn_ack_onu_set_process(NET_MSG *pmsg)
{
    pmsg = pmsg;

    //1.��ǰ��¼״̬

    return 0;
}

static UINT32 
ctrl_net_asyn_ack_onu_get_process(NET_MSG *pmsg)
{
    pmsg = pmsg;
    //1.��ǰ��¼״̬

    return 0;
}

static UINT32 
ctrl_net_asyn_ack_olt_set_process(NET_MSG *pmsg)
{
    pmsg = pmsg;

    //1.��ǰ��¼״̬
    
    return 0;
}

static UINT32 
ctrl_net_asyn_ack_olt_get_process(NET_MSG *pmsg)
{
    //for debug
    pmsg = pmsg;

    return 0;
}

static UINT32 
ctrl_net_asyn_ack_nw_set_process(NET_MSG *pmsg)
{
    //for debug
    pmsg = pmsg;

    //1.��ǰ��¼״̬

    return 0;
}

static UINT32 
ctrl_net_asyn_ack_nw_get_process(NET_MSG *pmsg)
{
    //for debug
    pmsg = pmsg;

    //1.��ǰ��¼״̬

    return 0;
}

#endif

#if DEF_FUNC("����ͬ��������Ϣ����ӿ�")

static UINT32 
ctrl_net_syn_req_drv_pull_process(NET_MSG *p_msg)
{
    /*�������ƺͱ�־msg,rc,rc_info,p_buf,len,ack�漰�꣬�����޸�*/
    unsigned int        rc;
    rc_info          rc_info = {.obj_id = OBJ_APP_ZK, .sub_obj_id.olt_id = MSG2SLOT(p_msg)};
    void                *p_buf = NULL;
    unsigned int        len = 0;
    
    NET_PHY_INFO    phy_info;
    //uint8_t             dev_id;
    //uint8_t             port_id;
    //uint16_t            onu_id;
    
    //��ȡ��Ϣ��������
    if ((rc = net_vif2phy(p_msg->vif_info, &phy_info)))
    {
      printf("%s %s %d error %d!!! \n", __FILE__, __FUNCTION__, __LINE__, rc);
      goto ack;
    }
    
    //dev_id = NET_PHY_INFO_TO_DEV_ID(phy_info);
    //port_id = NET_PHY_INFO_TO_ID(phy_info);
    //onu_id = NET_PHY_INFO_TO_ONU_ID(phy_info);

    switch(p_msg->cmd_info.cmd_id)
    {
        case NET_MSG_CMD_PROFILE_GET:
            {   
                char *buffer = net_malloc(&g_ctrl_net_msg_mp, MSG_BODY_LEN_TEST_PC);
                memset(buffer, 0x55, MSG_BODY_LEN_TEST_PC);
                p_buf = buffer;
                len = MSG_BODY_LEN_TEST_PC;
                sleep(1);//ӵ������
            }
            break;

        default:
            ERRNO_INFO2RC(rc_info, rc, APP_ZK_RC_PARAM_OUT_OF_RANGE);
            break;      
    }

    //no error return 0
    if (!(rc%RETURNCODE_BASE))rc = 0;

ack:
    if (rc)
    {
        printf("%s %s %d msg %d error %d\n", __FILE__, __FUNCTION__, __LINE__, p_msg->cmd_info.cmd_id, rc%RETURNCODE_BASE);
    }
    
    ctrl_net_msg_send(p_msg, p_buf, len, rc, NET_MSG_DRICTION_ACK);
    net_safe_free(p_buf);
    
    return 0;
}

#endif


#if DEF_FUNC("�����첽������Ϣ�ӿ�")

UINT32 
ctrl_net_asyn_operation(IN const NET_MSG_TYPE   msg_type,
                                IN const UINT64             vif_info, 
                                IN const NET_MSG_CMD    cmd_info, 
                                IN const void               *pvar, 
                                IN const UINT32             len)
{
    NET_MSG *msg = net_msg_pack(&g_ctrl_net_msg_mp, vif_info, cmd_info, pvar, len, len);

    if (!msg)
    {
        printf("%s %s %d error\n", __FILE__, __FUNCTION__, __LINE__);
        return APP_ZK_RC_MEM_ALLOCATION;
    }
    
    msg->msg_type  = msg_type;
    msg->direction = NET_MSG_DRICTION_REQUEST;
    msg->syn_flag  = NET_MSG_ASYN;
    
    if (ctrl_net_tx_mq_put(msg))
    {
        printf("%s %s %d error msg_type: %d\n", __FILE__, __FUNCTION__, __LINE__, msg->msg_type);
    }

    return 0;  
}

#endif

#if DEF_FUNC("����ͬ����Ϣ�ӿ�")

static UINT32 
ctrl_net_conn_msg_list_alloc(NET_CONN *conn)
{
    int i;
    NET_CONN_MSG *ptmsg;
    
    conn->msg_array = net_calloc(&g_ctrl_conn_msg_mp, conn->msg_num, 
                        sizeof(NET_CONN_MSG));
   
    if (!conn->msg_array)
        return APP_ZK_RC_MEM_ALLOCATION;

    ptmsg = conn->msg_array;
    for (i = 0; i < conn->msg_num; i++, ptmsg++)
    {
        UINT32 rc;

        TAILQ_INSERT_TAIL(&conn->free_req_list, ptmsg, l);
        
        rc = net_sem_create(&ptmsg->sem, 0);
        if (rc)
            return rc;
    }

    return 0;
}

static UINT32 
ctrl_net_conn_init(INT32 slot_id, NET_CONN **pconn)
{
    NET_CONN *conn;

    net_mutex_lock(&g_ctrl_conn_lock);
    conn = net_calloc(&g_ctrl_conn_mp, 1, sizeof(*conn));
    
    if (!conn)
    {
        net_mutex_unlock(&g_ctrl_conn_lock);
        return APP_ZK_RC_MEM_ALLOCATION;
    }

    snprintf(conn->name, sizeof(conn->name), "slot_%u", slot_id);
    TAILQ_INIT(&conn->free_req_list);
    TAILQ_INIT(&conn->msg_list);
    net_mutex_create(&conn->mutex);
    conn->msg_num = CTRL_NET_CONN_MSG_NUM;

    ctrl_net_conn_msg_list_alloc(conn);

    conn->connected = 1;
    conn->slot_id = slot_id;
    *pconn = conn;
    
    net_mutex_unlock(&g_ctrl_conn_lock);

    return 0;
}

static UINT32 
ctrl_net_conn_get_any(INT32 slot_id, NET_CONN **pconn)
{
    UINT32 err;

    if (!pconn)
    {
        return APP_ZK_RC_PARAM_NULL;
    }
    
    if (slot_id >= NET_MANAGE_MAX_SLOT)
    {
        return APP_ZK_RC_PARAM_OUT_OF_RANGE;
    }
    
    *pconn = g_ctrl_conn[slot_id];
    
    if (*pconn)
    {
        return 0;
    }
    
    err = ctrl_net_conn_init(slot_id, &g_ctrl_conn[slot_id]);
    *pconn = g_ctrl_conn[slot_id];
    
    return err;
}

UINT32 
ctrl_net_conn_get(INT32 slot_id, NET_CONN **pconn)
{
    return ctrl_net_conn_get_any(slot_id, pconn);
}

/* Send message and wait for restfse */
static UINT32 
ctrl_net_msg_syn_send(NET_MSG *pmsg)
{
    if (!pmsg)
    {
        return APP_ZK_RC_PARAM_NULL;
    }
    
    NET_CONN_MSG *pcmsg;
    NET_CONN     *pconn;
    unsigned int     err;

    //����slot��ȡconn
    err = ctrl_net_conn_get(MSG2SLOT(pmsg), &pconn);
    if (err)
    {
        printf("ctrl_net_conn_get error!!!\n");
        return err;
    }

    /* request under connection lock */
    net_mutex_lock(&pconn->mutex);

    pcmsg = net_conn_msg_get_free(&pconn->free_req_list); 
    if (!pcmsg)
    {
        net_mutex_unlock(&pconn->mutex);
        printf("ctrl_net_conn_msg_get_free error!!!\n");
        return APP_ZK_RC_PARAM_GET;
    }

    //ʱ�����Ϊ����У��ħ����
    pmsg->corr_tag = net_timestamp();

    //�������ڷ�����Ϣ����������ͬ����Ϣ����
    NET_MSG *p_autofree_msg = net_malloc(&g_ctrl_net_msg_mp, pmsg->len);
    if (!p_autofree_msg)
    {
        printf("%s %s %d net_malloc error!!!\r\n", __FILE__, __FUNCTION__, __LINE__);
        net_conn_msg_free(pcmsg, &pconn->free_req_list);
        return APP_ZK_RC_MEM_ALLOCATION;
    }
    
    memcpy(p_autofree_msg, pmsg, pmsg->len);
    
    //pmsg��conn ��Ϣ���ڽ���
    pcmsg->data = pmsg;

    //��������
    if ((err = ctrl_net_tx_mq_put(p_autofree_msg)))
    {
        printf("%s %s %d error!!!\n", __FILE__, __FUNCTION__, __LINE__);
        //pmsg ���ⲿ�ͷ�
        //pcmsg �ͷŵ����ж���
        net_conn_msg_free(pcmsg, &pconn->free_req_list);
        return err;
    }

    //����ȴ���Ϣ����
    TAILQ_INSERT_TAIL(&pconn->msg_list, pcmsg, l);

    net_mutex_unlock(&pconn->mutex);

    /* Wait for restfse or timeout*/
    if (net_sem_wait(&pcmsg->sem, pmsg->cmd_info.timeout))
    {
        //pmsg ���ⲿ�ͷ�
        //pcmsg ���ٹ�ע��Ҫ�ͷŵ����ж���
        net_mutex_lock(&pconn->mutex);
        
        TAILQ_REMOVE(&pconn->msg_list, pcmsg, l);
        net_conn_msg_free(pcmsg, &pconn->free_req_list);
        
        net_mutex_unlock(&pconn->mutex);
        
        return APP_ZK_RC_MSG_SYN_WAITE;
    }

    //�����õ���Ӧ����ж���
    net_mutex_lock(&pconn->mutex);
    net_conn_msg_free(pcmsg, &pconn->free_req_list);
    net_mutex_unlock(&pconn->mutex);

    return 0;
}

/*
�ӿڰ������ַ���ֵ��
1. ҵ���ӿ�  ����  state
2. �ӿ�                   ����  return
3. ����                   ����  param
*/
UINT32 
ctrl_net_syn_operation( IN const  NET_MSG_TYPE msg_type,
                                IN const  UINT64           vif_info, 
                                IN const  NET_MSG_CMD  cmd_info, 
                                IN const  void             *pvar, 
                                IN const  UINT32           pvar_len,
                                OUT void                   *param,
                                IN  const UINT32           param_len,
                                OUT UINT32                 *state)
{
    UINT32      syn_ack_len;
    NET_MSG *p_msg;
    UINT32      rc;
    rc_info rc_info = {.obj_id = OBJ_APP_ZK, .sub_obj_id.olt_id = (vif_info & 0xff)};

    //�����ж������Ƿ����
    if (!ctrl_net_state_working(vif_info & 0xff))
    {
        printf("ctrl remote slot %llu state is not working\r\n", vif_info & 0xff);
        return OBJ_APP_ZK_RC_OFFSET + APP_ZK_RC_NET_NOT_CONNECT;
    }
    
    if (!state)
    {
        ERRNO_INFO2RC(rc_info, rc, APP_ZK_RC_PARAM_NULL);
        return rc;
    }
    
    if ((param && 0 == param_len) || (NULL == param && param_len))
    {
        printf("%s %s %d param error\n", __FILE__, __FUNCTION__, __LINE__);
        ERRNO_INFO2RC(rc_info, rc, APP_ZK_RC_PARAM_OUT_OF_RANGE);
        return rc;
    }
    
    if ((NULL == param && 0 == param_len) || (param_len < pvar_len))
    {
        //��Ҫ���ε�����Ҫ���ز���|| ���νṹ���ڷ��ز���
        p_msg = net_msg_pack(&g_ctrl_net_msg_mp, vif_info, cmd_info, pvar, pvar_len, pvar_len);
    }
    else
    {
        //���νṹС�ڷ��ز���
        p_msg = net_msg_pack(&g_ctrl_net_msg_mp, vif_info, cmd_info, pvar, pvar_len, param_len);
    }
   
    if (!p_msg)
    {
        printf("%s %s %d error\n", __FILE__, __FUNCTION__, __LINE__);
        ERRNO_INFO2RC(rc_info, rc, APP_ZK_RC_MEM_ALLOCATION);
        return rc;
    }
    
    p_msg->msg_type  = msg_type;
    p_msg->direction = NET_MSG_DRICTION_REQUEST;
    p_msg->syn_flag  = NET_MSG_SYN;
    
    if ((rc = ctrl_net_msg_syn_send(p_msg)))
    {
        printf("%s %s %d error\n", __FILE__, __FUNCTION__, __LINE__);
        
        net_safe_free(p_msg);
        ERRNO_INFO2RC(rc_info, rc, rc);
        return rc;
    }
    
    *state = p_msg->state;

    if (!(*state))
    {
        if(param && param_len)
        {
            syn_ack_len = p_msg->len - NET_MSG_HEAD_LEN;
            
            if (param_len < syn_ack_len)
            printf("ctrl struct msg, body size too small!!!\n");

            memcpy(param, p_msg->body, (syn_ack_len > param_len ? param_len : syn_ack_len));
        }
    }
  
    net_safe_free(p_msg);

    return 0;  
}

#endif

