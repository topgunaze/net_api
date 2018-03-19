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

typedef struct treenode
{
    int  v;
    struct treenode *p_left;
    struct treenode *p_right;
}treenode;

int treedeep(treenode *p_root)
{
    if (p_root == NULL)
    {
        return 0;
    }

    int left, right;
    left = treedeep(p_root->p_left);
    right = treedeep(p_root->p_right);

    return 1 + (left > right ? left : right);
}

int isbalance(treenode *p_root, int *deep)
{
    if (p_root == NULL)
    {
        *deep = 0;
        return NODE_CTRL_RC_OK;
    }

    int left, right;
    if (isbalance(p_root->p_left, &left) && isbalance(p_root->p_right, &right))
    {
        int diff = left - right;
        if (diff >= 1 && diff < -1)
        {
            *deep = 1 + (left > right ? left : right);
            return NODE_CTRL_RC_OK;
        }

        return APP_CTRL_RC_ERROR;
    }

    return APP_CTRL_RC_ERROR;
}

int numberget(int *p_array, int len, int *p_num)
{
    if (len < 1 && !p_array)
    {
        return APP_CTRL_RC_ERROR;
    }

    int i;
    for (i = 0; i<len;i++)
    {
        *p_num ^= p_array[i];
    }

    return NODE_CTRL_RC_OK;
}

int bitis1(int num)
{
    
}

int numberget(int *p_array, int len, int *p_num1, int *p_num2)
{
    if (len < 2 && !p_array)
    {
        return APP_CTRL_RC_ERROR;
    }

    int i, xor_num, bit;
    for (i = 0; i < len; ++i)
    {
        xor_num ^= p_array[i];
    }

    bit = bitis1(xor_num);

    
    
    return NODE_CTRL_RC_OK;
}

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

    treenode t7 = {7, NULL, NULL};
    treenode t6 = {6, NULL, NULL};
    treenode t5 = {5, &t7, NULL};
    treenode t4 = {4, NULL, NULL};
    treenode t3 = {3, NULL, &t6};
    treenode t2 = {2, &t4, &t5};
    treenode t1 = {1, &t2, &t3};

    int deep;

    deep = treedeep(&t1);
    printf("t1 deep %d\r\n", deep);

    if (isbalance(&t1, &deep))
    {
      printf("t is balanse deep %d\r\n", deep);  
    }

    while(1)
    {
        sleep(1);
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

#if 0
    short ret_code;
    char  sendbuf[128];
    char  recvbuf[128];
    int   sendlen = sizeof(sendbuf);//recvlen = sizeof(recvbuf);
    
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

    if (ipc_if_engage_event(IPC_EVENT_INITED))
    {
        printf("ctrl ipc engage event IPC_EVENT_INITED fail\r\n");
    }

    if (ipc_if_engage_event(IPC_EVENT_ALARM_NOTIFY))
    {
        printf("ctrl ipc engage event IPC_EVENT_ALARM_NOTIFY fail\r\n");
    }
    
    while(1)
    {
        sleep(3);
        if (ipc_if_release_event(IPC_EVENT_CLI_START, NULL, 0))
        {
            printf("ctrl ipc release event IPC_EVENT_CLI_START fail\r\n");
        }

        sleep(3);
        bzero(sendbuf, sizeof(sendbuf));
        snprintf(sendbuf, sendlen, "this is event init msg form ctrl id_%d", IPC_EVENT_INITED);
        if (ipc_if_exe_cmd(MODULE_NODE1, IPC_EVENT_INITED, sendbuf, strlen(sendbuf), &ret_code))
        {
            printf("ctrl ipc exe cmd IPC_EVENT_INITED fail\r\n");
        }

        printf("ctrl ipc exe cmd IPC_EVENT_INITED ret %d\r\n", ret_code);

        sleep(3);
        bzero(sendbuf, sizeof(sendbuf));
        snprintf(sendbuf, sendlen, "this is event alarm notify msg form ctrl id_%d", IPC_EVENT_INITED);
        if (ipc_if_get_cmd_result(MODULE_NODE1, IPC_EVENT_ALARM_NOTIFY, 
                                  sendbuf, strlen(sendbuf), recvbuf, sizeof(recvbuf), &ret_code))
        {
            printf("ctrl ipc get cmd result IPC_EVENT_ALARM_NOTIFY fail\r\n");
        }

         printf("ctrl ipc get cmd result IPC_EVENT_ALARM_NOTIFY ret %d recbuf %s\r\n", ret_code, recvbuf);
    }
#endif

#if 0        

    struct thread_master *master;
    struct thread        thread;
    UINT8                module;

    /* 1.Prepare master thread. */
    master = thread_master_create ();

    cmd_init (1);
    vty_init (master);

    /* 4. Get configuration file. */
    vty_read_config (GCONFIG_FILENAME, GCONFIG_DIR);

    // Create a server, if you want to telnet vtyport
    /* Create VTY's socket .;VTY_TEST_PORT*/
#if 1

    vty_serv_sock (NULL, 0, GVTYSH_PATH);
#else
    //vty_user_init ();
    //vty_serv_sock (NULL, 5678, GVTYSH_PATH);
#endif

    //vty_set_config_write(gdistributed_config_write);

    //TELL_MONITOR("tfdevctrl");

    /* Execute each thread. */
    while (thread_fetch (master, &thread))
        thread_call (&thread); 
     

#if 0
    /* 2.Library initialization. */
    tflog_default = opentflog("GTF", GSYSLOG_MODULE_MAX_NUM, TRUE,
                                TFLOG_SYSLOG | TFLOG_CLI, LOG_LOCAL0, alarm_to_vty);

    for(module = 0; module < GSYSLOG_MODULE_MAX_NUM; module++)
        tflog_debug_mod_enable(module, 0);
#endif

    /* Not reached. */
    exit(1);
#endif
}

