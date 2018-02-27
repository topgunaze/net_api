/**************************************************************
* �ļ�����: 
* �ļ�����:
* ��           ��: 
* �޸���ʷ:
*     <�޸���>     <ʱ��>      <�汾 >     <����>

**************************************************************/

#include "tf_adaptor_net_if.h"

#if DEFUNC("����ͨ�Žӿ�")

/*
����ֶ�����
*/
static unsigned int 
tf_net_msg_push(
                    TF_NET_MULTI_MSG    *pmsg, 
                    void                *pdata, 
                    unsigned int        len)
{
    if (NULL == pdata || NULL == pmsg)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    if (pmsg->count >= IO_MAXSIZE)
    {
        printf("The parameter value is out of range !\n");
        return TF_MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    pmsg->io[pmsg->count].iov_base = pdata;
    pmsg->io[pmsg->count].iov_len  = len;
    pmsg->count++;
    
    return 0;
}

/*
�հ�
*/
static unsigned int 
tf_net_rec_packet(
                int             fd, 
                void            *pdata, 
                unsigned int    data_len, 
                unsigned int    *prec_len)
{
    int recv_num = 0;

    if (!prec_len)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    if (!pdata || 0 == data_len)
    {
        //û����Ҫ���յ�����
        return 0;
    }

    recv_num = recv(fd, pdata, data_len, 0);
    if (recv_num < 0)
    {
        switch(errno)
        {
            case EAGAIN:
                //try again
                errno = 0;
                break;
            case ETIMEDOUT:
                //keepallive error
                break;
            default: 
                printf("AF_INET server bad,ERRNO:%d,errno description:%s\n",errno,strerror(errno));
                return TF_MDW_NET_RC_REC_PACKET;
        }

    }
    else
    if (0 == recv_num)
    {
        printf("remote disconnected !\n");  
        return TF_MDW_NET_RC_REMOTE_DISCONNECT;
    }
    else
    {
        *prec_len = recv_num;
    }
    
    return 0;
}

/*
���չ̶����Ȱ�
*/
unsigned int 
tf_net_rec_packet_fix(
                int             fd, 
                void            *pdata, 
                unsigned int    data_len)
{
    unsigned int read_len = 0;
    unsigned int rc;
    unsigned int len = 0;
  
    do 
    {
        rc = tf_net_rec_packet(fd, (char*)pdata + read_len, data_len - read_len, &len);
        if(rc)  
        {          
            printf("read error !\n");
            return rc;  
        }
        
        read_len += len;
        
    }while(data_len > read_len);

    return 0;
}

static unsigned int 
tf_net_rec_packet_peek(
                int             fd, 
                void            *pdata, 
                unsigned int    data_len, 
                unsigned int    *prec_len)
{
    int recv_num = 0;

    if (!prec_len)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    if (!pdata || 0 == data_len)
    {
        //û����Ҫ���յ�����
        return 0;
    }

    recv_num = recv(fd, pdata, data_len, MSG_PEEK);
    if(recv_num <  0)
    {
        switch(errno)
        {
            case EAGAIN:
                    //try again
                    break;
            case ETIMEDOUT:
                    //keepallive error
                    break;
            default: 
                    printf("AF_INET server bad,ERRNO:%d,errno description:%s\n", errno, strerror(errno));
                    return TF_MDW_NET_RC_REC_PACKET;
        }

    }
    else
    if (0 == recv_num)
    {
        printf("remote disconnected !\n");  
        return TF_MDW_NET_RC_REMOTE_DISCONNECT;
    }
    else
    {
        *prec_len = recv_num;
    }
    
    return 0;
}

unsigned int 
tf_net_rec_packet_fix_peek(
                int             fd, 
                void            *pdata, 
                unsigned int    data_len)
{
    unsigned int read_len = 0;
    unsigned int rc;
    unsigned int len      = 0;
  
    do 
    {
        rc = tf_net_rec_packet_peek(fd, (char*)pdata + read_len, data_len - read_len, &len);
        if(rc)  
        {          
            printf("read error !\n");
            return rc;  
        }
        
        read_len += len;
        
    }while(data_len > read_len);

    return 0;
}

/*
���ڶ˿ڵķ�������
*/
static unsigned int 
tf_net_send_msg(
                int                fd,
                struct sockaddr_in *p_remote_addr,
                TF_NET_MULTI_MSG   *pmsg)
{
    int             send_size;
    struct msghdr   tmp_msg;
    int             i;

    if (NULL == p_remote_addr || NULL == pmsg)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    bzero((void*)&tmp_msg,sizeof(struct msghdr));
    
    tmp_msg.msg_name    = p_remote_addr;
    tmp_msg.msg_namelen = sizeof(struct sockaddr_in);
    tmp_msg.msg_iov     = pmsg->io;
    tmp_msg.msg_iovlen  = pmsg->count;

    for (i = 0; i < TF_NET_SEND_COUNT; i++)
    {
        send_size = sendmsg(fd, &tmp_msg, 0);
        if (send_size < 0)
        {
            printf("sendmsg err %d, try again\n", errno);
        }
        else
        break;
    }

    return 0;
}

//������Ϣ
unsigned int 
tf_net_send_packet(
                int                fd,
                struct sockaddr_in *p_remote_addr,
                TF_NET_MSG         *pnet_msg)
{
    int                 ret;
    TF_NET_MULTI_MSG    msg;

    if (NULL == p_remote_addr || NULL == pnet_msg)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    bzero((void*)&msg, sizeof(msg));

    if ((ret = tf_net_msg_push(&msg, pnet_msg, pnet_msg->len)))
    {
        printf("data_push error !\n");
        return ret;
    }

    ret = tf_net_send_msg(fd, p_remote_addr, &msg);
    if(ret)
    {
        printf("AF_INET sendto error !\n");
        return ret;
    } 

    return 0;
}

unsigned int 
tf_net_send_alone_packet(
                int         fd, 
                TF_NET_MSG  *pnet_msg)
{
    int ret, i, send_size;

    if (NULL == pnet_msg)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    for (i = 0; i < TF_NET_SEND_COUNT; i++)
    {
        send_size = send(fd, pnet_msg, pnet_msg->len, 0);
        if (send_size < 0)
        {
            printf("send err %d, try again\n", errno);
        }
        else
        break;
    }

    return 0;
}

static unsigned int 
tf_net_opt_init(int fd)
{
    int             ret;
    int             reuse = 1;
    struct linger   so_linger;

    //���õ�ַ������
    ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse));
    if (ret < 0) 
    {
        goto fail;
    }

    //���ð�����������ӳٹر�
    so_linger.l_onoff  = 1;
    so_linger.l_linger = 1;
    
    ret = setsockopt(fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
    if (ret < 0) 
    {
        goto fail;
    }

    return 0;

fail:
    printf("%s %s %d error %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
    return TF_MDW_NET_RC_SETSOCKOPT;
}

unsigned int 
tf_net_trbuf_get(
                int fd, 
                int *sdb_size, 
                int *rdb_size)
{
    int ret;
    int result     = 0;
    int result_len = sizeof(result);

    //��ȡ���ͺͽ��ջ�������С
    ret = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void*)&result, (socklen_t*)&result_len);
    if (ret < 0) 
    {
        goto fail;
    }

    *sdb_size = result;

    ret = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void *)&result, (socklen_t*)&result_len);
    if (ret < 0) 
    {
        goto fail;
    }

    *rdb_size = result;

    return 0;

