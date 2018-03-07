/**************************************************************
 * 文件名称:  
 * 作           者:  
 * 日           期:  
 * 文件描述:  
 * 版           本:  
 * 修改历史:
 * <修改者>   <时间>    <版本 >   <描述>
**************************************************************/

#include "ctrl_net.h"
#include "ipc_if.h"

#define GCONFIG_FILENAME    "test.conf"
#define GCONFIG_DIR         "/mnt"

UINT32 
ctrl_process_msg_task(
                char  *pMsgIn,
                UINT32 ulMsgLen,
                UINT8  ucMsgType,
                UINT8  ucSrcMo);


//1. vty命令行 与 配置保存
//2. snmp
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

    //key_t key = ipc_key_get("/etc", 20);

#if 0
    int     msgq_id;
    msgbuf  tx_buf;
    int     i = 1;

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
        bzero(&tx_buf, sizeof(msgbuf));
        tx_buf.mtype = i;
        snprintf(tx_buf.mtext, sizeof(tx_buf.mtext), "this is ctrl msg type %ld", tx_buf.mtype);
        ipc_mq_in(msgq_id, &tx_buf, sizeof(msgbuf), WAIT_FOREVER);
        sleep(1);

        ++i;
        i %= 5;
        if (i == 0)
        {
            i = 1;
        }
    }
#endif

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
        printf("this is ctrl\r\n");
        //sleep(1);
        ipc_sem_v(semid);
        sleep(0);
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

    printf("ctrl read : %s\r\n", p_text->buf);

    sleep(10);

    ipc_shm_unmap((void*)p_text);

    ipc_shm_del(shm_id);
#endif
    
    ipc_if_init();

    /* 向IPC 注册*/
    if (ipc_if_reg_module(MODULE_CTRL, "GTRL", (IPC_MSG_CALLBACK)ctrl_process_msg_task))
    {
        printf("ctrl reg ipc module fail\r\n");
    }

    //通知事件
    if (ipc_if_engage_event(IPC_EVENT_CLI_START))
    {
        printf("ctrl ipc engage event IPC_EVENT_CLI_START fail\r\n");
    }

    /* 发布初始化完成消息*/
    if (ipc_if_release_event(IPC_EVENT_INITED, NULL, 0))
    {
        printf("ctrl ipc release event IPC_EVENT_CLI_START fail\r\n");
    }

    while(1)
    {
        sleep(1);
        if (ipc_if_release_event(IPC_EVENT_CLI_START, NULL, 0))
        {
            printf("ctrl ipc release event IPC_EVENT_CLI_START fail\r\n");
        }
    }

    return 0;

#if 0        

    struct thread_master *master;
    struct thread thread;

    UINT8 module;


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

    TELL_MONITOR("tfdevctrl");

    /* Execute each thread. */
    while (thread_fetch (master, &thread))
        thread_call (&thread); 
     
    /* Not reached. */
    exit(1);
#endif
}


