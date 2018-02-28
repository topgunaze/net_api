/**************************************************************
 * �ļ�����:  
 * ��           ��:  
 * ��           ��:  
 * �ļ�����:  
 * ��           ��:  
 * �޸���ʷ:
 * <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/

//#include "gtfDevInit.h"
#include "zk_net.h"

#define GCONFIG_FILENAME    "test.conf"
#define GCONFIG_DIR         "/mnt"

//1. vty������ �� ���ñ���
//2. snmp
//3. ���ͨ��
//4. ����ͨ��
//5. log

/**************************************************************
 * ��������:
 * ��           ��:
 * ��           ��:
 *
 * ��������:
 * �������:
 * �������:
 * ������ֵ:
                            RTN_OK        : �ɹ�
                            RTN_ERROR   : ʧ��
 * ��           ��:
**************************************************************/

int
main(void)
{

        //������������
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

        while(1)
        {
            sleep(1);
        }

        return 0;

#if 0        

    struct thread_master *master;
    struct thread thread;

    UINT8 module;

    /* GTF ģ��IPC ��ʼ��*/
    ipc_if_init();

    /* ��IPC ע��*/
    ipc_if_reg_module(MODULE_GTF, "GTF", (IPC_MSG_CALLBACK)process_gcmd_task);

    /* ��ע�¼�*/
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

    //������ư����ݿ���ģ������ӿ�

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

    /* ������ʼ�������Ϣ*/
    ipc_if_release_event(IPC_EVENT_INITED, NULL, 0);

    TELL_MONITOR("tfdevctrl");

    /* Execute each thread. */
    while (thread_fetch (master, &thread))
        thread_call (&thread); 
     
    /* Not reached. */
    exit(1);
#endif
}