fail:
    printf("%s %s %d errorno %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
    return TF_MDW_NET_RC_GETSOCKOPT;
}

unsigned int 
tf_net_trbuf_set(
                int fd, 
                int sdb_size, 
                int rdb_size)
{
    int ret;
    int result;

    //��ȡ���ͺͽ��ջ�������С
    ret = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void *)&sdb_size, sizeof(sdb_size));
    if (ret < 0) 
    {
        goto fail;
    }

    ret = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void *)&rdb_size, sizeof(rdb_size));
    if (ret < 0) 
    {
        goto fail;
    }

    return 0;

fail:
    printf("%s %s %d errorno %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
    return TF_MDW_NET_RC_SETSOCKOPT;
}

// ����keepalive����
unsigned int 
tf_net_keepalive_set(
                int fd, 
                int keepalive, 
                int idle, 
                int interval, 
                int count)
{
    int ret;

    //������ر�keepalive
    ret = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive));
    if (ret < 0) 
    {
        goto fail;
    }

    if (0 == keepalive)
    {
        return 0;
    }

    // ��ð��������idle����û���κ���������,�����̽��
    ret= setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void*)&idle, sizeof(idle));
    if (ret < 0) 
    {
        goto fail;
    }
    
    // ̽��ʱ������ʱ����Ϊinterval��
    ret = setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void *)&interval, sizeof(interval));
    if (ret < 0) 
    {
        goto fail;
    }
    
    // ̽�Ⳣ�ԵĴ���.�����1��̽������յ���Ӧ��,���2�εĲ��ٷ�
    ret = setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (void *)&count, sizeof(count));
    if (ret < 0) 
    {
        goto fail;
    }

    return 0;

