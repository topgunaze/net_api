/**************************************************************
 * 文件名称:  
 * 文件描述: 
 * 版           本:  
 * 修改历史:
 *     <修改者>     <时间>      <版本 >     <描述>
**************************************************************/

#include "node_init.h"
#include "node_net.h"
#include "ipc_if.h"

THREAD_POOL phy_proc_thread_pool;

static int
node_secondary_init(void)
{
    int     rc;

    /* 初始化数据库*/
    rc = node_db_init();
    if(rc)
    {
        printf("tfdrv onu db init failed, rc: %d.\r\n", rc);
        return -1;
    }

    //node_thread_pool_init(&phy_proc_thread_pool, 16);

    return rc;
}

//#endif
//extern uint8_t              g_node_packet_test;
//extern NODE_STATE    g_node_state;

void test_release_cb(void)
{
    printf("exec test_release_cb\r\n");
}

void worker_handler(const uint32_t key, const uint32_t arg1)
{
    printf("key %d, arg1 %d\r\n", key, arg1);
    sleep(1);
}

UINT32 
node_process_msg_task(
                char  *pMsgIn,
                UINT32 ulMsgLen,
                UINT8  ucMsgType,
                UINT8  ucSrcMo)
{
    IPC_APP_MSG      *pMsg;
    IPC_EVENT_R_INFO *pEvent;
    INT32            dataLen = 0;
    short            rc = 0; /* message retrun value */

    do
    {
        if (ucMsgType == IPC_EVENT_RELEASE) 
        {
            dataLen = ulMsgLen - sizeof(IPC_EVENT_R_INFO);
            if (dataLen < 0) 
            {
                MY_ERROR("Receive bad IPC_EVENT_RELEASE message! Message length %d is less than %d.\r\n",
                       dataLen, sizeof(IPC_EVENT_R_INFO));
                break;
            }
            
            pEvent = (IPC_EVENT_R_INFO*)pMsgIn;
        
            switch (pEvent->EventMsgHead.ucEventId)
            {
                case IPC_EVENT_CLI_START:
                    MY_PRINT("NODE rec cli start msg\r\n");
                    break;

                case IPC_EVENT_INITED:
                    MY_PRINT("NODE rec event init msg\r\n");
                    break;

                /* 终端调试开关*/
                case IPC_EVENT_TERM_DEBUG_NOTICE:
                {
                    UINT8 enable = FALSE;
                    
                    if (dataLen != sizeof(enable)) 
                    {
                        MY_ERROR("receive bad IPC_EVENT_TERM_DEBUG_NOTICE msg!message length %d is not equal size of enable:%d.\r\n",
                                        dataLen,sizeof(enable));
                        break;
                    }
                    
                    enable = *(UINT8*)pEvent->data;
                    //tflog_debug_term_enable(enable);
                    break;
                }

                /* 子代理开关*/
                case IPC_EVENT_SNMP_AGENT_SWITCH:
                {                     
                    if (dataLen != sizeof(UINT8)) 
                    {
                        MY_ERROR("Receive bad IPC_EVENT_SNMP_AGENT_SWITCH message! Message length %d is not equal to %d.\r\n",
                                       dataLen, sizeof(UINT8));
                        break;
                    }
                    
                    //snmpAgentEnable = *(UINT8 *)(pEvent->data);
                    /*if(snmpAgentEnable)
                        tf_snmpd_start();
                    else
                        tf_snmpd_stop();*/
                    
                    break;
                }

                default:
                    break;
            }
        } 
        else if (ucMsgType == IPC_MSG_CMD) 
        {
            dataLen = ulMsgLen - sizeof(IPC_APP_MSG);
            if (dataLen < 0)
            {
                MY_ERROR("Receive bad IPC_MSG_CMD msg! message length %d is less than size of IPC_APP_MSG:%d.\r\n",
                         dataLen, sizeof(IPC_APP_MSG));
                goto end;
            }
            
            pMsg = (IPC_APP_MSG*)pMsgIn;
               
            switch(pMsg->MsgHead.MsgID)
            {
#if 0                
                case IPC_SNMP_TF_PORT_STATE_SET_MSG:
                {
                    snmpPonPortState_t* pPonPortState = NULL;
                    UINT16 gtfOltPortId = 0;
                    UINT16 state         = 0;
                    
                    if (dataLen < sizeof(snmpPonPortState_t)){
                        MY_ERROR("receive bad IPC_SNMP_TF_PORT_STATE_SET_MSG msg!message length %d is not equal size of Port_Attr_t:%d.\r\n",dataLen,sizeof(snmpPonPortState_t));
                        goto end;
                    }    
                    
                    pPonPortState = (snmpPonPortState_t *)(pMsg->data);
                    gtfOltPortId = pPonPortState->gtfOltPortId;
                    state         = pPonPortState->state;

                    if(RTN_OK != tf_snmp_gtf_port_state_set(gtfOltPortId, state))
                    {
                        MY_ERROR("Called _tf_snmp_gtf_port_state_set() error.\r\n");
                        rc = -1;
                    }
                }
                break;
#endif         
                case IPC_EVENT_INITED:
                    {
                        char *p;

                        /*if (dataLen < 128)
                        {
                            MY_ERROR("receive bad IPC_EVENT_INITED msg!message length %d is not equal size of Port_Attr_t:%d.\r\n",dataLen, 128);
                            goto end;
                        }*/    
                    
                        p = (char*)(pMsg->data);
                        /*if(RTN_OK != tf_snmp_gtf_port_state_set(gtfOltPortId, state))
                        {
                            MY_ERROR("Called _tf_snmp_gtf_port_state_set() error.\r\n");
                            rc = -1;
                        }*/
                        printf("node rec cmd IPC_EVENT_INITED %s\r\n", p);
                        rc = 0;
                    }
                    break;

                case IPC_EVENT_ALARM_NOTIFY:
                    {
                        char *p;
                        char sendbuf[128] = {"this is node send\r\n"};

                        /*if (dataLen < 128)
                        {
                            MY_ERROR("receive bad IPC_EVENT_ALARM_NOTIFY msg!message length %d is not equal size of Port_Attr_t:%d.\r\n",dataLen, 128);
                            goto end;
                        }*/    
                    
                        p = (char*)(pMsg->data);
                        /*if(RTN_OK != tf_snmp_gtf_port_state_set(gtfOltPortId, state))
                        {
                            MY_ERROR("Called _tf_snmp_gtf_port_state_set() error.\r\n");
                            rc = -1;
                        }*/
                        printf("node rec cmd IPC_EVENT_ALARM_NOTIFY %s\r\n", p);
                        rc = 0;
                        ipc_if_send_ack(ucSrcMo, pMsgIn, rc, sendbuf, sizeof(sendbuf)); /*rc作为错误值返回给用户*/
                        goto end;
                    }
                    break;
                    
                default:
                    rc = -1;
                    break;
            }
            
            ipc_if_send_ack(ucSrcMo, pMsgIn, rc, NULL, 0); /*rc作为错误值返回给用户*/
        }       
    }while(0);

end:
    ipc_if_free(pMsgIn);

    return 0;
}

