/**************************************************************
* �ļ�����: 
* �ļ�����:
* ��           ��: 
* �޸���ʷ:
*     <�޸���>     <ʱ��>      <�汾 >     <����>

**************************************************************/

#ifndef __NODE_NET__
#define __NODE_NET__

#include "adaptor_net_if.h"
#include "adaptor_types.h"

typedef enum
{
    NODE_DEVICE_STATE_INIT           = 0,
    NODE_NET_CONNECTION_COMPLETED    = 1,//���������������
    NODE_DEVICE_STATE_READY          = 2,//������68620������
    NODE_DEVICE_STATE_STANDBY        = 3,//����
    NODE_NET_WORK_SYNCHRONOUS        = 4,//��������ƿ���
    NODE_NET_STATE_WORKING           = 7,//����
    NODE_NET_STATE_WORK_WAITED       = 8,//�ȴ�����

    NODE_NET_STATE_NUM_OF
}NODE_NET_STATE_TYPE;

typedef struct
{
    NET_MUTUX          mutex;
    NET_PARA           net_param;   /*ҵ���������Ϣ*/
    NODE_NET_STATE_TYPE  state;       /*ҵ�������״̬*/
    uint8_t            slot_id;     /*ҵ����λ��Ϣ*/

}NODE_NET_STATE;

//ҵ�������Ϣ���������߳�����
#define NODE_NET_MSG_CC_PRO_NUM   9
#define NODE_NET_MSG_SER_PRO_ID   8
#define NODE_NET_MSG_CC_PRO_MASK  0x07

#define NODE_NET_CONNECT_TIMEOUT  3

//������Ϣ������󷵻���
//û�д���
#define NODE_NET_PROCESS_REQ_NO_PARAM_VERIFY()               NET_PROCESS_REQ_NO_PARAM_STRUCT(p_msg,rc_info,rc,NODE_CTRL_RC_PARAM_OUT_OF_RANGE,ack)
//�д���
#define NODE_NET_PROCESS_REQ_PARAM_STRUCT(type,p_name)       NET_PROCESS_REQ_PARAM_STRUCT(type,p_name,p_msg,rc_info,rc,NODE_CTRL_RC_PARAM_OUT_OF_RANGE,ack)
//�лش�����
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