fail:
    printf("%s %s %d errorno %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
    return TF_MDW_NET_RC_SETSOCKOPT;
}

unsigned int 
tf_net_setnonblock(int fd)
{
    long flags;

    flags = fcntl(fd, F_GETFL);
    if (flags < 0) 
    {
        goto fail;
    }

    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0) 
    {
        goto fail;
    }

    return 0;
    
fail:
    printf("%s %s %d errorno %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
    return TF_MDW_NET_RC_FCNTL;
}

unsigned int 
tf_net_init(struct sockaddr_in *psin, int *p_fd)
{
    int             fd;
    struct ifreq    ifr_ip;
    char            ipaddr[20];

    if (NULL == psin || NULL == p_fd)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    if( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )  
    {  
        printf("%s %s %d error %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        return TF_MDW_NET_RC_ERROR;
    }

    if (0 != tf_net_opt_init(fd))
    {
        printf("%s %s %d error %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        goto fail;
    }

    //��ȡ����lo��ַ
    bzero(&ifr_ip, sizeof(struct ifreq));     
    strncpy(ifr_ip.ifr_name, "lo", sizeof(ifr_ip.ifr_newname) - 1);
    
    if(ioctl(fd, SIOCGIFADDR, &ifr_ip) < 0)     
    {     
        printf("%s %s %d error %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        goto fail;
    }  
             
    memcpy(psin, &ifr_ip.ifr_ifru.ifru_addr, sizeof(struct sockaddr));
    
    strncpy(ipaddr, (char*)inet_ntoa(psin->sin_addr), sizeof(ipaddr));
    printf("local ip %s\n", ipaddr);

    *p_fd = fd;

    return 0;

fail:
    close(fd);
    return TF_MDW_NET_RC_INIT;
}

//У����Ϣ
unsigned int 
tf_net_msg_verify(TF_NET_MSG *pmsg)
{
    if (NULL == pmsg)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    //msg base
    if ((pmsg->msg_type >= NET_MSG_TYPE_NUM_OF) || 
        (pmsg->syn_flag >= NET_MSG_SYN_NUM_OF) || 
        (pmsg->direction >= NET_MSG_DRICTION_NUM_OF))
    {
        printf("%s %s %d msg base type error\n", __FILE__, __FUNCTION__, __LINE__);
        return TF_MDW_NET_RC_MSG_HEADER_VERIFY;
    }

    //msg len
    if (pmsg->len < NET_MSG_HEAD_LEN)
    {
        printf("%s %s %d msg len error\n", __FILE__, __FUNCTION__, __LINE__);
        return TF_MDW_NET_RC_MSG_HEADER_VERIFY;
    }
   
    return 0;
}

/*
    ��װ��Ϣ,�����ڴ����ⲿ�ͷţ�
    pvar_len ���ڷ�����Ϣ���ڴ棬
    param_len���ڹ���ش���Ϣ���ڴ棬
    ����ctrl��Ҫ����param_len >= pvar_len
    ����drv��Ҫ����param_len == pvar_len
*/
//�����ڴ�ʹ��ϵͳ�ӿ�
TF_NET_MSG *
tf_net_msg_pack(   
                TF_NET_BLK_POOL            *p_mem_pool,
                const unsigned long long   vif_info,
                const TF_NET_MSG_CMD       cmd_info, 
                const void                 *pvar, 
                const unsigned int         pvar_len,
                const unsigned int         param_len)
{
    unsigned int out_len = pvar_len;
    
    //para check pvar ��pvar_len ��Ӧ
    if ((NULL == pvar && pvar_len) || (pvar && 0 == pvar_len))
    {
        return NULL;  
    }

    if (param_len < pvar_len)
    {
        return NULL;
    }

    //����ͬ����Ϣ�ṹ�����ڴ�: header + bodyԤ���ռ�
    int msg_size = NET_MSG_HEAD_LEN + param_len;
    //tx��Ϣ��Ч�ڴ�, ��Ҫ������Ϣ����ʵ����
    int msg_len  = NET_MSG_HEAD_LEN + pvar_len;

    TF_NET_MSG *msg = tf_net_malloc(p_mem_pool, msg_size);

    if (!msg)
    {
        return NULL;  
    }

    msg->len       = msg_len;
    msg->vif_info  = vif_info;
    msg->corr_tag  = 0;         //ħ����Ĭ��0
    msg->state     = 0;         //sdkĬ�Ϸ���0
    msg->body_size = param_len;
    msg->compress  = 0;         //Ĭ�ϲ�ѹ��

    memcpy(&msg->cmd_info, &cmd_info, sizeof(TF_NET_MSG_CMD));
    
    if (pvar && pvar_len)
    {
        //����ѹ������
        if (!tf_net_msg_compress_process(msg, pvar, pvar_len, &out_len))
        {
            //corrected parameter
            msg->uncmp_len  = pvar_len;
            msg->len        = out_len + NET_MSG_HEAD_LEN;
            msg->compress   = 1;
        }
        else
        {
            //ʹ�÷�ѹ����Ϣ
            memcpy(msg->body, pvar, pvar_len);
        }
    }

    return msg;
}

unsigned long long 
tf_net_htonll(unsigned long long x)
{
    unsigned int tmp1, tmp2;

    tmp1 = htonl(x&0xFFFFFFFF);
    tmp2 = htonl((x>>32) & 0xFFFFFFFF);
    x = (unsigned long long)tmp1 << 32;
    x |= tmp2;
    
    return (unsigned long long)tmp1 << 32 | tmp2;;
}

unsigned long long 
tf_net_ntohll(unsigned long long x)
{
    unsigned int tmp1, tmp2;

    tmp1 = ntohl(x&0xFFFFFFFF);
    tmp2 = ntohl((x>>32) & 0xFFFFFFFF);

    return (unsigned long long)tmp1 << 32 | tmp2;
}

//��Ϣ����ǰ��Ϣͷ������
unsigned int
tf_net_msg_header_tx_process(int *p_conn_id, TF_NET_MSG *p_msg)
{
    if (!p_conn_id || !p_msg)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    //��ȡ���ͶԶ�
    *p_conn_id = MSG2SLOT(p_msg);
    
    //msg ͷ���ֽ�������������
    p_msg->msg_type           = htonl(p_msg->msg_type);
    p_msg->cmd_info.cmd_id    = htonl(p_msg->cmd_info.cmd_id);
    p_msg->cmd_info.timeout   = htonl(p_msg->cmd_info.timeout);
    p_msg->cmd_info.order     = htonl(p_msg->cmd_info.order);
    p_msg->vif_info           = tf_net_htonll(p_msg->vif_info);
    p_msg->syn_flag           = htonl(p_msg->syn_flag);
    p_msg->corr_tag           = htonl(p_msg->corr_tag);
    p_msg->direction          = htonl(p_msg->direction);
    p_msg->state              = htonl(p_msg->state);
    p_msg->len                = htonl(p_msg->len);
    p_msg->body_size          = htonl(p_msg->body_size);
    
    return 0;
}

#endif

#if DEFUNC("ͬ������")

static TF_NET_CONN_MSG *
tf_net_conn_msg_syn_varify(
                const TF_NET_CONN   *pconn, 
                const TF_NET_MSG    *pmsg)
{
    TF_NET_CONN_MSG *pcmsg;
    TF_NET_MSG      *ptmsg;

    TAILQ_FOREACH(pcmsg, &pconn->msg_list, l)
    {
        ptmsg = (TF_NET_MSG*)(pcmsg->data);
        if (ptmsg->corr_tag == pmsg->corr_tag)
            break;
    }
    
    return pcmsg;
}

static void 
tf_net_conn_msg_syn_ready(TF_NET_CONN_MSG *pcmsg)
{
    tf_net_sem_post(&pcmsg->sem);
}

static inline void 
tf_net_conn_msg_syn_restfse(TF_NET_CONN *pconn, TF_NET_CONN_MSG *pcmsg)
{
    tf_net_mutex_unlock(&pconn->mutex);

    tf_net_conn_msg_syn_ready(pcmsg);
}

unsigned int 
tf_net_conn_msg_syn_ack_process(TF_NET_CONN *pconn, TF_NET_MSG *pmsg)
{  
    TF_NET_CONN_MSG    *pcmsg;
    int                 max_len;

    tf_net_mutex_lock(&pconn->mutex);
    
    pcmsg = tf_net_conn_msg_syn_varify(pconn, pmsg);
    if (!pcmsg)
    {
        printf("tf_net_conn_msg_syn_varify error!!!\n");
        tf_net_mutex_unlock(&pconn->mutex);
        return TF_APP_ZK_RC_MSG_SYN_VARIFY;
    }

    //AR msg copy ��syn send �ӿ�������ڴ���
    //��������Ϣ��������Ӧ������msg body size >= len - header_len
    max_len = ((TF_NET_MSG*)(pcmsg->data))->body_size + NET_MSG_HEAD_LEN;
    memcpy(pcmsg->data, pmsg, (pmsg->len > max_len ? max_len : pmsg->len));

    TAILQ_REMOVE(&pconn->msg_list, pcmsg, l);

    tf_net_conn_msg_syn_restfse(pconn, pcmsg);

    return 0;
}

//��ȡfifo�е�conn msg �������ⲿ����
TF_NET_CONN_MSG* 
tf_net_conn_msg_get_free(tf_net_conn_list *plist)
{
    TF_NET_CONN_MSG *pcmsg = TAILQ_FIRST(plist);
    
    if (pcmsg)
        TAILQ_REMOVE(plist, pcmsg, l);
        
    return pcmsg;
}

//conn msg����ж���,�����ⲿ����
void 
tf_net_conn_msg_free(  TF_NET_CONN_MSG  *pcmsg, 
                            tf_net_conn_list *plist)
{
    pcmsg->data = NULL;

    TAILQ_INSERT_TAIL(plist, pcmsg, l);
}

#endif

#if DEFUNC("���ݽ�ѹ��")

static unsigned int
tf_net_msg_need_compress(TF_NET_MSG* p_msg)
{
    //��ʹ��ѹ����ֱ�ӷ���ʧ�ܣ�ʹ�÷�ѹ������
#ifndef TF_NET_USE_COMPRESS
        return TF_MDW_NET_RC_COMPRESS_NO_NEED;
#endif
    
    //��ʹ��ѹ����ֱ�ӷ���ʧ�ܣ�ʹ�÷�ѹ������
    if (p_msg->len < NET_MSG_HEAD_LEN + TF_NET_COMPRESS_MIN_LEN)
        return TF_MDW_NET_RC_COMPRESS_NO_NEED;

    return 0;
}

//��Ϣѹ��
unsigned int
tf_net_msg_compress_process(
                                TF_NET_MSG          *p_msg,
                                const void          *p_var, 
                                const unsigned int  var_len,
                                unsigned int        *p_out_len)
{
    unsigned rc;
    unsigned level = 1;

    if (!p_msg || !p_var || !p_out_len)
    {
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    if (!var_len)
    {
        return TF_MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    if ((rc = tf_net_msg_need_compress(p_msg)))
    {
        return rc;
    }
    //ѹ������1-9�� Խ��Խ��ʱ
    rc = tf_net_data_compress(p_var, (const unsigned long)var_len, p_msg->body, (unsigned long*)p_out_len, level);
    
    return rc; 
}

unsigned int
tf_net_msg_need_uncompress(TF_NET_MSG *p_msg)
{
    //��ʹ��ѹ����ֱ�ӷ��ش���δѹ����Ϣ
#ifndef TF_NET_USE_COMPRESS
        return TF_MDW_NET_RC_UNCOMPRESS_NO_NEED;
#endif
    
    //��ʹ��ѹ����ֱ�ӷ��ش���δѹ����Ϣ
    if (!p_msg->compress)
        return TF_MDW_NET_RC_UNCOMPRESS_NO_NEED;

    return 0;
}

//��Ϣ��ѹ������
unsigned int
tf_net_msg_uncompress_process(TF_NET_BLK_POOL *p_mem_pool, TF_NET_MSG **p_msg)
{
    unsigned int    rc;
    TF_NET_MSG     *p_in    = *p_msg;
    TF_NET_MSG     *p_out;
    unsigned int    out_len = p_in->uncmp_len;
    
    if (!p_mem_pool || !p_msg || !p_in)
        return TF_MDW_NET_RC_PARAM_NULL;

    if ((rc = tf_net_msg_need_uncompress(p_in)))
    {
        return 0;   
    }

    //�����Ϣ�ṹ��Ϊbody�Ļ���
    p_out = tf_net_malloc(p_mem_pool, NET_BUFFER_LEN);
    if (!p_out)
        return TF_MDW_NET_RC_MEM_ALLOCATE;
        
    memcpy(p_out, p_in, NET_MSG_HEAD_LEN);
    
    rc = tf_net_data_uncompress((void*)p_in->body, p_in->len-NET_MSG_HEAD_LEN, (void*)p_out->body, (unsigned long*)&out_len);
    if (rc)
    {
        tf_net_safe_free(p_out);
        return rc;
    }
    
    p_out->len  = out_len + NET_MSG_HEAD_LEN;
    *p_msg      = p_out;
    
    tf_net_safe_free(p_in);

    return 0;
}

#endif

#if DEFUNC("�ӿڲ���ת��")

//����id ��0Ϊ��ʵ����������gtf_ni 0~31 0x00~0x1f
unsigned int 
tf_net_vif2phy(unsigned long long ulong, TF_NET_PHY_INFO *p_phy)
{
    tf_rc_info      rc_info = {.obj_id = TF_OBJ_MDW_NET_RC_OFFSET};
    unsigned    int rc;

    if (!p_phy)
    {
        TF_TF_ERRNO_INFO2RC(rc_info, rc, TF_MDW_NET_RC_PARAM_NULL);
        return rc; 
    }
    
    p_phy->key_type = ulong >> KEY_TYPE_MOVE_BITS;
    
    switch (p_phy->key_type) 
    {
        case TF_NI:
            p_phy->phy_oscs.slot_id = (ulong & 0xff);
            p_phy->phy_oscs.tf_ni   = ((ulong >> SLOT_ID_HOLD_BITS) & 0xff);
            break;

        case TF_ONU:
            p_phy->phy_oscs.slot_id = (ulong & 0xff);
            p_phy->phy_oscs.tf_ni   = ((ulong >> SLOT_ID_HOLD_BITS) & 0xff);
            p_phy->phy_oscs.onu_id  = ((ulong >> (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS)) & 0xff);
            break;

        case TF_ALLOC:
            p_phy->phy_oscs.slot_id  = (ulong & 0xff);
            p_phy->phy_oscs.tf_ni   = ((ulong >> SLOT_ID_HOLD_BITS) & 0xff);
            p_phy->phy_oscs.alloc_id = ((ulong >> (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS)) & 0xffff);
            break;

        case TF_GEMPORT:
            p_phy->phy_oscs.slot_id    = (ulong & 0xff);
            p_phy->phy_oscs.tf_ni     = ((ulong >> SLOT_ID_HOLD_BITS) & 0xff);
            p_phy->phy_oscs.gemport_id = ((ulong >> (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS)) & 0xffff);
            break;

        case TF_VLAN:
            p_phy->phy_oscs.slot_id = (ulong & 0xff);
            p_phy->phy_oscs.tf_ni  = ((ulong >> SLOT_ID_HOLD_BITS) & 0xff);
            p_phy->phy_oscs.vlan_id = ((ulong >> (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS)) & 0xffff);
            break;

        case TF_FLOW:
            p_phy->phy_oscs.slot_id = (ulong & 0xff);
            p_phy->phy_oscs.tf_ni  = ((ulong >> SLOT_ID_HOLD_BITS) & 0xff);
            p_phy->phy_oscs.flow_id = ((ulong >> (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS)) & 0xffff);
            break;

        case TF_GPIO:
            p_phy->phy_oscs.slot_id = (ulong & 0xff);
            p_phy->phy_oscs.tf_ni  =  ((ulong >> SLOT_ID_HOLD_BITS) & 0xff);
            p_phy->phy_oscs.gpio_id = ((ulong >> (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS)) & 0xffff);
            break;

        case TF_BOARD:
            p_phy->phy_oscs.slot_id = (ulong & 0xff);
            break;
        
        default:
            TF_TF_ERRNO_INFO2RC(rc_info, rc, TF_MDW_NET_RC_PARAM_OUT_OF_RANGE);
            return rc;
    }

    return 0;    
}

unsigned int 
tf_net_phy2vif(TF_NET_PHY_INFO *p_phy, unsigned long long *p_ulong)
{
    tf_rc_info      rc_info = {.obj_id = TF_OBJ_MDW_NET_RC_OFFSET};
    unsigned    int rc;
    
    if (!p_phy || !p_ulong)
    {
        TF_TF_ERRNO_INFO2RC(rc_info, rc, TF_MDW_NET_RC_PARAM_NULL);
        return rc;
    }
    
    #if 0
    printf("inner phy info:\n"
           "key_type: %d\n"
           "slot_id: %d\n"
           "tf_ni: %d\n"
           "onu_id: %d\n"
           "alloc_id: %d\n"
           "gemport_id: %d\n"
           "vlan_id: %d\n"
           "flow_id: %d\n"
           "gpio_id: %d\n",
           p_phy->key_type, p_phy->phy_oscs.slot_id, p_phy->phy_oscs.tf_ni, 
           p_phy->phy_oscs.onu_id, p_phy->phy_oscs.alloc_id, 
           p_phy->phy_oscs.gemport_id, p_phy->phy_oscs.vlan_id, 
           p_phy->phy_oscs.flow_id, p_phy->phy_oscs.gpio_id);
    #endif
    
    *p_ulong = (unsigned long long)p_phy->key_type << KEY_TYPE_MOVE_BITS;

    switch (p_phy->key_type) 
    {
        case TF_NI:
            *p_ulong |= p_phy->phy_oscs.slot_id; 
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.tf_ni << SLOT_ID_HOLD_BITS);
            break;

        case TF_ONU:
            *p_ulong |= p_phy->phy_oscs.slot_id; 
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.tf_ni << SLOT_ID_HOLD_BITS);
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.onu_id << (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS));
            break;

        case TF_ALLOC:
            *p_ulong |= p_phy->phy_oscs.slot_id; 
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.tf_ni << SLOT_ID_HOLD_BITS);
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.alloc_id << (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS));
            break;

        case TF_GEMPORT:
            *p_ulong |= p_phy->phy_oscs.slot_id; 
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.tf_ni << SLOT_ID_HOLD_BITS);
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.gemport_id << (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS));
            break;

        case TF_VLAN:
            *p_ulong |= p_phy->phy_oscs.slot_id; 
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.tf_ni << SLOT_ID_HOLD_BITS);
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.vlan_id << (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS));
            break;

        case TF_FLOW:
            *p_ulong |= p_phy->phy_oscs.slot_id; 
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.tf_ni << SLOT_ID_HOLD_BITS);
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.flow_id << (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS));
            break;

        case TF_GPIO:
            *p_ulong |= p_phy->phy_oscs.slot_id; 
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.tf_ni << SLOT_ID_HOLD_BITS);
            *p_ulong |= ((unsigned long long)p_phy->phy_oscs.gpio_id << (SLOT_ID_HOLD_BITS + TF_NI_HOLD_BITS));
            break;

        case TF_BOARD:
            *p_ulong |= p_phy->phy_oscs.slot_id; 
            break;

        default:
            TF_TF_ERRNO_INFO2RC(rc_info, rc, TF_MDW_NET_RC_PARAM_OUT_OF_RANGE);
            return rc;
    }

    return 0;
}

