/**************************************************************
 * �ļ�����:  tf_queue.c
 * ��           ��:  keith.gong
 * ��           ��:  2014.12.27
 * �ļ�����:  ϵͳ��Ϣ���й���
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/


#include <unistd.h>
#include <sys/msg.h>
#include <errno.h>

#include "tf_types.h"
#include "tf_queue.h"

#include <stdio.h>
#define QUEUE_PRINTF(fmt, arg...)  printf(fmt,##arg)

/* ��¼ÿ����Ϣռ�������0:δ��ʹ��1:�Ѿ���ʹ�� */
static UINT8 queue_used_flag[SYS_QUEUE_MAX_NUM];


/**************************************************************
 * ��������: tf_queue_create
 * ��           ��: keith.gong
 * ��           ��: 2014��12��27��
 *
 * ��������: ����������Ϣ����
 * �������: 
                            p_queue_id
                            sub_key  : 0 ~ (SYS_QUEUE_MAX_NUM-1)
                            queue_size
 * �������: 
 * ������ֵ: 
                            OK : 
                            ERROR : 
 * ��           ��:  
**************************************************************/
RTN_STATUS 
tf_queue_create (
                INT32 *p_queue_id, 
                SYS_QUEUE_SUB_KEY_E sub_key, 
                UINT32 queue_size)
{
	int tmp_skt;
	key_t q_key;
	struct msqid_ds msg_ds;

	if(p_queue_id == NULL) 
	{
		QUEUE_PRINTF("Create queue failed cause some parameter is NULL.\r\n");
		return RTN_ERROR;
	}

	if(queue_size == 0) 
	{
		QUEUE_PRINTF("Create queue with queue size %d.\r\n", queue_size);
		return RTN_ERROR;
	}

	if(sub_key >= SYS_QUEUE_MAX_NUM || queue_used_flag[sub_key] != 0)
	{
		QUEUE_PRINTF("Create queue with sub key %d failed cause wrong key or recreating. \r\n", sub_key);
		return RTN_ERROR;
	}
    
	queue_used_flag[sub_key] = 1;

	/* Set the possible task Id to not free so that  no other task can try to use it */			  
	q_key = ftok("/dev/", sub_key); 	
	if((tmp_skt = msgget(q_key, IPC_CREAT)) < 0)
	{
		QUEUE_PRINTF("Create msgget failed.\r\n");
		return RTN_ERROR;
	}

	if(msgctl(tmp_skt, IPC_RMID, NULL) < 0)
	{
		QUEUE_PRINTF("Delete queue failed.\r\n");
		return RTN_ERROR;
	}

	if((tmp_skt = msgget(q_key, IPC_CREAT)) < 0)
	{
		QUEUE_PRINTF("Create msgget failed.\r\n");
		return RTN_ERROR;
	}
	
	/*Set msg queue max bytes*/
	msgctl(tmp_skt, IPC_STAT, &msg_ds);
	msg_ds.msg_qbytes = queue_size;
	
	if(msgctl(tmp_skt, IPC_SET, &msg_ds) < 0)
	{
		QUEUE_PRINTF("Set msgq size failed.\r\n");
		return RTN_ERROR;
	}

	*p_queue_id = tmp_skt;

	return RTN_OK;
}


