/**************************************************************
* �ļ�����: 
* �ļ�����:
* ��           ��: 
* �޸���ʷ:
*     <�޸���>     <ʱ��>      <�汾 >     <����>

**************************************************************/

#ifndef __FK_NET__
#define __FK_NET__

#include "adaptor_net_if.h"

typedef enum
{
    FK_DEVICE_STATE_INIT           = 0,
    FK_NET_CONNECTION_COMPLETED    = 1,//���������������
    FK_DEVICE_STATE_READY          = 2,//������68620������
    FK_DEVICE_STATE_STANDBY        = 3,//����
    FK_NET_WORK_SYNCHRONOUS        = 4,//��������ƿ���
    FK_NET_STATE_WORKING           = 7,//����
    FK_NET_STATE_WORK_WAITED       = 8,//�ȴ�����

    FK_STATE_NUM_OF
}FK_NET_STATE;

typedef struct
{
    NET_MUTUX     mutex;
    NET_PARA      net_param;   /*ҵ���������Ϣ*/
    FK_NET_STATE  state;       /*ҵ�������״̬*/
    uint8_t          slot_id;     /*ҵ����λ��Ϣ*/

}FK_STATE;

//ҵ�������Ϣ���������߳�����
#define FK_NET_MSG_CC_PRO_NUM   9
#define FK_NET_MSG_SER_PRO_ID   8
#define FK_NET_MSG_CC_PRO_MASK  0x07

#define FK_NET_CONNECT_TIMEOUT  3

//������Ϣ������󷵻���
//û�д���
#define FK_NET_PROCESS_REQ_NO_PARAM_VERIFY()               NET_PROCESS_REQ_NO_PARAM_STRUCT(p_msg,rc_info,rc,FK_ZK_RC_PARAM_OUT_OF_RANGE,ack)
//�д���
#define FK_NET_PROCESS_REQ_PARAM_STRUCT(type,p_name)       NET_PROCESS_REQ_PARAM_STRUCT(type,p_name,p_msg,rc_info,rc,FK_ZK_RC_PARAM_OUT_OF_RANGE,ack)
//�лش�����
#define FK_NET_PROCESS_ACK_PARAM_STRUCT(type,p_name)       NET_PROCESS_ACK_PARAM_STRUCT(type,p_name,g_fk_net_msg_mp,p_buf,len,rc_info,rc,FK_ZK_RC_MEM_ALLOCATION,ack)

#define FK_NET_ERR_INFO2RC(obj,sub_obj,err_no)             ERR_INFO2RC(rc_info,obj,sub_obj,err_no,rc)


unsigned int fk_net_ev_init(void);
unsigned int fk_net_task_init(void);

unsigned int 
fk_net_mempool_init(void);

uint32_t
fk_net_syn_req_profile_get(const uint8_t slot_id,
                                MSG_PC_TEST *p_in,
                                MSG_PC_TEST *p_out);

uint32_t 
fk_net_syn_operation(  IN const NET_MSG_TYPE  msg_type,
                                IN const  uint64_t         vif_info, 
                                IN const  NET_MSG_CMD  cmd_info, 
                                IN const  void             *pvar, 
                                IN const  uint32_t         pvar_len,
                                OUT void                   *param,
                                IN  const uint32_t         param_len,
                                OUT uint32_t               *state);

uint32_t 
fk_net_msg_syn_ack_process(NET_MSG *pmsg);

void
fk_net_state_set(int state);

uint32_t
fk_net_state_get(void);

void
fk_net_state_clear(int state);

uint32_t
fk_net_state_check(const int state);

uint32_t
fk_net_state_prepare_work(void);

uint32_t
fk_net_state_is_work(void);

#endif