//TF_NI if param convert
unsigned int 
tf_net_tf_ni2vif_ullong(  
                                unsigned int        slot_id, 
                                unsigned int        tf_ni,
                                unsigned long long  *p_ullong)
{
    TF_NET_PHY_INFO olt_phy_info;

    olt_phy_info.key_type         = TF_NI;
    olt_phy_info.phy_oscs.slot_id = slot_id;
    olt_phy_info.phy_oscs.tf_ni   = tf_ni;

    return tf_net_phy2vif(&olt_phy_info, p_ullong);
}

//TF_ONU if param convert
unsigned int 
tf_net_tf_onu2vif_ullong( 
                                unsigned int       slot_id, 
                                unsigned int       tf_ni ,
                                unsigned int       onu_id,
                                unsigned long long *p_ullong)
{
    TF_NET_PHY_INFO olt_phy_info = {0};

    olt_phy_info.key_type         = TF_ONU;
    olt_phy_info.phy_oscs.slot_id = slot_id;
    olt_phy_info.phy_oscs.tf_ni   = tf_ni;
    olt_phy_info.phy_oscs.onu_id  = onu_id;

    return tf_net_phy2vif(&olt_phy_info, p_ullong);
}

//TF_ALLOC if param convert
unsigned int 
tf_net_tf_alloc2vif_ullong( 
                                unsigned int       slot_id, 
                                unsigned int       tf_ni ,
                                unsigned int       alloc_id,
                                unsigned long long *p_ullong)
{
    TF_NET_PHY_INFO olt_phy_info = {0};

    olt_phy_info.key_type           = TF_ALLOC;
    olt_phy_info.phy_oscs.slot_id   = slot_id;
    olt_phy_info.phy_oscs.tf_ni     = tf_ni;
    olt_phy_info.phy_oscs.alloc_id  = alloc_id;

    return tf_net_phy2vif(&olt_phy_info, p_ullong);
}

