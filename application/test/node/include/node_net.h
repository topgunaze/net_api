/**************************************************************
* 文件名称: 
* 文件描述:
* 版           本: 
* 修改历史:
*     <修改者>     <时间>      <版本 >     <描述>

**************************************************************/

#ifndef __NODE_NET__
#define __NODE_NET__

#include "adaptor_net_if.h"
#include "adaptor_types.h"

typedef enum
{
    NODE_DEVICE_STATE_INIT           = 0,
    NODE_NET_CONNECTION_COMPLETED    = 1,//建立与控制面连接
    NODE_DEVICE_STATE_READY          = 2,//建立与68620的连接
    NODE_DEVICE_STATE_STANDBY        = 3,//待命
    NODE_NET_WORK_SYNCHRONOUS        = 4,//控制面控制开工
    NODE_NET_STATE_WORKING           = 7,//工作
    NODE_NET_STATE_WORK_WAITED       = 8,//等待开工

    NODE_NET_STATE_NUM_OF
}NODE_NET_STATE_TYPE;

typedef struct
{
    NET_MUTUX          mutex;
    NET_PARA           net_param;   /*业务板连接信息*/
    NODE_NET_STATE_TYPE  state;       /*业务板网络状态*/
    uint8_t            slot_id;     /*业务板槽位信息*/

}NODE_NET_STATE;

//业务板板间消息并发处理线程数，
#define NODE_NET_MSG_CC_PRO_NUM   9
#define NODE_NET_MSG_SER_PRO_ID   8
#define NODE_NET_MSG_CC_PRO_MASK  0x07

#define NODE_NET_CONNECT_TIMEOUT  3

//请求消息处理错误返回码
//没有传参
#define NODE_NET_PROCESS_REQ_NO_PARAM_VERIFY()               NET_PROCESS_REQ_NO_PARAM_STRUCT(p_msg,rc_info,rc,NODE_CTRL_RC_PARAM_OUT_OF_RANGE,ack)
//有传参
#define NODE_NET_PROCESS_REQ_PARAM_STRUCT(type,p_name)       NET_PROCESS_REQ_PARAM_STRUCT(type,p_name,p_msg,rc_info,rc,NODE_CTRL_RC_PARAM_OUT_OF_RANGE,ack)
//有回传参数
#define NODE_NET_PROCESS_ACK_PARAM_STRUCT(type,p_name)       NET_PROCESS_ACK_PARAM_STRUCT(type,p_name,g_node_net_msg_mp,p_buf,len,rc_info,rc,NODE_CTRL_RC_MEM_ALLOCATION,ack)

#define NODE_NET_ERR_INFO2RC(obj,sub_obj,err_no)             ERR_INFO2RC(rc_info,obj,sub_obj,err_no,rc)


unsigned int node_net_ev_init(void);
unsigned int node_net_task_init(void);

unsigned int 
node_net_mempool_init(void);

uint32_t
node_net_syn_req_profile_get(const uint8_t slot_id,
                                MSG_PC_TEST *p_in,
                                MSG_PC_TEST *p_out);

uint32_t 
node_net_syn_operation(  IN const NET_MSG_TYPE  msg_type,
                                IN const  uint64_t         vif_info, 
                                IN const  NET_MSG_CMD  cmd_info, 
                                IN const  void             *pvar, 
                                IN const  uint32_t         pvar_len,
                                OUT void                   *param,
                                IN  const uint32_t         param_len,
                                OUT uint32_t               *state);

uint32_t 
node_net_msg_syn_ack_process(NET_MSG *pmsg);

void
node_net_state_set(int state);

uint32_t
node_net_state_get(void);

void
node_net_state_clear(int state);

uint32_t
node_net_state_check(const int state);

uint32_t
node_net_state_prepare_work(void);

uint32_t
node_net_state_is_work(void);

#endif
