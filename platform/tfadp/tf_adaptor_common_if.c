/**************************************************************
* �ļ�����: 
* �ļ�����:
* ��           ��: 
* �޸���ʷ:
*     <�޸���>     <ʱ��>      <�汾 >     <����>

**************************************************************/

#include "tf_adaptor_common_if.h"
#include "tf_adaptor_errno.h"
#include <sys/syscall.h>   
#include "zlib.h"


#if DEFUNC("ʱ������ӿ�")

void 
tf_net_timewait(int sec, int us)
{
    struct timeval tv;

    tv.tv_sec   = sec;
    tv.tv_usec  = us;
    select (0, NULL, NULL, NULL, &tv);

    return ;  
}

/* 
Get current timestamp
returns the current system timestamp (us)
 */
unsigned int 
tf_net_timestamp(void)
{
    struct timespec tp;
    
    clock_gettime(CLOCK_MONOTONIC, &tp);
    
    return (tp.tv_sec * 1000000 + tp.tv_nsec / 1000);
}

/* calculate absolute time equal to the current time + timeout */
void 
tf_net_abs_time_get(struct timespec *pts, int timeout)
{
    int rc;
    
    rc = clock_gettime(CLOCK_REALTIME, pts);

    pts->tv_sec += timeout / 1000000;
    pts->tv_nsec += (timeout % 1000000) * 1000;
    if (pts->tv_nsec > 1000000000)
    {
        pts->tv_sec += pts->tv_nsec / 1000000000;
        pts->tv_nsec %= 1000000000;
    }
}

#endif

#if DEFUNC("�ź��������ӿ�")

/* Create semaphore */
unsigned int 
tf_net_sem_create(TF_NET_SEM *psem, int count)
{
    if (!psem)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    sem_init(&psem->s, 0, count);
    return 0;
}

/* Destroy semaphore */
unsigned int
tf_net_sem_destroy(TF_NET_SEM *psem)
{
    if (!psem)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    sem_destroy(&psem->s);

    return 0;
}

/* Decrement semaphore counter. Wait if the counter is 0 */
unsigned int 
tf_net_sem_wait(TF_NET_SEM *psem, int timeout)
{
    int rc;
    struct timespec ts;

    if (!psem)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    /* Init end time if necessary */
    if (timeout && timeout != TF_NET_WAIT_FOREVER)
    {
        tf_net_abs_time_get(&ts, timeout);
    }

    do
    {
        if (timeout == TF_NET_WAIT_FOREVER)
        {
            rc = sem_wait(&psem->s);
        }
        else if (timeout)
        {
            rc = sem_timedwait(&psem->s, &ts);
        }
        else
        {
            rc = sem_trywait(&psem->s);
        }
    } while (rc && errno == EINTR);

    if (rc)
    {
        rc = errno;
        if (rc == ETIMEDOUT)
        {
            printf("sem_wait() timeout\n");
        }
        else
        {
            printf("sem_wait() error: %d\n", rc);
        }

        return TF_MDW_NET_RC_MSG_SEM_WAIT;
    }

    return 0;
}

/* Increment semaphore counter */
unsigned int 
tf_net_sem_post(TF_NET_SEM *psem)
{
    if (!psem)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    sem_post(&psem->s);

    return 0;
}

#endif

#if DEFUNC("�������ӿ�")

//������
/* Create recursive mutex */
unsigned int 
tf_net_mutex_create(TF_NET_MUTUX *pmutex)
{
    int err;

    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    err = pthread_mutexattr_init(&pmutex->attr);
    err = err ? err : pthread_mutexattr_settype(&pmutex->attr, PTHREAD_MUTEX_RECURSIVE_NP);
    err = err ? err : pthread_mutex_init(&pmutex->m, &pmutex->attr);
    
    if (err)
    {
        printf("tf_net_mutex_create error!!!\n");
        return TF_MDW_NET_RC_MUTEX_CREATE;
    }
  
    return 0;
}

/*Destroy mutex*/
unsigned int tf_net_mutux_destory(TF_NET_MUTUX *pmutex)
{
    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_mutex_destroy(&pmutex->m);
    pthread_mutexattr_destroy(&pmutex->attr);

    return 0;
}