//TF_GEMPORT if param convert
unsigned int 
tf_net_tf_gemport2vif_ullong( 
                                    unsigned int       slot_id, 
                                    unsigned int       tf_ni ,
                                    unsigned int       gemport_id,
                                    unsigned long long *p_ullong)
{
    TF_NET_PHY_INFO olt_phy_info = {0};

    olt_phy_info.key_type             = TF_GEMPORT;
    olt_phy_info.phy_oscs.slot_id     = slot_id;
    olt_phy_info.phy_oscs.tf_ni       = tf_ni;
    olt_phy_info.phy_oscs.gemport_id  = gemport_id;

    return tf_net_phy2vif(&olt_phy_info, p_ullong);	
}

//TF_GEMPORT if param convert
unsigned int 
tf_net_tf_vlan2vif_ullong( 
                                unsigned int       slot_id, 
                                unsigned int       tf_ni ,
                                unsigned int       vlan_id,
                                unsigned long long *p_ullong)
{
    TF_NET_PHY_INFO olt_phy_info = {0};

    olt_phy_info.key_type             = TF_VLAN;
    olt_phy_info.phy_oscs.slot_id     = slot_id;
    olt_phy_info.phy_oscs.tf_ni       = tf_ni;
    olt_phy_info.phy_oscs.vlan_id     = vlan_id;

    return tf_net_phy2vif(&olt_phy_info, p_ullong);
}

