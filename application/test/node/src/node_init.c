/**************************************************************
 * �ļ�����:  
 * �ļ�����: 
 * ��           ��:  
 * �޸���ʷ:
 *     <�޸���>     <ʱ��>      <�汾 >     <����>
**************************************************************/

#include "node_init.h"
#include "node_net.h"
#include "ipc_if.h"

THREAD_POOL phy_proc_thread_pool;

static int
node_secondary_init(void)
{
    int     rc;

    /* ��ʼ�����ݿ�*/
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
    
    //ģ���豸��������
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

//#if 0
    int ct_nr_msgq_id;
    int cr_nt_msgq_id;

    if (net_systemv_mq_create(&ct_nr_msgq_id, MQ_CTRL_TX_NODE_RX_MSG, 1024*10))
    {
        printf("MQ_CTRL_TX_NODE_RX_MSG msg queue error\r\n");
    }
    

    if (net_systemv_mq_create(&cr_nt_msgq_id, MQ_CTRL_RX_NODE_TX_MSG, 1024*10))
    {
        printf("MQ_CTRL_RX_NODE_TX_MSG msg queue error\r\n");
    }

    msgbuf       tx_buf;
    msgbuf       rc_buf;
    unsigned int size;
    int          i = 1;
    
    while(1)
    {
        bzero(&tx_buf, sizeof(msgbuf));
        tx_buf.mtype = i;

        snprintf(tx_buf.mtext, sizeof(tx_buf.mtext), "this is node type %ld\r\n", tx_buf.mtype);

        //net_systemv_mq_in(cr_nt_msgq_id, &tx_buf, sizeof(tx_buf.mtext), WAIT_FOREVER);//WAIT_FOREVER
        net_systemv_mq_in(cr_nt_msgq_id, &tx_buf, sizeof(tx_buf), WAIT_FOREVER);//WAIT_FOREVER

        bzero(&rc_buf, sizeof(msgbuf));
        rc_buf.mtype = i;

        //net_systemv_mq_out(ct_nr_msgq_id, rc_buf.mtype, &rc_buf.mtext, sizeof(rc_buf.mtext), WAIT_FOREVER, &size);//NO_WAIT
        net_systemv_mq_out(ct_nr_msgq_id, rc_buf.mtype, &rc_buf, sizeof(rc_buf), WAIT_FOREVER, &size);//NO_WAIT

        printf("rec type %ld text %s, size %d\r\n", rc_buf.mtype, rc_buf.mtext, size);
        sleep(1);

        ++i;
        i %= 5;
        if (i == 0)
        {
            i = 1;
        }
    }
//#endif

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

#if 0    
    int rc;
    
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

    node_net_state_set(NODE_DEVICE_STATE_READY);

    int i;
    MSG_PC_TEST test_in;
    MSG_PC_TEST test_out;

    memset(&test_in, 0x55, sizeof(MSG_PC_TEST));
    //�ȴ�����
    sleep(10);
       
    while(1)
    {
       sleep(1);

       if (!g_node_packet_test)
       {
           printf("tfdrv %d begin tx syn profile current time(us): %d\r\n", g_node_state.slot_id, net_timestamp());
           for (i = 0; i < 5; i++)//MSG_COUNT_PC_TEST
           {
               if (node_net_syn_req_profile_get(g_node_state.slot_id, &test_in, &test_out))
               {
                   printf("drv %d send syn error\r\n", g_node_state.slot_id);
               }

               if (memcmp(&test_in, &test_out, sizeof(MSG_PC_TEST)))
               {
                   printf("drv %d send syn not match\r\n", g_node_state.slot_id);
               }
           }

           printf("drv %d send test msg %d finish %d !!!\r\n", g_node_state.slot_id, MSG_COUNT_PC_TEST, net_timestamp());
       }


    }
#endif
//ģ��ͬ������
#if 0

    int rc;

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

    rc = node_if_dbaprofile_init();
    if(rc)
    {
        printf("tfdrv dba profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = node_if_lineprofile_init();
    if(rc)
    {
        printf("tfdrv line profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = node_if_srvprofile_module_init();
    if(rc)
    {
        printf("tfdrv service profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = node_if_pots_profile_module_init();
    if(rc)
    {
        printf("tfdrv pots profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = node_if_digitmap_profile_module_init();
    if(rc)
    {
        printf("tfdrv digitmap profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = node_if_sipagent_profile_module_init();
    if(rc)
    {
        printf("tfdrv sipagent profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = node_if_sipright_profile_module_init();
    if(rc)
    {
        printf("tfdrv sipright profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = node_if_traffic_profile_init();
    if(rc)
    {
        printf("tfdrv traffic profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    node_net_state_set(NODE_DEVICE_STATE_READY);

    //�ȴ�����
    sleep(10);
   
    while(1)
    {
       sleep(1);
    }

#endif
}

