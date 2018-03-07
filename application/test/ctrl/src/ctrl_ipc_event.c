/**************************************************************
 * 文件名称:  gtfDevIpcAdapter.c
 * 作           者:  keith.gong
 * 日           期:  2015.05.30
 * 文件描述:  GTF  IPC  适配层
 * 版           本:  V1.00
 * 修改历史:  
 *     <修改者>   <时间>    <版本 >   <描述>
**************************************************************/

#include "ipc_if.h"
#include "adaptor_net_if.h"
#include "adaptor_types.h"

UINT32 
ctrl_process_msg_task(
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
                    MY_PRINT("CTRL rec cli start msg\r\n");
                    break;

                /* 终端调试开关*/
                case IPC_EVENT_TERM_DEBUG_NOTICE:
                {
                    UINT8 enable = FALSE;
                    
                    if (dataLen != sizeof(enable)) {
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
            
            MY_PRINT("ctrl rec msg cmd, msgid:%d\r\n", pMsg->MsgHead.MsgID);
            
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


