/**************************************************************
 * 文件名称:  
 * 作           者:  
 * 日           期:  
 * 文件描述:  
 * 版           本:  
 * 修改历史:
 * <修改者>   <时间>    <版本 >   <描述>
**************************************************************/

#include "zk_net.h"

#define GCONFIG_FILENAME    "test.conf"
#define GCONFIG_DIR         "/mnt"

//1. vty命令行 与 配置保存
//2. snmp
//3. 板间通信
//4. 本地通信
//5. log

/**************************************************************
 * 函数名称:
 * 作           者:
 * 日           期:
 *
 * 函数功能:
 * 输入参数:
 * 输出参数:
 * 返回数值:
                            RTN_OK        : 成功
                            RTN_ERROR   : 失败
 * 其           它:
**************************************************************/

typedef struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1024];    /* message data */
}msgbuf;

int
main(void)
{
    //启动网络任务
#ifdef NET_USE_MP
    if (ctrl_net_mempool_init())
    {
        exit(1);
    }
#endif
    
    ctrl_net_state_init();
    ctrl_net_map_init();
    
    if (ctrl_net_task_init() || ctrl_net_ev_init())
    {
        exit(1);
    }

    //for system v msgq
    int tx_msgq_id;
    int rx_msgq_id;
    
    if (net_systemv_mq_create(&tx_msgq_id, MQ_CTRL_TX_MSG, 1024*10))
    {
        printf("MQ_FK_TX_MSG msg queue error\r\n");
    }
    
    if (net_systemv_mq_create(&rx_msgq_id, MQ_CTRL_RX_SYN_REQ_MSG, 1024*10))
    {
        printf("MQ_FK_TX_MSG msg queue error\r\n");
    }

    msgbuf       tx_buf;
    msgbuf       rc_buf = {0};
    //unsigned int size;
    int          i = 1;
    
    while(1)
    {
        bzero(&tx_buf, sizeof(msgbuf));
        tx_buf.mtype = i;

        snprintf(tx_buf.mtext, sizeof(tx_buf.mtext), "this is zk time %ld\r\n", tx_buf.mtype);

        net_systemv_mq_in(tx_msgq_id, &tx_buf, sizeof(msgbuf), WAIT_FOREVER);//NO_WAIT
        sleep(1);
        net_systemv_mq_out(rx_msgq_id, tx_buf.mtype, &rc_buf, sizeof(rc_buf), NO_WAIT, &size);//WAIT_FOREVER

        //printf("rec type %ld text %s, size %d\r\n", rc_buf.mtype, rc_buf.mtext, size);

        ++i;
        i %= 5;
        if (i == 0)
        {
            i = 1;
        }
    }

    return 0;

#if 0        

    struct thread_master *master;
    struct thread thread;

    UINT8 module;

    /* GTF 模块IPC 初始化*/
    ipc_if_init();

    /* 向IPC 注册*/
    ipc_if_reg_module(MODULE_GTF, "GTF", (IPC_MSG_CALLBACK)process_gcmd_task);

    /* 关注事件*/
    ipc_if_engage_event(IPC_EVENT_CLI_START);
    ipc_if_engage_event(IPC_EVENT_PORT_UP_DOWN);
    //ipc_if_engage_event(IPC_EVENT_SNMP_AGENT_SWITCH);
    //ipc_if_engage_event(IPC_EVENT_SNMP_PORT_SWITCH);

    /* 1.Prepare master thread. */
    master = thread_master_create ();

    /* 2.Library initialization. */
    tflog_default = opentflog("GTF", GSYSLOG_MODULE_MAX_NUM, TRUE,
                                TFLOG_SYSLOG | TFLOG_CLI, LOG_LOCAL0, alarm_to_vty);

    for(module = 0; module < GSYSLOG_MODULE_MAX_NUM; module++)
        tflog_debug_mod_enable(module, 0);
        
    cmd_init (1);
    vty_init (master);

    /* 3.modules initialization */

    //适配控制板数据库与模块网络接口

    /* 4. Get configuration file. */
    vty_read_config (GCONFIG_FILENAME, GCONFIG_DIR);

    // Create a server, if you want to telnet vtyport
    /* Create VTY's socket .;VTY_TEST_PORT*/
#if 0

    vty_serv_sock (NULL, 0, GVTYSH_PATH);
#else
    //vty_user_init ();
    //vty_serv_sock (NULL, 5678, GVTYSH_PATH);
#endif

    vty_set_config_write(gdistributed_config_write);

    /* 发布初始化完成消息*/
    ipc_if_release_event(IPC_EVENT_INITED, NULL, 0);

    TELL_MONITOR("tfdevctrl");

    /* Execute each thread. */
    while (thread_fetch (master, &thread))
        thread_call (&thread); 
     
    /* Not reached. */
    exit(1);
#endif
}