int
main(int argc, char ** argv)
{
    int    rc;
    int    sec = 0;
    
    rc = node_net_task_init();
    if(rc)
    {
        printf("tfdrv net tast init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = node_net_ev_init();
    if(rc)
    {
        printf("tfdrv net ev init failed, rc: %d.\r\n", rc);
        return -1;
    }
    
    //模拟设备正常工作
    node_net_state_set(NODE_DEVICE_STATE_STANDBY);
    while(!node_net_state_prepare_work())
    {
        sleep(1);

        if(++sec > 60)
            return -1;
    }

    //TELL_MONITOR("tfdrv");

    printf("tfdrv connected to mc and ready\r\n");

    rc = node_secondary_init();
    if(rc)
    {
        printf("tfdrv secondary init failed, rc: %d.\r\n", rc);
        return -1;
    }

	shm_test();

    while(1)
    {
        sleep(1);
    }

#if 0
    ipc_mq_recv_test();
#endif

#if 0
    ipc_sem_sync_test2();
#endif

#if 0
    int shm_id;
    
    shm_id = ipc_shm_create(key, sizeof(shm_struct));
    if (shm_id == -1)
    {
        shm_id = ipc_shm_open(key);
        if (shm_id == -1)
        {
            return shm_id;
        }
    }

    shm_struct *p_text; 

    p_text = ipc_shm_map(shm_id);

    snprintf(p_text->buf, sizeof(p_text->buf), "this is node write\r\n");

    sleep(10);

    ipc_shm_unmap((void*)p_text);

    ipc_shm_del(shm_id);
#endif

#if 0
    ipc_if_init();

    /* 向IPC 注册模块*/
    if (ipc_if_reg_module(MODULE_NODE1, "NODE1", (IPC_MSG_CALLBACK)node_process_msg_task))
    {
        printf("node reg ipc module fail\r\n");
    }

    //注册事件
    if (ipc_if_engage_event(IPC_EVENT_CLI_START))
    {
        printf("node ipc engage event IPC_EVENT_CLI_START fail\r\n");
    }

    if (ipc_if_engage_event(IPC_EVENT_INITED))
    {
        printf("node ipc engage event IPC_EVENT_INITED fail\r\n");
    }

    if (ipc_if_engage_event(IPC_EVENT_ALARM_NOTIFY))
    {
        printf("ctrl ipc engage event IPC_EVENT_ALARM_NOTIFY fail\r\n");
    }

    while(1)
    {
        sleep(1);
        /*
        if (ipc_if_release_event(IPC_EVENT_CLI_START, NULL, 0))
        {
            printf("node ipc release event IPC_EVENT_CLI_START fail\r\n");
        }

        sleep(1);
        if (ipc_if_release_event(IPC_EVENT_INITED, NULL, 0))
        {
            printf("node ipc release event IPC_EVENT_CLI_START fail\r\n");
        }*/
    }
#endif

    return 0 ;

   
#if 0
    THREAD_POOL pt_pool;

    bzero(&pt_pool, sizeof(THREAD_POOL));
    
    thread_pool_init(&pt_pool, 2);
    thread_pool_worker_type_set_limit(&pt_pool, 0, 2);
    thread_pool_worker_type_set_release_cb(&pt_pool, 0, test_release_cb);

    int worker_id, worker_node;

    printf("add woker \r\n");
    for (worker_id = 0; worker_id < MAX_WORKER_NUM; ++worker_id)
    {
        for (worker_node = 0; worker_node < MAX_NODE_PER_WORKER; ++worker_node)
        {
            thread_pool_add_worker(&pt_pool, worker_handler, WORKER_TYPE_COMMON, worker_id, worker_node);
        }
    }

    printf("add woker over\r\n");
    
    while(1)
    {
        sleep(1);
    }
#endif
}