unsigned int 
tf_net_mutex_trylock(TF_NET_MUTUX *pmutex)
{
    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    return pthread_mutex_trylock(&pmutex->m); 
}

/*Lock mutex*/
unsigned int 
tf_net_mutex_lock(TF_NET_MUTUX *pmutex)
{
    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_mutex_lock(&pmutex->m); 
    
    return 0;
}

/* Release mutex*/
unsigned int
tf_net_mutex_unlock(TF_NET_MUTUX *pmutex)
{
    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL; 
    }
    
    pthread_mutex_unlock(&pmutex->m);
    return 0;
}

//��д��
unsigned int 
tf_net_rwlock_create(TF_NET_RWLOCK *p_rwlock)
{
    int err;

    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    err = pthread_rwlockattr_init(&p_rwlock->attr);
    err = err ? err : pthread_rwlockattr_setpshared(&p_rwlock->attr, PTHREAD_PROCESS_PRIVATE);//PTHREAD_PROCESS_SHARED
    err = err ? err : pthread_rwlock_init(&p_rwlock->m, &p_rwlock->attr);
    
    if (err)
    {
        printf("tf_net_rwlock_create error!!!\n");
        return TF_MDW_NET_RC_RWLOCK_CREATE;
    }
  
    return 0;
}

unsigned int 
tf_net_rwlock_destory(TF_NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_destroy(&p_rwlock->m);
    pthread_rwlockattr_destroy(&p_rwlock->attr);

    return 0;
}

unsigned int 
tf_net_rwlock_rd_trylock(TF_NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    return pthread_rwlock_tryrdlock(&p_rwlock->m); 
}

unsigned int 
tf_net_rwlock_wr_trylock(TF_NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    return pthread_rwlock_trywrlock(&p_rwlock->m); 
}

unsigned int 
tf_net_rwlock_rd_lock(TF_NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_rdlock(&p_rwlock->m); 
    
    return 0;
}

unsigned int 
tf_net_rwlock_rd_timelock(TF_NET_RWLOCK *p_rwlock, struct timespec *p_timeout)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_timedrdlock(&p_rwlock->m, p_timeout);
    
    return 0;
}

unsigned int 
tf_net_rwlock_wr_lock(TF_NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_wrlock(&p_rwlock->m); 
    
    return 0;
}

unsigned int 
tf_net_rwlock_wr_timelock(TF_NET_RWLOCK *p_rwlock, struct timespec *p_timeout)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_timedwrlock(&p_rwlock->m, p_timeout);
    
    return 0;
}

unsigned int
tf_net_rwlock_unlock(TF_NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL; 
    }
    
    pthread_rwlock_unlock(&p_rwlock->m);
    return 0;
}

#endif

#if DEFUNC("�ڴ�ؽӿ�")

/* �ڴ���������� */
static TF_NET_MUTUX g_tf_net_mp_res_lock = PTHREAD_MUTEX_INITIALIZER;

static unsigned int  g_tf_net_total_blk_pool_size;

/*�ڴ��������*/
static STAILQ_HEAD(, TF_NET_BLK_POOL) g_tf_net_blk_pool_list = STAILQ_HEAD_INITIALIZER(g_tf_net_blk_pool_list);

//�������ϼ���֤
static void 
tf_net_blk_pool_reset(TF_NET_BLK_POOL *pool)
{
    unsigned int   blk_size;
    TF_NET_MEMBLK *blk;
    unsigned int   i;

    STAILQ_INIT(&pool->free_list);

    blk_size = pool->parm.blk_size + sizeof(TF_NET_MEMBLK);
    blk_size = TF_NET_ROUND_UP(blk_size, sizeof(void *));

    /* Put all blocks on free list */
    blk = (TF_NET_MEMBLK *)pool->start;
    for (i = 0; i < pool->parm.num_blks; i++)
    {
        blk->pool = pool;
        STAILQ_INSERT_TAIL(&pool->free_list, blk, next);
        blk = (TF_NET_MEMBLK *)((long)blk + blk_size);
    }

    /* Init statistics */
    memset(&pool->stat, 0, sizeof(pool->stat));
    pool->stat.free = pool->parm.num_blks;
}

