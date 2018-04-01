/**************************************************************
* 文件名称: 
* 文件描述:
* 版           本: 
* 修改历史:
*     <修改者>     <时间>      <版本 >     <描述>

**************************************************************/

#include "adaptor_common_if.h"
#include "adaptor_errno.h"
#include <sys/syscall.h> 
#include <ctype.h>
#include "zlib.h"


#if DEFUNC("时间基本接口")

void 
net_timewait(int sec, int us)
{
    struct timeval tv = {.tv_sec = sec, .tv_usec = us};
    int            ret;

    do
    {
        ret = select(0, NULL, NULL, NULL, &tv);
    }while(ret < 0 && errno == EINTR);

    return ;  
}

/* 
Get current timestamp
returns the current system timestamp (us)
 */
unsigned int 
net_timestamp(void)
{
    struct timespec tp;
    
    clock_gettime(CLOCK_MONOTONIC, &tp);
    
    return (tp.tv_sec * 1000000 + tp.tv_nsec / 1000);
}

/* calculate absolute time equal to the current time + timeout */
void 
net_abs_time_get(struct timespec *pts, int timeout)
{ 
    clock_gettime(CLOCK_REALTIME, pts);

    pts->tv_sec += timeout / 1000000;
    pts->tv_nsec += (timeout % 1000000) * 1000;
    if (pts->tv_nsec > 1000000000)
    {
        pts->tv_sec += pts->tv_nsec / 1000000000;
        pts->tv_nsec %= 1000000000;
    }
}

#endif

#if DEFUNC("信号量基本接口")

/* Create semaphore */
unsigned int 
net_sem_create(NET_SEM *psem, int count)
{
    if (!psem)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    sem_init(&psem->s, 0, count);
    return 0;
}

/* Destroy semaphore */
unsigned int
net_sem_destroy(NET_SEM *psem)
{
    if (!psem)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    sem_destroy(&psem->s);

    return 0;
}

/* Decrement semaphore counter. Wait if the counter is 0 */
unsigned int 
net_sem_wait(NET_SEM *psem, int timeout)
{
    int rc;
    struct timespec ts;

    if (!psem)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }

    /* Init end time if necessary */
    if (timeout && timeout != NET_WAIT_FOREVER)
    {
        net_abs_time_get(&ts, timeout);
    }

    do
    {
        if (timeout == NET_WAIT_FOREVER)
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

        return MDW_NET_RC_MSG_SEM_WAIT;
    }

    return 0;
}

/* Increment semaphore counter */
unsigned int 
net_sem_post(NET_SEM *psem)
{
    if (!psem)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    sem_post(&psem->s);

    return 0;
}

#endif

#if DEFUNC("锁基本接口")

//互斥锁
/* Create recursive mutex */
unsigned int 
net_mutex_create(NET_MUTUX *pmutex)
{
    int err;

    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    err = pthread_mutexattr_init(&pmutex->attr);
    err = err ? err : pthread_mutexattr_settype(&pmutex->attr, PTHREAD_MUTEX_RECURSIVE_NP);
    err = err ? err : pthread_mutex_init(&pmutex->m, &pmutex->attr);
    
    if (err)
    {
        printf("net_mutex_create error!!!\n");
        return MDW_NET_RC_MUTEX_CREATE;
    }
  
    return 0;
}

/*Destroy mutex*/
unsigned int net_mutux_destory(NET_MUTUX *pmutex)
{
    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_mutex_destroy(&pmutex->m);
    pthread_mutexattr_destroy(&pmutex->attr);

    return 0;
}

unsigned int 
net_mutex_trylock(NET_MUTUX *pmutex)
{
    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    return pthread_mutex_trylock(&pmutex->m); 
}

/*Lock mutex*/
unsigned int 
net_mutex_lock(NET_MUTUX *pmutex)
{
    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_mutex_lock(&pmutex->m); 
    
    return 0;
}

/* Release mutex*/
unsigned int
net_mutex_unlock(NET_MUTUX *pmutex)
{
    if (!pmutex)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL; 
    }
    
    pthread_mutex_unlock(&pmutex->m);
    return 0;
}

