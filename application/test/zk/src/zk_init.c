/**************************************************************
 * �ļ�����:  
 * ��           ��:  
 * ��           ��:  
 * �ļ�����:  
 * ��           ��:  
 * �޸���ʷ:
 * <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/

#if 0
#include "tf_common.h"
#include "sys_debug.h"
#include "tf_log.h"

#include "ipc_if.h"

#include "thread.h"
#include "zebra.h"
#include "command.h"
#include "global.h"
#include "memory.h"
#include "vtysh.h"

#include "tf_monitor_pub.h"
#include "tf_types.h"
#include "gtfCommon.h"
#endif

//#include "gtfDevInit.h"
#include "zk_net.h"

#define GTF_CONFIG_FILENAME    "test.conf"
#define GTF_CONFIG_DIR         "/mnt"

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
#ifdef TF_NET_USE_MP
        if (tf_ctrl_net_mempool_init())
        {
            exit(1);
        }
#endif
    
        tf_ctrl_net_state_init();
        tf_ctrl_net_map_init();
    
        if (tf_ctrl_net_task_init() || tf_ctrl_net_ev_init())
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
    ipc_if_reg_module(MODULE_GTF, "GTF", (IPC_MSG_CALLBACK)process_gtf_cmd_task);

    /* ��ע�¼�*/
    ipc_if_engage_event(IPC_EVENT_CLI_START);
    ipc_if_engage_event(IPC_EVENT_TF_PORT_UP_DOWN);
    //ipc_if_engage_event(IPC_EVENT_SNMP_AGENT_SWITCH);
    //ipc_if_engage_event(IPC_EVENT_SNMP_TF_PORT_SWITCH);

    /* 1.Prepare master thread. */
    master = thread_master_create ();

    /* 2.Library initialization. */
    tflog_default = opentflog("GTF", GTF_SYSLOG_MODULE_MAX_NUM, TRUE,
                                TFLOG_SYSLOG | TFLOG_CLI, LOG_LOCAL0, alarm_to_vty);

    for(module = 0; module < GTF_SYSLOG_MODULE_MAX_NUM; module++)
        tflog_debug_mod_enable(module, 0);
        
    cmd_init (1);
    vty_init (master);

    /* 3.modules initialization */

    //������ư����ݿ���ģ������ӿ�

    /* 4. Get configuration file. */
    vty_read_config (GTF_CONFIG_FILENAME, GTF_CONFIG_DIR);

    // Create a server, if you want to telnet vtyport
    /* Create VTY's socket .;VTY_TEST_PORT*/
#if 0

    vty_serv_sock (NULL, 0, GTF_VTYSH_PATH);
#else
    //vty_user_init ();
    //vty_serv_sock (NULL, 5678, GTF_VTYSH_PATH);
#endif

    vty_set_config_write(tf_gtf_distributed_config_write);

    /* ������ʼ�������Ϣ*/
    ipc_if_release_event(IPC_EVENT_TF_INITED, NULL, 0);

    TELL_MONITOR("tfdevctrl");

    /* Execute each thread. */
    while (thread_fetch (master, &thread))
        thread_call (&thread); 
     
    /* Not reached. */
    exit(1);
#endif
}