/*
�ڴ����ÿһ���ڴ��С�̶���
ÿһ���ڴ�Ϊ4�ֽڵ��������� 
*/
unsigned int 
tf_net_blk_pool_create(TF_NET_BLK_POOL *pool, const TF_NET_BLK_POOL_PARM *parm)
{
    unsigned int blk_size;

    if (!pool || !parm)
    {
        printf("Parameter is null !\n");
        return TF_MDW_NET_RC_PARAM_NULL; 
    }

    if ( !parm->blk_size || !parm->num_blks)
    {
        printf("blk_size=%u, num_blks=%u\n", parm->blk_size, parm->num_blks);
        return TF_MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }
  
    memset(pool, 0, sizeof(*(pool)));
    pool->parm = *parm;

    if (pool->parm.name)
    {
        strncpy(pool->name, pool->parm.name, sizeof(pool->name) - 1);
        pool->parm.name = pool->name;
    }

    /*
    calculate total block size in bytes, including overheads
    */
    pool->parm.blk_size = TF_NET_ROUND_UP(pool->parm.blk_size, sizeof(unsigned int));
    blk_size = pool->parm.blk_size + sizeof(TF_NET_MEMBLK);

    blk_size = TF_NET_ROUND_UP(blk_size, sizeof(void *));

    pool->parm.pool_size = pool->parm.num_blks * blk_size;

    /* Allocate memory for the pool*/
    pool->start = malloc(pool->parm.pool_size);
    if (!pool->start)
    {
        printf("Can't allocate memory for block pool %s\n", parm->name);
        return TF_MDW_NET_RC_MEM_ALLOCATE;
    }

    tf_net_mutex_create(&pool->lock);

    /* Put all blocks on free list */
    tf_net_blk_pool_reset(pool);

    pool->status = TF_TF_NET_BLK_POOL_VALID;

    tf_net_mutex_lock(&g_tf_net_mp_res_lock);
    STAILQ_INSERT_TAIL(&g_tf_net_blk_pool_list, pool, list);
    g_tf_net_total_blk_pool_size += pool->parm.pool_size;
    tf_net_mutex_unlock(&g_tf_net_mp_res_lock);

    return 0;
}