//都写锁
unsigned int 
net_rwlock_create(NET_RWLOCK *p_rwlock)
{
    int err;

    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    err = pthread_rwlockattr_init(&p_rwlock->attr);
    err = err ? err : pthread_rwlockattr_setpshared(&p_rwlock->attr, PTHREAD_PROCESS_PRIVATE);
    err = err ? err : pthread_rwlock_init(&p_rwlock->m, &p_rwlock->attr);
    
    if (err)
    {
        printf("net_rwlock_create error!!!\n");
        return MDW_NET_RC_RWLOCK_CREATE;
    }
  
    return 0;
}

unsigned int 
net_rwlock_destory(NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_destroy(&p_rwlock->m);
    pthread_rwlockattr_destroy(&p_rwlock->attr);

    return 0;
}

unsigned int 
net_rwlock_rd_trylock(NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    return pthread_rwlock_tryrdlock(&p_rwlock->m); 
}

unsigned int 
net_rwlock_wr_trylock(NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    return pthread_rwlock_trywrlock(&p_rwlock->m); 
}

unsigned int 
net_rwlock_rd_lock(NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_rdlock(&p_rwlock->m); 
    
    return 0;
}

unsigned int 
net_rwlock_rd_timelock(NET_RWLOCK *p_rwlock, struct timespec *p_timeout)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_timedrdlock(&p_rwlock->m, p_timeout);
    
    return 0;
}

unsigned int 
net_rwlock_wr_lock(NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_wrlock(&p_rwlock->m); 
    
    return 0;
}

unsigned int 
net_rwlock_wr_timelock(NET_RWLOCK *p_rwlock, struct timespec *p_timeout)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL;
    }
    
    pthread_rwlock_timedwrlock(&p_rwlock->m, p_timeout);
    
    return 0;
}

unsigned int
net_rwlock_unlock(NET_RWLOCK *p_rwlock)
{
    if (!p_rwlock)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL; 
    }
    
    pthread_rwlock_unlock(&p_rwlock->m);
    return 0;
}

#endif

#if DEFUNC("内存池接口")

/* 内存池链队列锁 */
static NET_MUTUX g_net_mp_res_lock = {.m = PTHREAD_MUTEX_INITIALIZER};

static unsigned int  g_net_total_blk_pool_size;

/*内存池链队列*/
static STAILQ_HEAD(, NET_BLK_POOL) g_net_blk_pool_list = STAILQ_HEAD_INITIALIZER(g_net_blk_pool_list);

//参数由上级保证
static void 
net_blk_pool_reset(NET_BLK_POOL *pool)
{
    unsigned int  blk_size;
    NET_MEMBLK    *blk;
    unsigned int  i;

    STAILQ_INIT(&pool->free_list);

    blk_size = pool->parm.blk_size + sizeof(NET_MEMBLK);
    blk_size = NET_ROUND_UP(blk_size, sizeof(void *));

    /* Put all blocks on free list */
    blk = (NET_MEMBLK *)pool->start;
    for (i = 0; i < pool->parm.num_blks; i++)
    {
        blk->pool = pool;
        STAILQ_INSERT_TAIL(&pool->free_list, blk, next);
        blk = (NET_MEMBLK *)((long)blk + blk_size);
    }

    /* Init statistics */
    memset(&pool->stat, 0, sizeof(pool->stat));
    pool->stat.free = pool->parm.num_blks;
}

