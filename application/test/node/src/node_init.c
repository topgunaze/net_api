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

    key_t key = ipc_key_get("/etc", 20);

//#if 0
    int          msgq_id, i = 1;
    msgbuf       rx_buf;
    unsigned int size;

    msgq_id = ipc_mq_create(key);
    if (msgq_id == -1)
    {
        msgq_id = ipc_mq_open(key);
        if (msgq_id == -1)
        {
            return msgq_id;
        }
    }
    else
    {
        ipc_mq_size_set(msgq_id, 1024 * 10);
    }

    while(1)
    {
        bzero(&rx_buf, sizeof(msgbuf));
        rx_buf.mtype = i;

        ipc_mq_out(msgq_id, rx_buf.mtype, &rx_buf, sizeof(msgbuf), WAIT_FOREVER, &size);
        printf("node rec type %ld size %d text %s\r\n", rx_buf.mtype, size, rx_buf.mtext);
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
    int semid;

    semid = ipc_sem_create(key);
    if (semid == -1)
    {
        semid = ipc_sem_open(key);
        if (semid == -1)
        {
            return semid;
        }
    }

    ipc_sem_setval(semid, 1);

    while(1)
    {
        ipc_sem_p(semid);
        sleep(1);
        ipc_sem_v(semid);
    }

    ipc_sem_del(semid); 
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
    //等待开工
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
//模板同步测试
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

    //等待开工
    sleep(10);
   
    while(1)
    {
       sleep(1);
    }

#endif
}

