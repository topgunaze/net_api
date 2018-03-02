/**************************************************************
 * 文件名称:  
 * 文件描述: 
 * 版           本:  
 * 修改历史:
 *     <修改者>     <时间>      <版本 >     <描述>
**************************************************************/

#include "fk_init.h"
#include "fk_net.h"

THREAD_POOL phy_proc_thread_pool;

static int
fk_secondary_init(void)
{
    int     rc;

    /* 初始化数据库*/
    rc = fk_db_init();
    if(rc)
    {
        printf("tfdrv onu db init failed, rc: %d.\r\n", rc);
        return -1;
    }

    //fk_thread_pool_init(&phy_proc_thread_pool, 16);

    return rc;
}

//#endif
//extern uint8_t              g_fk_packet_test;
//extern FK_STATE    g_fk_state;

typedef struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1024];    /* message data */
}msgbuf;

int
main(int argc, char ** argv)
{
    int    rc;
    int    sec = 0;
    
    rc = fk_net_task_init();
    if(rc)
    {
        printf("tfdrv net tast init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = fk_net_ev_init();
    if(rc)
    {
        printf("tfdrv net ev init failed, rc: %d.\r\n", rc);
        return -1;
    }
    
    //模拟设备正常工作
    fk_net_state_set(FK_DEVICE_STATE_STANDBY);
    while(!fk_net_state_prepare_work())
    {
        sleep(1);

        if(++sec > 60)
            return -1;
    }

    //TELL_MONITOR("tfdrv");

    printf("tfdrv connected to mc and ready\r\n");

    rc = fk_secondary_init();
    if(rc)
    {
        printf("tfdrv secondary init failed, rc: %d.\r\n", rc);
        return -1;
    }

    //for system v msgq
    int tx_msgq_id;
    //int rx_msgq_id;

//#if 0
    if (net_systemv_mq_create(&tx_msgq_id, MQ_CTRL_TX_MSG, 1024*10))
    {
        printf("MQ_FK_TX_MSG msg queue error\r\n");
    }
//#endif    
    
#if 0
    if (net_systemv_mq_create(&rx_msgq_id, MQ_CTRL_RX_SYN_REQ_MSG, 1024*10))
    {
        printf("MQ_FK_TX_MSG msg queue error\r\n");
    }
#endif

    //msgbuf       tx_buf;
    msgbuf       rc_buf;
    unsigned int size;
    int          i = 1;
    
    while(1)
    {
#if 0
        bzero(&tx_buf, sizeof(msgbuf));
        tx_buf.mtype = i;

        snprintf(tx_buf.mtext, sizeof(tx_buf.mtext), "this is zk time %ld\r\n", tx_buf.mtype);

        net_systemv_mq_in(tx_msgq_id, &tx_buf, sizeof(msgbuf), NO_WAIT);//WAIT_FOREVER
#endif
        bzero(&rc_buf, sizeof(msgbuf));
        rc_buf.mtype = i;

        sleep(1);
        net_systemv_mq_out(tx_msgq_id, rc_buf.mtype, &rc_buf, sizeof(rc_buf), WAIT_FOREVER, &size);//NO_WAIT

        printf("rec type %ld text %s, size %d\r\n", rc_buf.mtype, rc_buf.mtext, size);

        ++i;
        i %= 5;
        if (i == 0)
        {
            i = 1;
        }
    }

    

    while(1)
    {
        sleep(1);
    }
//#endif
#if 0    
    int rc;
    
    rc = fk_net_task_init();
    if(rc)
    {
       printf("tfdrv net tast init failed, rc: %d.\r\n", rc);
       return -1;
    }

    rc = fk_net_ev_init();
    if(rc)
    {
       printf("tfdrv net ev init failed, rc: %d.\r\n", rc);
       return -1;
    }

    fk_net_state_set(FK_DEVICE_STATE_READY);

    int i;
    MSG_PC_TEST test_in;
    MSG_PC_TEST test_out;

    memset(&test_in, 0x55, sizeof(MSG_PC_TEST));
    //等待开工
    sleep(10);
       
    while(1)
    {
       sleep(1);

       if (!g_fk_packet_test)
       {
           printf("tfdrv %d begin tx syn profile current time(us): %d\r\n", g_fk_state.slot_id, net_timestamp());
           for (i = 0; i < 5; i++)//MSG_COUNT_PC_TEST
           {
               if (fk_net_syn_req_profile_get(g_fk_state.slot_id, &test_in, &test_out))
               {
                   printf("drv %d send syn error\r\n", g_fk_state.slot_id);
               }

               if (memcmp(&test_in, &test_out, sizeof(MSG_PC_TEST)))
               {
                   printf("drv %d send syn not match\r\n", g_fk_state.slot_id);
               }
           }

           printf("drv %d send test msg %d finish %d !!!\r\n", g_fk_state.slot_id, MSG_COUNT_PC_TEST, net_timestamp());
       }


    }
#endif
//模板同步测试
#if 0

    int rc;

    rc = fk_net_task_init();
    if(rc)
    {
       printf("tfdrv net tast init failed, rc: %d.\r\n", rc);
       return -1;
    }

    rc = fk_net_ev_init();
    if(rc)
    {
       printf("tfdrv net ev init failed, rc: %d.\r\n", rc);
       return -1;
    }

    rc = fk_if_dbaprofile_init();
    if(rc)
    {
        printf("tfdrv dba profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = fk_if_lineprofile_init();
    if(rc)
    {
        printf("tfdrv line profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = fk_if_srvprofile_module_init();
    if(rc)
    {
        printf("tfdrv service profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = fk_if_pots_profile_module_init();
    if(rc)
    {
        printf("tfdrv pots profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = fk_if_digitmap_profile_module_init();
    if(rc)
    {
        printf("tfdrv digitmap profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = fk_if_sipagent_profile_module_init();
    if(rc)
    {
        printf("tfdrv sipagent profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = fk_if_sipright_profile_module_init();
    if(rc)
    {
        printf("tfdrv sipright profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    rc = fk_if_traffic_profile_init();
    if(rc)
    {
        printf("tfdrv traffic profile init failed, rc: %d.\r\n", rc);
        return -1;
    }

    fk_net_state_set(FK_DEVICE_STATE_READY);

    //等待开工
    sleep(10);
   
    while(1)
    {
       sleep(1);
    }

#endif
}