/*
内存池中每一块内存大小固定，
每一块内存为4字节的整数倍， 
*/
unsigned int 
net_blk_pool_create(NET_BLK_POOL *pool, const NET_BLK_POOL_PARM *parm)
{
    unsigned int blk_size;

    if (!pool || !parm)
    {
        printf("Parameter is null !\n");
        return MDW_NET_RC_PARAM_NULL; 
    }

    if ( !parm->blk_size || !parm->num_blks)
    {
        printf("blk_size=%u, num_blks=%u\n", parm->blk_size, parm->num_blks);
        return MDW_NET_RC_PARAM_OUT_OF_RANGE;
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
    pool->parm.blk_size = NET_ROUND_UP(pool->parm.blk_size, sizeof(unsigned int));
    blk_size = pool->parm.blk_size + sizeof(NET_MEMBLK);

    blk_size = NET_ROUND_UP(blk_size, sizeof(void *));

    pool->parm.pool_size = pool->parm.num_blks * blk_size;

    /* Allocate memory for the pool*/
    pool->start = malloc(pool->parm.pool_size);
    if (!pool->start)
    {
        printf("Can't allocate memory for block pool %s\n", parm->name);
        return MDW_NET_RC_MEM_ALLOCATE;
    }

    net_mutex_create(&pool->lock);

    /* Put all blocks on free list */
    net_blk_pool_reset(pool);

    pool->status = NET_BLK_POOL_VALID;

    net_mutex_lock(&g_net_mp_res_lock);
    STAILQ_INSERT_TAIL(&g_net_blk_pool_list, pool, list);
    g_net_total_blk_pool_size += pool->parm.pool_size;
    net_mutex_unlock(&g_net_mp_res_lock);

    return 0;
}

unsigned int 
net_blk_pool_destroy(NET_BLK_POOL *pool)
{
    if (!pool)
    {
        printf("pool handle is invalid\n");
        return MDW_NET_RC_PARAM_NULL;
    }

    if ( pool->status != NET_BLK_POOL_VALID || pool->stat.free < pool->parm.num_blks)
    {
        printf("pool status %d, %i blocks are still allocated from the pool %s\n",
            pool->status, pool->parm.num_blks - pool->stat.free, pool->parm.name);
        return MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    net_mutex_lock(&g_net_mp_res_lock);
    STAILQ_REMOVE(&g_net_blk_pool_list, pool, NET_BLK_POOL, list);
    g_net_total_blk_pool_size -= pool->parm.pool_size;
    net_mutex_unlock(&g_net_mp_res_lock);

    if (pool->start)
    {
        free(pool->start);
    }
    pool->status = NET_BLK_POOL_DELETED;

    return 0;
}

/*
申请接口每次申请一块固定大小的内存
*/
void*
net_blk_pool_alloc(NET_BLK_POOL *pool)
{
    NET_MEMBLK *blk;

    if (!pool)
    {
        printf("pool handle is invalid\n");
        return NULL;
    }

    net_mutex_lock(&pool->lock);
    blk = STAILQ_FIRST(&pool->free_list);
    if (blk)
    {
        STAILQ_REMOVE_HEAD(&pool->free_list, next);
        ++pool->stat.allocated;
        net_mutex_unlock(&pool->lock);
        return (void *)(blk + 1);
    }

    printf("blk pool alloc fail!!!\n");

    /* No memory */
    ++pool->stat.alloc_failed;
    net_mutex_unlock(&pool->lock);
    return NULL;
}

void*
net_blk_pool_calloc(NET_BLK_POOL *pool)
{
    if (!pool)
    {
        printf("pool handle is invalid\n");
        return NULL;
    }

    void *ptr = net_blk_pool_alloc(pool);
    if (ptr)
    {
        memset(ptr, 0, pool->parm.blk_size);
    }
    return ptr;
}

//同free传参合法性由上级保障
void 
net_blk_pool_free(void *ptr)
{
    NET_MEMBLK   *blk;
    NET_BLK_POOL *pool;

    blk = (NET_MEMBLK *)((long)ptr - sizeof(NET_MEMBLK));
    pool = blk->pool;

    net_mutex_lock(&pool->lock);
    STAILQ_INSERT_HEAD(&pool->free_list, blk, next);
    ++pool->stat.released;
    net_mutex_unlock(&pool->lock);
}

unsigned int 
net_blk_pool_query(const NET_BLK_POOL *pool, NET_BLK_POOL_INFO *info)
{
    if (!pool || !info)
    {
        return MDW_NET_RC_PARAM_NULL;
    }

    info->parm = pool->parm;
    info->stat = pool->stat;
    info->stat.free = pool->parm.num_blks - (info->stat.allocated - info->stat.released);
    return 0;
}

unsigned int 
net_blk_pool_get_next(const NET_BLK_POOL **prev)
{
    const NET_BLK_POOL *pool;

    if (prev == NULL)
    {
        return MDW_NET_RC_PARAM_NULL;
    }
    
    pool = *prev;

    if (pool && (pool->status != NET_BLK_POOL_VALID))
    {
        return MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }
    
    if (pool)
    {
        pool = STAILQ_NEXT(pool, list);
    }
    else
    {
        pool = STAILQ_FIRST(&g_net_blk_pool_list);
    }
    
    *prev = pool;
    if (!pool)
    {
        return MDW_NET_RC_PARAM_NULL;
    }
    
    return 0;
}

void*
net_malloc(NET_BLK_POOL* p_mem_pool, unsigned int size)
{
    void* p_buffer;
#ifdef NET_USE_MP
    if (!p_mem_pool || p_mem_pool->parm.blk_size < size)
    {    
        return NULL;
    }

    p_buffer = (void*)net_blk_pool_alloc(p_mem_pool);
#else
    p_buffer = (void*)malloc(size);
#endif

    return p_buffer;
}

void*
net_calloc(NET_BLK_POOL* p_mem_pool, unsigned int num, unsigned int size)
{
    void* p_buffer;
#ifdef NET_USE_MP
    if (!p_mem_pool || p_mem_pool->parm.blk_size < size)
    {
        return NULL;
    }

    p_buffer = (void*)net_blk_pool_calloc(p_mem_pool);
#else
    p_buffer = (void*)calloc(num, size);
#endif

    return p_buffer;
}

void 
net_safe_free(void* p_buffer)
{
#ifdef NET_USE_MP
    SAFE_MP_FREE(p_buffer);
#else
    SAFE_FREE(p_buffer);
#endif

    return ;
}

#endif

#if DEFUNC("数据压缩接口")

//报文压缩
unsigned int
net_data_compress(
                            const void*             p_in, 
                            const unsigned long     in_len, 
                            void*                   p_out, 
                            unsigned long*          p_out_len,
                            unsigned int            level)
{
    int rc;

    if (!p_in || !p_out || !p_out_len)
    {
      return MDW_NET_RC_PARAM_NULL;
    }

    if (!in_len)
    {
        return MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    if ((rc = compress2(p_out, p_out_len, p_in, in_len, level)))
    {
        printf("net_data_compress fail return %d\r\n", rc);
        return MDW_NET_RC_COMPRESS_FAIL;
    }

    //压缩率
    return (*p_out_len < in_len) ? MDW_NET_RC_OK : MDW_NET_RC_ERROR; 
}

//报文解压
unsigned int
net_data_uncompress(
                            void*           p_in, 
                            unsigned long   in_len, 
                            void*           p_out, 
                            unsigned long*  p_out_len)
{
    int rc;
    
    if (!p_in || !p_out || !p_out_len)
    {
        return MDW_NET_RC_PARAM_NULL;
    }

    if (!in_len)
    {
        return MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }

    if ((rc = uncompress(p_out, p_out_len, p_in, in_len)))
    {
        printf("net_data_uncompress fail return %d\r\n", rc);
        return MDW_NET_RC_UNCOMPRESS_FAIL;
    }
    
    return 0;
}

#endif

#if DEFUNC("消息队列接口")

//队列消息释放
void 
net_zc_mq_os_msg_free(NET_OS_MSG *p_msg)
{
    //板间消息&队列消息
    net_safe_free(p_msg->data);
    net_safe_free(p_msg);
}

static void 
net_zc_mq_st_init(NET_ZC_MQ_ST *p_queue)
{
    STAILQ_INIT(&p_queue->msgl);
    net_mutex_create(&p_queue->lock);
    p_queue->msg_in = 0;
    
    return ;
}

static void 
net_zc_mq_st_destroy(NET_ZC_MQ_ST *p_queue)
{ 
    NET_OS_MSG *p_msg, *p_tmp_msg;
    STAILQ_FOREACH_SAFE(p_msg, &p_queue->msgl, next, p_tmp_msg)
    {
        STAILQ_REMOVE_HEAD(&p_queue->msgl, next);
        net_zc_mq_os_msg_free(p_msg);
    }

    return ;
}

unsigned int
net_zc_mq_create(NET_ZC_MQ *p_queue, const char *p_name)
{
    unsigned int rc;

    if (!p_queue || !p_name)
    {
        printf("queue %p, parm %p\n", p_queue, p_name);
        return MDW_NET_RC_PARAM_NULL;
    }
 
    memset(p_queue, 0, sizeof(*p_queue));

    strncpy(p_queue->name, p_name, sizeof(p_queue->name) - 1);

    rc = net_sem_create(&p_queue->m, 0);
    if (rc)
    {
        printf("net_zc_mq_create fail\r\n");
        return rc; 
    }

    net_zc_mq_st_init(&p_queue->q);

    p_queue->state = NET_ZC_MSG_QUEUE_VALID;
   
    return 0;
}

unsigned int 
net_zc_mq_destroy(NET_ZC_MQ *p_queue)
{
    if (!p_queue || p_queue->state != NET_ZC_MSG_QUEUE_VALID)
    {
        printf("queue handle is invalid\r\n");
        return MDW_NET_RC_PARAM_OUT_OF_RANGE;
    }
    
    p_queue->state = NET_ZC_MSG_QUEUE_DELETED;
    
    net_sem_destroy(&p_queue->m);

    net_zc_mq_st_destroy(&p_queue->q);

    return 0;
}

void
net_zc_mq_os_msg_put(NET_ZC_MQ *p_queue, NET_OS_MSG *p_msg)
{
    net_mutex_lock(&p_queue->q.lock);
   
    STAILQ_INSERT_TAIL(&p_queue->q.msgl, p_msg, next);
    ++p_queue->q.msg_in;

    if (p_queue->is_waiting)
    {
        p_queue->is_waiting = 0;
        net_mutex_unlock(&p_queue->q.lock);
        net_sem_post(&p_queue->m);
    }
    else
    {
        net_mutex_unlock(&p_queue->q.lock);
    }

    return ;
}

static NET_OS_MSG *
_net_zc_mq_os_msg_get(NET_ZC_MQ_ST *p_queue)
{
    NET_OS_MSG *p_msg;

    p_msg = STAILQ_FIRST(&p_queue->msgl);
    if (p_msg)
    {
        STAILQ_REMOVE_HEAD(&p_queue->msgl, next);
        --p_queue->msg_in;
    }

    return p_msg;
}

unsigned int 
net_zc_mq_os_msg_get(
                                NET_ZC_MQ   *p_queue, 
                                unsigned int    timeout, 
                                NET_OS_MSG  **p_msg)
{
    if (!p_queue || !p_msg)
    {
        printf("queue %p, msg %p\n", p_queue, p_msg);
        return -1;
    }

    net_mutex_lock(&p_queue->q.lock);
    *p_msg = _net_zc_mq_os_msg_get(&p_queue->q);
    if (*p_msg)
    {
        net_mutex_unlock(&p_queue->q.lock);
        return 0;
    }
    
    if (!timeout)
    {
        net_mutex_unlock(&p_queue->q.lock);
        return -1;
    }

    /* Receive with wait */
    p_queue->is_waiting = 1;
    net_mutex_unlock(&p_queue->q.lock);

    /* wait for it */
    net_sem_wait(&p_queue->m, timeout);
    net_mutex_lock(&p_queue->q.lock);
    *p_msg = _net_zc_mq_os_msg_get(&p_queue->q);
    p_queue->is_waiting = 0;
    net_mutex_unlock(&p_queue->q.lock);
    if (!*p_msg)
    {
        printf("net_zc_mq_os_msg_get timeout\r\n");
        return MDW_NET_RC_PARAM_NULL;   
    }

    return 0;
}

#endif

#if DEFUNC("调试工具接口")

unsigned int 
net_tid_get(void)  
{  
   pid_t tid;
   
   tid = syscall(SYS_gettid);
   
   return tid;
}

//获取单板内存使用率，0~100 
unsigned int 
net_mem_load_get(unsigned int *p_occupy)
{
    FILE            *fd;                         
    char            buff[256] = {0};
    NET_MEM_OCCUPY  m;

    bzero((void *)&m, sizeof(NET_MEM_OCCUPY));
    
    fd = fopen ("/proc/meminfo", "r");
    fseek(fd, 0, SEEK_SET);
    
    //总内存
    if (feof(fd) || fgets(buff, sizeof(buff), fd) == NULL)
    {
        goto fail;
    }
    
    sscanf (buff, "%s %lu %s", m.name_total, &m.total, m.name_total_uint);
    
    //空闲内存
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
    return MDW_NET_RC_ERROR;
}

//获取单板CPU时间
unsigned int 
net_cpu_time_get(NET_CPU_OCCUPY *p_time)
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
    return MDW_NET_RC_ERROR;
}

//获取单板CPU使用率，0~100
unsigned int 
net_cpu_cal_occupy(NET_CPU_OCCUPY *p_old, NET_CPU_OCCUPY *p_new, unsigned int *p_cpu_use)  
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
    
    return MDW_NET_RC_ERROR;
}

#endif

#if DEFUNC("位操作接口")

static const unsigned char g_net_bitmask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

void 
net_bitlist_set(unsigned char *p_pit_list, unsigned int bit)
{
    p_pit_list[bit/8] |= g_net_bitmask[bit%8];
}

void 
net_bitlist_clear(unsigned char *p_pit_list, unsigned int bit)
{    
    p_pit_list[bit/8] &= ~g_net_bitmask[bit%8];
}

unsigned char 
net_bitlist_check(unsigned char *p_pit_list, unsigned int bit)
{    
    return (p_pit_list[bit/8] & g_net_bitmask[bit%8])? 1 : 0;
}

unsigned int 
net_bitlist_check_all(unsigned char *p_pit_list, unsigned int bit_list_len)
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

#if DEFUNC("配置文件读接口")

static void 
net_config_file_arg_get(
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

    //标记引号
    if (*p_content_tmp == '\"' || *p_content_tmp == '\'')  
        quote = *p_content_tmp++;  
        
    while(1)  
    {  
        //参数以引号分割，引号成对出现
        if (quote && quote == *p_content_tmp)  
        {  
            p_content_tmp++;  
            break;  
        }  

        //参数以空格分割
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
net_config_file_paser(
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
        printf("net_config_file_paser fopen() failed\r\n");  
        return MDW_NET_RC_ERROR;
    }

    fseek(fp, 0, SEEK_SET);
   
    while(1)  
    {  
        if (feof(fp) || fgets(line_buf, sizeof(line_buf), fp) == NULL)
            break;
            
        p = line_buf;
        
        while(isspace(*p))  
            p++;

        //# 注释    
        if (*p == '\0' || *p == '#')  
            continue;
            
        //每一行一对key-vlaue
        net_config_file_arg_get(key, sizeof(key), &p);  

        if (!strncmp(key, p_target_name, strlen(p_target_name)))
        {
            net_config_file_arg_get(p_target, target_len, &p);
            find_flag = 1;
            break;
        }
        
    }

    fclose(fp);
    
    return (find_flag == 1) ? 0 : MDW_NET_RC_PARAM_OUT_OF_RANGE;  
}  

int binary_search(int *p_data, int length, int value)  
{  
    int left  = 0;
    int right = length - 1;  
    int middle;

    if (!p_data)
        return -1;
    
    while(left <= right)  
    {  
        //防止使用(left + right) / 2出现溢出,middle偏左
        middle = left + ((right - left)>>1);
        if(p_data[middle] == value)  
            return middle;  
        else if(p_data[middle] > value)  
            right = middle - 1;  
        else  
            left = middle + 1;  
    }
    
    return -1;  
}  

#endif