/**************************************************************
 * ��������: tf_queue_get
 * ��           ��: keith.gong
 * ��           ��: 2014��12��27��
 *
 * ��������: ����Ϣ�����ж�ȡ��Ϣ
 * �������: 
                            queue_id : 
                            type:        ��Ϣ����
                            p_data:     buffer ��ָ��
                            size:         buffer �Ĵ�С
                            timeout:    SYS_QUEUE_WAIT_FOREVER / SYS_QUEUE_NO_WAIT / ����ֵ
 * �������: 
                            p_data
                            p_size_copied:  ʵ�ʶ�����Ϣ�Ĵ�С
 * ������ֵ: 
                            OK : 
                            ERROR : 
 * ��           ��:  
**************************************************************/
RTN_STATUS
tf_queue_get (
                INT32  queue_id, 
                long   type, 
                void   *p_data, 
                UINT32 size, 
                INT32  timeout, /* ms  */
                UINT32 *p_size_copied)
{
	int size_copied;

	/* Check Parameters */
	if( (p_data == NULL) || (p_size_copied == NULL) ) 
	{
		QUEUE_PRINTF("Get msg failed cause some parameter is NULL.\r\n");
		return RTN_ERROR;
	}	

 	/* Read the socket for data */
	if (timeout == SYS_QUEUE_WAIT_FOREVER)
	{
		size_copied = msgrcv(queue_id, p_data, size, type, 0);		
		//if (size_copied == -1)
		if (size_copied <= 0)
		{
			*p_size_copied = 0;
			return RTN_ERROR;
		}
		else if(size_copied <= size )
		{
			*p_size_copied = size_copied;
			return RTN_OK;
		}
        else            
            return RTN_ERROR;
	}
	else if (timeout == SYS_QUEUE_NO_WAIT)
	{
		size_copied = msgrcv(queue_id, p_data, size, type, IPC_NOWAIT);	  
		//if (size_copied == -1 && errno == ENOMSG )
		if (size_copied <= 0)
		{
			*p_size_copied = 0;
			return RTN_ERROR;
		}
		else if(size_copied <= size )
		{
			*p_size_copied = size_copied;
			return RTN_OK;
		}
        else            
            return RTN_ERROR;
	}
	else /* timeout */
	{
		int timeloop;
		/*
		** This "timeout" will check the socket for data every 100 milliseconds
		** up until the timeout value. Although this works fine for a desktop environment,
		** it should be written more efficiently for a flight system.
		** The proper way will be to use select or poll with a timeout
		*/
		for ( timeloop = timeout; timeloop > 0; timeloop = timeloop - 100 )
		{
			size_copied = msgrcv(queue_id, p_data, size, type, IPC_NOWAIT);

			//if (size_copied == -1)
            if (size_copied <= 0)
			{
				/* Sleep for 100 milliseconds */
				usleep(100 * 1000);
			}
			//else if ( size_copied == size )
			else if ( size_copied <= size)
			{
				*p_size_copied = size_copied;
				return RTN_OK;
			}
            else
                return RTN_ERROR;
		}

		return RTN_ERROR;

	} /* END timeout */

	/* Should never really get here. */
	return RTN_OK;
}



/**************************************************************
 * ��������: tf_queue_put
 * ��           ��: keith.gong
 * ��           ��: 2014��12��27��
 *
 * ��������: д��Ϣ����Ϣ������
 * �������: 
                            queue_id : 
                            p_data:     buffer ��ָ��
                            size:         buffer �Ĵ�С
                            timeout:    SYS_QUEUE_NO_WAIT
 * �������: 
 * ������ֵ: 
                            OK : 
                            ERROR : 
 * ��           ��:  
                            δʵ���������size�ļ�飬����ȷ���������ݴ�Сδ����queue�Ĵ�С
                            timeout �����ݲ�֧�ֺ���ȴ�
**************************************************************/
RTN_STATUS
tf_queue_put (
                INT32  queue_id, 
                void   *p_data, 
                UINT32 size, 
                INT32  timeout)
{
	/* Check Parameters */
	if (p_data == NULL) 
    {
		QUEUE_PRINTF("Can not put NULL msg into the queue. \r\n");
		return RTN_ERROR;
	}

    /* kth.debug, maybe need to check size */
	if(msgsnd(queue_id, (INT8 *)p_data, size, (SYS_QUEUE_NO_WAIT == timeout) ? IPC_NOWAIT : 0) < 0)
	{
		QUEUE_PRINTF("Send msg failed.\r\n");
		return RTN_ERROR;
	}

	return RTN_OK;
}