unsigned int 
tf_net_blk_pool_destroy(TF_NET_BLK_POOL *pool)
{
    if (!pool)
    {
        printf("pool handle is invalid\n");
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    if ( pool->status != TF_TF_NET_BLK_POOL_VALID || pool->stat.free < pool->parm.num_blks)
    {
        printf("pool status %d, %i blocks are still allocated from the pool %s\n",
            pool->status, pool->parm.num_blks - pool->stat.free, pool->parm.name);
        return TF_MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    tf_net_mutex_lock(&g_tf_net_mp_res_lock);
    STAILQ_REMOVE(&g_tf_net_blk_pool_list, pool, TF_NET_BLK_POOL, list);
    g_tf_net_total_blk_pool_size -= pool->parm.pool_size;
    tf_net_mutex_unlock(&g_tf_net_mp_res_lock);

    if (pool->start)
    {
        free(pool->start);
    }
    pool->status = TF_TF_NET_BLK_POOL_DELETED;

    return 0;
}

/*
����ӿ�ÿ������һ��̶���С���ڴ�
*/
void*
tf_net_blk_pool_alloc(TF_NET_BLK_POOL *pool)
{
    TF_NET_MEMBLK *blk;

    if (!pool)
    {
        printf("pool handle is invalid\n");
        return NULL;
    }

    tf_net_mutex_lock(&pool->lock);
    blk = STAILQ_FIRST(&pool->free_list);
    if (blk)
    {
        STAILQ_REMOVE_HEAD(&pool->free_list, next);
        ++pool->stat.allocated;
        tf_net_mutex_unlock(&pool->lock);
        return (void *)(blk + 1);
    }

    printf("blk pool alloc fail!!!\n");

    /* No memory */
    ++pool->stat.alloc_failed;
    tf_net_mutex_unlock(&pool->lock);
    return NULL;
}

void*
tf_net_blk_pool_calloc(TF_NET_BLK_POOL *pool)
{
    if (!pool)
    {
        printf("pool handle is invalid\n");
        return NULL;
    }

    void *ptr = tf_net_blk_pool_alloc(pool);
    if (ptr)
    {
        memset(ptr, 0, pool->parm.blk_size);
    }
    return ptr;
}

//ͬfree���κϷ������ϼ�����
void 
tf_net_blk_pool_free(void *ptr)
{
    TF_NET_MEMBLK   *blk;
    TF_NET_BLK_POOL *pool;

    blk = (TF_NET_MEMBLK *)((long)ptr - sizeof(TF_NET_MEMBLK));
    pool = blk->pool;

    tf_net_mutex_lock(&pool->lock);
    STAILQ_INSERT_HEAD(&pool->free_list, blk, next);
    ++pool->stat.released;
    tf_net_mutex_unlock(&pool->lock);
}

unsigned int 
tf_net_blk_pool_query(const TF_NET_BLK_POOL *pool, TF_NET_BLK_POOL_INFO *info)
{
    if (!pool || !info)
    {
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    info->parm = pool->parm;
    info->stat = pool->stat;
    info->stat.free = pool->parm.num_blks - (info->stat.allocated - info->stat.released);
    return 0;
}

unsigned int 
tf_net_blk_pool_get_next(const TF_NET_BLK_POOL **prev)
{
    const TF_NET_BLK_POOL *pool;

    if (prev == NULL)
    {
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    pool = *prev;

    if (pool && (pool->status != TF_TF_NET_BLK_POOL_VALID))
    {
        return TF_MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }
    
    if (pool)
    {
        pool = STAILQ_NEXT(pool, list);
    }
    else
    {
        pool = STAILQ_FIRST(&g_tf_net_blk_pool_list);
    }
    
    *prev = pool;
    if (!pool)
    {
        return TF_MDW_NET_RC_PARAM_NULL;
    }
    
    return 0;
}

void*
tf_net_malloc(TF_NET_BLK_POOL* p_mem_pool, unsigned int size)
{
    void* p_buffer;
#ifdef TF_NET_USE_MP
    if (!p_mem_pool || p_mem_pool->parm.blk_size < size)
    {    
        return NULL;
    }

    p_buffer = (void*)tf_net_blk_pool_alloc(p_mem_pool);
#else
    p_buffer = (void*)malloc(size);
#endif

    return p_buffer;
}

void*
tf_net_calloc(TF_NET_BLK_POOL* p_mem_pool, unsigned int num, unsigned int size)
{
    void* p_buffer;
#ifdef TF_NET_USE_MP
    if (!p_mem_pool || p_mem_pool->parm.blk_size < size)
    {
        return NULL;
    }

    p_buffer = (void*)tf_net_blk_pool_calloc(p_mem_pool);
#else
    p_buffer = (void*)calloc(num, size);
#endif

    return p_buffer;
}

void 
tf_net_safe_free(void* p_buffer)
{
#ifdef TF_NET_USE_MP
    SAFE_MP_FREE(p_buffer);
#else
    SAFE_FREE(p_buffer);
#endif

    return ;
}

#endif

#if DEFUNC("����ѹ���ӿ�")

//����ѹ��
unsigned int
tf_net_data_compress(
                            const void*             p_in, 
                            const unsigned long     in_len, 
                            void*                   p_out, 
                            unsigned long*          p_out_len,
                            unsigned int            level)
{
    int rc;

    if (!p_in || !p_out || !p_out_len)
    {
      return TF_MDW_NET_RC_PARAM_NULL;
    }

    if (!in_len)
    {
        return TF_MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    if ((rc = compress2(p_out, p_out_len, p_in, in_len, level)))
    {
        printf("tf_net_data_compress fail return %d\r\n", rc);
        return TF_MDW_NET_RC_COMPRESS_FAIL;
    }

    //ѹ����
    return (*p_out_len < in_len) ? TF_MDW_NET_RC_OK : TF_MDW_NET_RC_ERROR; 
}

//���Ľ�ѹ
unsigned int
tf_net_data_uncompress(
                            void*           p_in, 
                            unsigned long   in_len, 
                            void*           p_out, 
                            unsigned long*  p_out_len)
{
    int rc;
    
    if (!p_in || !p_out || !p_out_len)
    {
        return TF_MDW_NET_RC_PARAM_NULL;
    }

    if (!in_len)
    {
        return TF_MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    if ((rc = uncompress(p_out, p_out_len, p_in, in_len)))
    {
        printf("tf_net_data_uncompress fail return %d\r\n", rc);
        return TF_MDW_NET_RC_UNCOMPRESS_FAIL;
    }
    
    return 0;
}

#endif

#if DEFUNC("��Ϣ���нӿ�")

/* ��¼ÿ����Ϣռ�������0:δ��ʹ��1:�Ѿ���ʹ�� */
static unsigned char systemv_mq_used_flag[SYS_V_MQ_MAX_NUM];

unsigned int 
tf_net_systemv_mq_create (
                int             *p_queue_id, 
                SYS_V_MQ_ID_E   sub_key, 
                unsigned int    queue_size)
{
    int             tmp_skt;
    key_t           q_key;
    struct msqid_ds msg_ds;

    if(!p_queue_id || !queue_size) 
    {
        printf("Create queue failed cause some parameter is invalid.\r\n");
        return -1;
    }

    if(sub_key >= SYS_V_MQ_MAX_NUM || systemv_mq_used_flag[sub_key])
    {
        printf("Create queue with sub key %d failed cause wrong key or recreating. \r\n", sub_key);
        return -1;
    }
    
    systemv_mq_used_flag[sub_key] = 1;

    /* Set the possible task Id to not free so that  no other task can try to use it */
    q_key = ftok("/etc/", sub_key);
    if((tmp_skt = msgget(q_key, IPC_CREAT)) < 0)
    {
        printf("Create msgget failed.\r\n");
        return -1;
    }

    if(msgctl(tmp_skt, IPC_RMID, NULL) < 0)
    {
        printf("Delete queue failed.\r\n");
        return -1;
    }

    if((tmp_skt = msgget(q_key, IPC_CREAT)) < 0)
    {
        printf("Create msgget failed.\r\n");
        return -1;
    }

    /*Set msg queue max bytes*/
    msgctl(tmp_skt, IPC_STAT, &msg_ds);
    msg_ds.msg_qbytes = queue_size;

    if(msgctl(tmp_skt, IPC_SET, &msg_ds) < 0)
    {
        printf("Set msgq size failed.\r\n");
        return -1;
    }

    *p_queue_id = tmp_skt;

    return 0;
}

unsigned int
tf_net_systemv_mq_get (
                int             queue_id, 
                long            type, 
                void            *p_data, 
                unsigned int    size, 
                int             timeout, /* ms  */
                unsigned int    *p_size_copied)
{
    int size_copied;

    /* Check Parameters */
    if( (!p_data) || (!p_size_copied)) 
    {
        printf("Get msg failed cause some parameter is NULL.\r\n");
        return -1;
    }

    /* Read the socket for data */
    if (timeout == SYS_V_MQ_WAIT_FOREVER)
    {
        size_copied = msgrcv(queue_id, p_data, size, type, 0);

        if (size_copied <= 0)
        {
            *p_size_copied = 0;
            return -1;
        }
        else if(size_copied <= size )
        {
            *p_size_copied = size_copied;
            return 0;
        }
        else            
            return -1;
    }
    else if (timeout == SYS_V_MQ_NO_WAIT)
    {
        size_copied = msgrcv(queue_id, p_data, size, type, IPC_NOWAIT);
        if (size_copied <= 0)
        {
            *p_size_copied = 0;
            return -1;
        }
        else if(size_copied <= size )
        {
            *p_size_copied = size_copied;
            return 0;
        }
        else            
            return -1;
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

            if (size_copied <= 0)
            {
                /* Sleep for 100 milliseconds */
                usleep(100 * 1000);
            }
            else if ( size_copied <= size)
            {
                *p_size_copied = size_copied;
                return 0;
            }
            else
            return -1;
        }

        return -1;

    } /* END timeout */

    /* Should never really get here. */
    return 0;
}

unsigned int
tf_net_systemv_mq_put (
                int             queue_id, 
                void            *p_data, 
                unsigned int    size, 
                int             timeout)
{
    /* Check Parameters */
    if (!p_data) 
    {
        printf("Can not put NULL msg into the queue. \r\n");
        return -1;
    }

    if(msgsnd(queue_id, p_data, size, (SYS_V_MQ_NO_WAIT == timeout) ? IPC_NOWAIT : 0) < 0)
    {
        printf("Send msg failed.\r\n");
        return -1;
    }

    return 0;
}

//������Ϣ�ͷ�
void 
tf_net_zc_mq_os_msg_free(TF_NET_OS_MSG *p_msg)
{
    //�����Ϣ&������Ϣ
    tf_net_safe_free(p_msg->data);
    tf_net_safe_free(p_msg);
}

static void 
tf_net_zc_mq_st_init(TF_NET_ZC_MQ_ST *p_queue)
{
    STAILQ_INIT(&p_queue->msgl);
    tf_net_mutex_create(&p_queue->lock);
    p_queue->msg_in = 0;
    
    return ;
}

static void 
tf_net_zc_mq_st_destroy(TF_NET_ZC_MQ_ST *p_queue)
{ 
    TF_NET_OS_MSG *p_msg, *p_tmp_msg;
    STAILQ_FOREACH_SAFE(p_msg, &p_queue->msgl, next, p_tmp_msg)
    {
        STAILQ_REMOVE_HEAD(&p_queue->msgl, next);
        tf_net_zc_mq_os_msg_free(p_msg);
    }

    return ;
}

unsigned int
tf_net_zc_mq_create(TF_NET_ZC_MQ *p_queue, const char *p_name)
{
    unsigned int rc;

    if (!p_queue || !p_name)
    {
        printf("queue %p, parm %p\n", p_queue, p_name);
        return TF_MDW_NET_RC_PARAM_NULL;
    }
 
    memset(p_queue, 0, sizeof(*p_queue));

    strncpy(p_queue->name, p_name, sizeof(p_queue->name) - 1);

    rc = tf_net_sem_create(&p_queue->m, 0);
    if (rc)
    {
        printf("tf_net_zc_mq_create fail\r\n");
        return rc; 
    }

    tf_net_zc_mq_st_init(&p_queue->q);

    p_queue->state = TF_NET_ZC_MSG_QUEUE_VALID;
   
    return 0;
}

unsigned int 
tf_net_zc_mq_destroy(TF_NET_ZC_MQ *p_queue)
{
    if (!p_queue || p_queue->state != TF_NET_ZC_MSG_QUEUE_VALID)
    {
        printf("queue handle is invalid\r\n");
        return TF_MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }
    
    p_queue->state = TF_NET_ZC_MSG_QUEUE_DELETED;
    
    tf_net_sem_destroy(&p_queue->m);

    tf_net_zc_mq_st_destroy(&p_queue->q);

    return 0;
}

void
tf_net_zc_mq_os_msg_put(TF_NET_ZC_MQ *p_queue, TF_NET_OS_MSG *p_msg)
{
    unsigned int rc;

    tf_net_mutex_lock(&p_queue->q.lock);
   
    STAILQ_INSERT_TAIL(&p_queue->q.msgl, p_msg, next);
    ++p_queue->q.msg_in;

    if (p_queue->is_waiting)
    {
        p_queue->is_waiting = 0;
        tf_net_mutex_unlock(&p_queue->q.lock);
        tf_net_sem_post(&p_queue->m);
    }
    else
    {
        tf_net_mutex_unlock(&p_queue->q.lock);
    }

    return ;
}

static TF_NET_OS_MSG *
_tf_net_zc_mq_os_msg_get(TF_NET_ZC_MQ_ST *p_queue)
{
    TF_NET_OS_MSG *p_msg;

    p_msg = STAILQ_FIRST(&p_queue->msgl);
    if (p_msg)
    {
        STAILQ_REMOVE_HEAD(&p_queue->msgl, next);
        --p_queue->msg_in;
    }

    return p_msg;
}

unsigned int 
tf_net_zc_mq_os_msg_get(
                                TF_NET_ZC_MQ   *p_queue, 
                                unsigned int    timeout, 
                                TF_NET_OS_MSG  **p_msg)
{
    if (!p_queue || !p_msg)
    {
        printf("queue %p, msg %p\n", p_queue, p_msg);
        return -1;
    }

    tf_net_mutex_lock(&p_queue->q.lock);
    *p_msg = _tf_net_zc_mq_os_msg_get(&p_queue->q);
    if (*p_msg)
    {
        tf_net_mutex_unlock(&p_queue->q.lock);
        return 0;
    }
    
    if (!timeout)
    {
        tf_net_mutex_unlock(&p_queue->q.lock);
        return -1;
    }

    /* Receive with wait */
    p_queue->is_waiting = 1;
    tf_net_mutex_unlock(&p_queue->q.lock);

    /* wait for it */
    tf_net_sem_wait(&p_queue->m, timeout);
    tf_net_mutex_lock(&p_queue->q.lock);
    *p_msg = _tf_net_zc_mq_os_msg_get(&p_queue->q);
    p_queue->is_waiting = 0;
    tf_net_mutex_unlock(&p_queue->q.lock);
    if (!*p_msg)
    {
        printf("tf_net_zc_mq_os_msg_get timeout\r\n");
        return TF_MDW_NET_RC_PARAM_NULL;   
    }

    return 0;
}

#endif

#if DEFUNC("���Թ��߽ӿ�")

unsigned int 
tf_net_tid_get(void)  
{  
   pid_t tid;
   
   tid = syscall(SYS_gettid);
   
   return tid;
}

//��ȡ�����ڴ�ʹ���ʣ�0~100 
unsigned int 
tf_net_mem_load_get(unsigned int *p_occupy)
{
    FILE                *fd;                         
    char                buff[256] = {0};
    TF_NET_MEM_OCCUPY  m = {0};
    
    fd = fopen ("/proc/meminfo", "r");
    fseek(fd, 0, SEEK_SET);
    
    //���ڴ�
    if (feof(fd) || fgets(buff, sizeof(buff), fd) == NULL)
    {
        goto fail;
    }
    
    sscanf (buff, "%s %lu %s", m.name_total, &m.total, m.name_total_uint);
    
    //�����ڴ�
    if (feof(fd) || fgets(buff, sizeof(buff), fd) == NULL)
    {
        goto fail;
    }

    sscanf (buff, "%s %lu %s", m.name_free, &m.free, m.name_free_uint);

    //printf("mem total %lu free %lu\r\n", m.total, m.free);
    
    if (m.total && (m.total >= m.free))
    {
        *p_occupy = (float)(m.total - m.free)*100/m.total;
    }
    else
    {
        goto fail;
    }

    fclose(fd);
    return 0;

fail:

    fclose(fd);
    return TF_MDW_NET_RC_ERROR;
}

//��ȡ����CPUʱ��
unsigned int 
tf_net_cpu_time_get(TF_NET_CPU_OCCUPY *p_time)
{         
    FILE    *fd;                         
    char    buff[256] = {0};
    
    fd = fopen ("/proc/stat", "r");
    fseek(fd, 0, SEEK_SET);

    if (feof(fd) || fgets(buff, sizeof(buff), fd) == NULL)
    {
        goto fail;
    }
    
    sscanf (buff, "%s %lu %lu %lu %lu %lu %lu %lu", 
            p_time->name, &p_time->user, &p_time->nice,&p_time->system, 
            &p_time->idle, &p_time->iowait, &p_time->irq, &p_time->softirq);
            
    fclose(fd);

    return 0;
    
fail:

    fclose(fd); 
    return TF_MDW_NET_RC_ERROR;
}

//��ȡ����CPUʹ���ʣ�0~100
unsigned int 
tf_net_cpu_cal_occupy(TF_NET_CPU_OCCUPY *p_old, TF_NET_CPU_OCCUPY *p_new, unsigned int *p_cpu_use)  
{         
    unsigned long old_time, new_time;         
    unsigned long idle_delay, time_delay;     
    
    old_time   = p_old->user + p_old->nice + p_old->system + p_old->idle + p_old->iowait + p_old->irq + p_old->softirq;   
    new_time   = p_new->user + p_new->nice + p_new->system + p_new->idle + p_new->iowait + p_new->irq + p_new->softirq;
    idle_delay = p_new->idle - p_old->idle;
    time_delay = new_time - old_time;
    
    if(time_delay) 
    {
        *p_cpu_use = 100 - (float)idle_delay*100/time_delay;
        //printf("cpu load pre time %lu current %lu idle %lu total %lu \r\n", old_time, new_time, idle_delay, time_delay);
        return 0;
    }
    
    return TF_MDW_NET_RC_ERROR;
}

#endif

#if DEFUNC("λ�����ӿ�")

static const unsigned char g_tf_net_bitmask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

void 
tf_net_bitlist_set(unsigned char *p_pit_list, unsigned int bit)
{
    p_pit_list[bit/8] |= g_tf_net_bitmask[bit%8];
}

void 
tf_net_bitlist_clear(unsigned char *p_pit_list, unsigned int bit)
{    
    p_pit_list[bit/8] &= ~g_tf_net_bitmask[bit%8];
}

unsigned char 
tf_net_bitlist_check(unsigned char *p_pit_list, unsigned int bit)
{    
    return (p_pit_list[bit/8] & g_tf_net_bitmask[bit%8])? 1 : 0;
}

unsigned int 
tf_net_bitlist_check_all(unsigned char *p_pit_list, unsigned int bit_list_len)
{    
    unsigned int i, found = 0, byte_count;
    
    /* Figure out the real number of bytes to scan */
    byte_count = ((bit_list_len%8) == 0) ? (bit_list_len/8) : (bit_list_len/8) + 1;    
    
    /* Add up all of the bytes to determine if the p_pit_list is empty. */    
    for( i = 0; i < byte_count; i++ )        
        found += p_pit_list[i];
    
    return found;
}

#endif

#if DEFUNC("�����ļ����ӿ�")

static void 
tf_net_config_file_arg_get(
                        char            *p_arg, 
                        unsigned int    arg_size, 
                        char            **p_content)  
{  
    char    *p_arg_tmp     = NULL;  
    char    *p_content_tmp = NULL;  
    int     quote = 0;  
  
    p_content_tmp = *p_content;  
    
    while(isspace(*p_content_tmp))  
        p_content_tmp++;  
        
    p_arg_tmp = p_arg;

    //�������
    if (*p_content_tmp == '\"' || *p_content_tmp == '\'')  
        quote = *p_content_tmp++;  
        
    while(1)  
    {  
        //���������ŷָ���ųɶԳ���
        if (quote && quote == *p_content_tmp)  
        {  
            p_content_tmp++;  
            break;  
        }  

        //�����Կո�ָ�
        if (quote == 0 && (isspace(*p_content_tmp) || *p_content_tmp =='\0'))  
            break;  
            
        if ((p_arg_tmp - p_arg) < (arg_size - 1))  
            *p_arg_tmp++ = *p_content_tmp++;  
    }
    
    *p_arg_tmp = '\0';  
    *p_content = p_content_tmp;

    return ;
}  
  
unsigned int 
tf_net_config_file_paser(
                    const char          *p_file_name, 
                    const char          *p_target_name, 
                    char*               p_target,
                    const unsigned int  target_len)  
{  
    FILE                *fp;
    char                line_buf[512]  = {0};  
    char                key[128]       = {0};
    char                *p             = NULL;
    unsigned int        find_flag = 0;
    
    if ((fp = fopen(p_file_name, "r")) == NULL)  
    {  
        printf("tf_net_config_file_paser fopen() failed\r\n");  
        return TF_MDW_NET_RC_ERROR;
    }

    fseek(fp, 0, SEEK_SET);
   
    while(1)  
    {  
        if (feof(fp) || fgets(line_buf, sizeof(line_buf), fp) == NULL)
            break;
            
        p = line_buf;
        
        while(isspace(*p))  
            p++;

        //# ע��    
        if (*p == '\0' || *p == '#')  
            continue;
            
        //ÿһ��һ��key-vlaue
        tf_net_config_file_arg_get(key, sizeof(key), &p);  

        if (!strncmp(key, p_target_name, strlen(p_target_name)))
        {
            tf_net_config_file_arg_get(p_target, target_len, &p);
            find_flag = 1;
            break;
        }
        
    }

    fclose(fp);
    
    return (find_flag == 1) ? 0 : TF_MDW_NET_RC_PARAM_OUT_OF_RANGE;  
}  

#endif