//TF_FLOW if param convert
unsigned int 
tf_net_tf_flow2vif_ullong( 
                                unsigned int       slot_id, 
                                unsigned int       tf_ni ,
                                unsigned int       flow_id,
                                unsigned long long *p_ullong)
{
    TF_NET_PHY_INFO olt_phy_info = {0};

    olt_phy_info.key_type             = TF_FLOW;
    olt_phy_info.phy_oscs.slot_id     = slot_id;
    olt_phy_info.phy_oscs.tf_ni       = tf_ni;
    olt_phy_info.phy_oscs.flow_id     = flow_id;

    return tf_net_phy2vif(&olt_phy_info, p_ullong);
}

//TF_GPIO if param convert
unsigned int 
tf_net_tf_gpio2vif_ullong( 
                                unsigned int       slot_id, 
                                unsigned int       tf_ni ,
                                unsigned int       gpio_id,
                                unsigned long long *p_ullong)
{
    TF_NET_PHY_INFO olt_phy_info = {0};

    olt_phy_info.key_type             = TF_FLOW;
    olt_phy_info.phy_oscs.slot_id     = slot_id;
    olt_phy_info.phy_oscs.tf_ni       = tf_ni;
    olt_phy_info.phy_oscs.gpio_id     = gpio_id;

    return tf_net_phy2vif(&olt_phy_info, p_ullong);
}

//TF_BOARD if param convert
unsigned int 
tf_net_tf_board2vif_ullong( 
                                unsigned int       slot_id, 
                                unsigned long long *p_ullong)
{
    TF_NET_PHY_INFO olt_phy_info = {0};

    olt_phy_info.key_type             = TF_BOARD;
    olt_phy_info.phy_oscs.slot_id     = slot_id;

    return tf_net_phy2vif(&olt_phy_info, p_ullong);
}

#endif

