/**************************************************************
* �ļ�����: 
* �ļ�����:
* ��           ��: 
* �޸���ʷ:
*     <�޸���>     <ʱ��>      <�汾 >     <����>

**************************************************************/

#ifndef __ADAPTOR_COMMON_IF__
#define __ADAPTOR_COMMON_IF__

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>

#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <malloc.h>

#include <sys/msg.h>


#ifndef DEFUNC
#define DEFUNC(x) 1
#endif

/*��־����ʹ�������*/
#define IN
#define OUT
#define INOUT

//���ư����֧��ҵ�����
#define NET_MANAGE_MAX_SLOT        1
#define DEFAULT_DEV_ID             0
#define MAX_SLOT_ID                NET_MANAGE_MAX_SLOT
#define SYS_MAX_EXIST_PORT_NUM        8
//���ư����������Ŀ
#define CTRL_SUPPORT_MAX_SLOT      2

#define MAX_NODE_EACH_FK               128
#define MAX_FK_EACH_SLOT               16

#define PHY_ID_2_PHY_UINT(fk_id, phy_id)   ((fk_id) * MAX_NODE_EACH_FK + (phy_id))
#define PHY_UINT_2_FK_ID(phy_uint)         ((phy_uint) / MAX_NODE_EACH_FK)
#define PHY_UINT_2_PHY_ID(phy_uint)        ((phy_uint) % MAX_NODE_EACH_FK)
#define MAX_PHY_UINT                       (PHY_ID_2_PHY_UINT(MAX_FK_EACH_SLOT-1, MAX_NODE_EACH_FK-1))
#define INVALID_PHY_UINT                     0xFFFFU
#define VALID_PHY_UINT(phy_uint)            ((phy_uint) <= MAX_PHY_UINT)

#define NODE_ID_2_NODE_UINT(fk_id, onu_id)   ((fk_id) * MAX_NODE_EACH_FK + (onu_id))
#define NODE_UINT_2_FK_ID(node_uint)         ((node_uint) / MAX_NODE_EACH_FK)
#define NODE_UINT_2_NODE_ID(node_uint)       ((node_uint) % MAX_NODE_EACH_FK)
#define MAX_NODE_UINT                        (NODE_ID_2_NODE_UINT(MAX_FK_EACH_SLOT-1, MAX_NODE_EACH_FK-1))
#define INVALID_NODE_UINT                     0xFFFFU
#define VALID_NODE_UINT(node_uint)           ((node_uint) <= MAX_NODE_UINT)


//�ڴ��ʹ�ÿ���
#define NET_USE_MP
//����ѹ��ʹ�ÿ���,����ѹ�����ڴ�����ʹ��
#define NET_USE_COMPRESS
#define NET_COMPRESS_MIN_LEN    (1024*10)

#define NET_NAME_LEN 48

#define NET_BUFFER_LEN       4096
#define NET_MSG_MAX_COUNT    5120//10240  //16k, 8(slot)*16(tf)*128(onu);auto msg  1000/s * 8
#define NET_MQ_MSG_COUNT     5
#define NET_WATCHER_COUNT    32   //1 accept fd + 8 client fd + 1 timer 

#define SAFE_FREE(x)           do{if(x){free(x);x=NULL;}}while(0)
#define SAFE_MP_FREE(x)        do{if(x){net_blk_pool_free(x);x=NULL;}}while(0)

#define FAKE_USE(x)

typedef enum
{
    FK_STATE_INACTIVE        = 0,        /**< Inactive. */
    FK_STATE_PROCESSING      = 1,        /**< Processing. */
    FK_STATE_ACTIVE_WORKING  = 2,        /**< Active Working. */
    FK_STATE_ACTIVE_STANDBY  = 3,        /**< Active Standby. */
    FK_STATE_NUM_OF    /**< Number of enum entries, not an entry itself. */
} FK_STATE;

#if DEFUNC("���к�")

#ifndef TAILQ_HEAD
/*
 * Tail queue declarations.
 */
#define TAILQ_HEAD(name, type)\
    struct name {\
    struct type *tqh_first; /* first element */\
    struct type **tqh_last; /* addr of last next element */\
}

#define TAILQ_HEAD_INITIALIZER(head)\
    { NULL, &(head).tqh_first }

#define TAILQ_ENTRY(type)\
struct {\
    struct type *tqe_next;  /* next element */\
    struct type **tqe_prev; /* address of previous next element */\
}

#define TAILQ_CONCAT(head1, head2, field) do {\
    if (!TAILQ_EMPTY(head2)) {\
        *(head1)->tqh_last = (head2)->tqh_first; \
        (head2)->tqh_first->field.tqe_prev = (head1)->tqh_last; \
        (head1)->tqh_last = (head2)->tqh_last; \
        TAILQ_INIT((head2)); \
    } \
} while (0)

#define TAILQ_EMPTY(head)   ((head)->tqh_first == NULL)

#define TAILQ_FIRST(head)   ((head)->tqh_first)

#define TAILQ_FOREACH(var, head, field) \
    for ((var) = TAILQ_FIRST((head));   \
        (var);  \
        (var) = TAILQ_NEXT((var), field))

#define TAILQ_FOREACH_SAFE(var, head, field, tvar)  \
    for ((var) = TAILQ_FIRST((head));   \
        (var) && ((tvar) = TAILQ_NEXT((var), field), 1);    \
        (var) = (tvar))

#define TAILQ_FOREACH_REVERSE(var, head, headname, field)\
    for ((var) = TAILQ_LAST((head), headname);  \
        (var);  \
        (var) = TAILQ_PREV((var), headname, field))

#define TAILQ_FOREACH_REVERSE_SAFE(var, head, headname, field, tvar)    \
    for ((var) = TAILQ_LAST((head), headname);  \
        (var) && ((tvar) = TAILQ_PREV((var), headname, field), 1);  \
        (var) = (tvar))

#define TAILQ_INIT(head) do {   \
        TAILQ_FIRST((head)) = NULL; \
        (head)->tqh_last = &TAILQ_FIRST((head));    \
} while (0)

#define TAILQ_INSERT_AFTER(head, listelm, elm, field) do {  \
    if ((TAILQ_NEXT((elm), field) = TAILQ_NEXT((listelm), field)) != NULL)\
    TAILQ_NEXT((elm), field)->field.tqe_prev =  \
    &TAILQ_NEXT((elm), field);  \
    else {  \
    (head)->tqh_last = &TAILQ_NEXT((elm), field);   \
    }   \
    TAILQ_NEXT((listelm), field) = (elm);   \
        (elm)->field.tqe_prev = &TAILQ_NEXT((listelm), field);  \
} while (0)

#define TAILQ_INSERT_BEFORE(listelm, elm, field) do {   \
    (elm)->field.tqe_prev = (listelm)->field.tqe_prev;  \
    TAILQ_NEXT((elm), field) = (listelm);   \
        *(listelm)->field.tqe_prev = (elm); \
        (listelm)->field.tqe_prev = &TAILQ_NEXT((elm), field);  \
} while (0)

#define TAILQ_INSERT_HEAD(head, elm, field) do {    \
    if ((TAILQ_NEXT((elm), field) = TAILQ_FIRST((head))) != NULL)   \
    TAILQ_FIRST((head))->field.tqe_prev =   \
    &TAILQ_NEXT((elm), field);  \
    else    \
    (head)->tqh_last = &TAILQ_NEXT((elm), field);   \
    TAILQ_FIRST((head)) = (elm);    \
    (elm)->field.tqe_prev = &TAILQ_FIRST((head));   \
} while (0)

#define TAILQ_INSERT_TAIL(head, elm, field) do {    \
    TAILQ_NEXT((elm), field) = NULL;    \
        (elm)->field.tqe_prev = (head)->tqh_last;   \
        *(head)->tqh_last = (elm);  \
        (head)->tqh_last = &TAILQ_NEXT((elm), field);   \
} while (0)

#define TAILQ_LAST(head, headname)  \
    (*(((struct headname *)((head)->tqh_last))->tqh_last))

#define TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)

#define TAILQ_PREV(elm, headname, field)    \
    (*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))

#define TAILQ_REMOVE(head, elm, field) do { \
    if ((TAILQ_NEXT((elm), field)) != NULL) \
    TAILQ_NEXT((elm), field)->field.tqe_prev =  \
    (elm)->field.tqe_prev;  \
    else {  \
    (head)->tqh_last = (elm)->field.tqe_prev;   \
    }   \
        *(elm)->field.tqe_prev = TAILQ_NEXT((elm), field);  \
} while (0)

#define TAILQ_SWAP(head1, head2, type, field) do {  \
    struct type *swap_first = (head1)->tqh_first;   \
    struct type **swap_last = (head1)->tqh_last;    \
    (head1)->tqh_first = (head2)->tqh_first;    \
    (head1)->tqh_last = (head2)->tqh_last;  \
    (head2)->tqh_first = swap_first;    \
    (head2)->tqh_last = swap_last;  \
    if ((swap_first = (head1)->tqh_first) != NULL)  \
    swap_first->field.tqe_prev = &(head1)->tqh_first;   \
    else    \
    (head1)->tqh_last = &(head1)->tqh_first;    \
    if ((swap_first = (head2)->tqh_first) != NULL)  \
    swap_first->field.tqe_prev = &(head2)->tqh_first;   \
    else    \
        (head2)->tqh_last = &(head2)->tqh_first;    \
} while (0)

/*
 * Singly-linked Tail queue declarations.
 */
#define STAILQ_HEAD(name, type) \
struct name {   \
    struct type *stqh_first;/* first element */ \
    struct type **stqh_last;/* addr of last next element */ \
}

#define STAILQ_HEAD_INITIALIZER(head)   \
    { NULL, &(head).stqh_first }

#define STAILQ_ENTRY(type)  \
struct {    \
        struct type *stqe_next; /* next element */  \
}

/*
 * Singly-linked Tail queue functions.
 */
#define STAILQ_CONCAT(head1, head2) do {    \
    if (!STAILQ_EMPTY((head2))) {   \
        *(head1)->stqh_last = (head2)->stqh_first;  \
        (head1)->stqh_last = (head2)->stqh_last;    \
        STAILQ_INIT((head2));   \
        }   \
} while (0)

#define STAILQ_EMPTY(head)  ((head)->stqh_first == NULL)

#define STAILQ_FIRST(head)  ((head)->stqh_first)

#define STAILQ_FOREACH(var, head, field)    \
        for((var) = STAILQ_FIRST((head));   \
        (var);  \
        (var) = STAILQ_NEXT((var), field))


#define STAILQ_FOREACH_SAFE(var, head, field, tvar) \
    for ((var) = STAILQ_FIRST((head));  \
        (var) && ((tvar) = STAILQ_NEXT((var), field), 1);   \
        (var) = (tvar))

#define STAILQ_INIT(head) do {  \
    STAILQ_FIRST((head)) = NULL;    \
        (head)->stqh_last = &STAILQ_FIRST((head));  \
} while (0)

#define STAILQ_INSERT_AFTER(head, tqelm, elm, field) do {   \
    if ((STAILQ_NEXT((elm), field) = STAILQ_NEXT((tqelm), field)) == NULL)\
        (head)->stqh_last = &STAILQ_NEXT((elm), field); \
        STAILQ_NEXT((tqelm), field) = (elm);    \
} while (0)

#define STAILQ_INSERT_HEAD(head, elm, field) do {   \
    if ((STAILQ_NEXT((elm), field) = STAILQ_FIRST((head))) == NULL) \
        (head)->stqh_last = &STAILQ_NEXT((elm), field); \
        STAILQ_FIRST((head)) = (elm);   \
} while (0)

#define STAILQ_INSERT_TAIL(head, elm, field) do {   \
    STAILQ_NEXT((elm), field) = NULL;   \
        *(head)->stqh_last = (elm); \
        (head)->stqh_last = &STAILQ_NEXT((elm), field); \
} while (0)

#define STAILQ_LAST(head, type, field)  \
    (STAILQ_EMPTY((head))   ?   \
        NULL :  \
            ((struct type *)(void *)    \
        ((char *)((head)->stqh_last) - __offsetof(struct type, field))))

#define STAILQ_NEXT(elm, field) ((elm)->field.stqe_next)

#define STAILQ_REMOVE(head, elm, type, field) do {  \
    if (STAILQ_FIRST((head)) == (elm)) {    \
        STAILQ_REMOVE_HEAD((head), field);  \
    }   \
    else {  \
        struct type *curelm = STAILQ_FIRST((head)); \
        while (STAILQ_NEXT(curelm, field) != (elm)) \
        curelm = STAILQ_NEXT(curelm, field);    \
        STAILQ_REMOVE_AFTER(head, curelm, field);   \
    }   \
} while (0)

#define STAILQ_REMOVE_AFTER(head, elm, field) do {  \
    if ((STAILQ_NEXT(elm, field) =  \
         STAILQ_NEXT(STAILQ_NEXT(elm, field), field)) == NULL)  \
        (head)->stqh_last = &STAILQ_NEXT((elm), field); \
} while (0)

#define STAILQ_REMOVE_HEAD(head, field) do {    \
    if ((STAILQ_FIRST((head)) = \
         STAILQ_NEXT(STAILQ_FIRST((head)), field)) == NULL) \
        (head)->stqh_last = &STAILQ_FIRST((head));  \
} while (0)

#define STAILQ_SWAP(head1, head2, type) do {    \
    struct type *swap_first = STAILQ_FIRST(head1);  \
    struct type **swap_last = (head1)->stqh_last;   \
    STAILQ_FIRST(head1) = STAILQ_FIRST(head2);  \
        (head1)->stqh_last = (head2)->stqh_last;    \
    STAILQ_FIRST(head2) = swap_first;   \
        (head2)->stqh_last = swap_last; \
    if (STAILQ_EMPTY(head1))    \
        (head1)->stqh_last = &STAILQ_FIRST(head1);  \
    if (STAILQ_EMPTY(head2))    \
        (head2)->stqh_last = &STAILQ_FIRST(head2);  \
} while (0)

#endif

#endif

#if DEFUNC("�ź�������")

#define NET_WAIT_FOREVER        0xFFFFFFFF  
#define NET_NO_WAIT             0   
#define NET_WAIT_DEFAULT        (1000000*10) //10s

typedef struct 
{
    sem_t s;            /**< pthread semaphore */
}NET_SEM;

typedef struct 
{
    pthread_mutex_t     m;          
    pthread_mutexattr_t attr;  
}NET_MUTUX;

typedef struct 
{
    pthread_rwlock_t     m;          
    pthread_rwlockattr_t attr;  
}NET_RWLOCK;

#endif

#if DEFUNC("�ڴ��")

#define NET_BLK_POOL_VALID   1
#define NET_BLK_POOL_DELETED 2

typedef struct
{
    unsigned int allocated;     
    unsigned int released;      
    unsigned int free;          
    unsigned int alloc_failed;  
} NET_BLK_POOL_STAT;

typedef struct
{
    const char   *name;   
    unsigned int blk_size;  
    unsigned int num_blks;
    unsigned int pool_size; /*ʹ���߲���Ҫ����*/
}NET_BLK_POOL_PARM;

typedef struct
{
    NET_BLK_POOL_PARM parm;  
    NET_BLK_POOL_STAT stat;  
} NET_BLK_POOL_INFO;

typedef STAILQ_HEAD(, NET_MEMBLK) NET_MEMBLK_LIST;

typedef struct NET_BLK_POOL
{
    NET_MUTUX                   lock;        
    NET_MEMBLK_LIST             free_list;
    NET_BLK_POOL_PARM           parm;  
    NET_BLK_POOL_STAT           stat;   
    void                            *start;               
    char                            name[NET_NAME_LEN];
    unsigned int                    status;
    STAILQ_ENTRY(NET_BLK_POOL)  list; 
}NET_BLK_POOL;

/*
�ڴ��ͷ��
*/
typedef struct NET_MEMBLK
{
    STAILQ_ENTRY(NET_MEMBLK) next;
    NET_BLK_POOL             *pool;
}NET_MEMBLK;

/*����ȡm������*/
#define NET_ROUND_UP(n, m) (((n) + (m) - 1) & ~((m) - 1))

#endif

#if DEFUNC("��Ϣ����")

#define SYS_V_MQ_MSG_SIZE                  (NET_BUFFER_LEN*NET_MSG_MAX_COUNT)//(1024*8192)

#define SYS_V_MQ_NO_WAIT                   (0)
#define SYS_V_MQ_WAIT_FOREVER              (-1)
#define SYS_V_MQ_MSG_ANY                   0

typedef enum
{
    SYS_V_MQ_CTRL_RX_SYN_REQ_MSG,
    SYS_V_MQ_CTRL_RX_SYN_ACK_MSG,
    SYS_V_MQ_CTRL_RX_ASYN_REQ_MSG,
    SYS_V_MQ_CTRL_RX_ASYN_ACK_MSG,
    SYS_V_MQ_CTRL_TX_MSG,

    SYS_V_MQ_FK_RX_SYN_REQ_MSG,
    SYS_V_MQ_FK_RX_SYN_ACK_MSG,
    SYS_V_MQ_FK_RX_ASYN_REQ_MSG,
    SYS_V_MQ_FK_RX_ASYN_ACK_MSG,
    SYS_V_MQ_FK_TX_MSG,

    SYS_V_MQ_MAX_NUM,
}SYS_V_MQ_ID_E;

#define NET_ZC_MSG_QUEUE_DELETED    0
#define NET_ZC_MSG_QUEUE_VALID      1

typedef struct NET_OS_MSG NET_OS_MSG;

struct NET_OS_MSG
{
    STAILQ_ENTRY(NET_OS_MSG) next;   
    void                     *data;  /**< Message data pointer */
    unsigned int             size;   /**< Message data size */
};

typedef STAILQ_HEAD(, NET_OS_MSG) NET_OS_MSG_LIST;

typedef struct NET_ZC_MQ_ST
{
    unsigned int    msg_in;      /* num of msg in the queue */
    NET_MUTUX       lock;        /* Queue protection lock */
    NET_OS_MSG_LIST msgl;        /* Message list */

} NET_ZC_MQ_ST;

typedef struct NET_ZC_MQ
{
    NET_ZC_MQ_ST          q;                          /**< Queue control block */
    NET_SEM               m;                          /**< Mutex to suspend waiting task on */
    int                      is_waiting;                 /**< TRUE if task is waiting on queue */
    char                     name[NET_NAME_LEN]; /**< Queue name */
    unsigned int             state;                      /**< magic number */
}NET_ZC_MQ;

#endif

typedef struct       
{  
    char            name_total[20];
    unsigned long   total;
    char            name_total_uint[20];
    char            name_free[20];  
    unsigned long   free;
    char            name_free_uint[20];  
}NET_MEM_OCCUPY;

typedef struct PACKED         
{  
    char             name[20];      
    unsigned long    user; 
    unsigned long    nice; 
    unsigned long    system;
    unsigned long    idle;
    unsigned long    iowait; 
    unsigned long    irq;
    unsigned long    softirq;
}NET_CPU_OCCUPY;   

void 
net_timewait(int sec, int us);

unsigned int 
net_timestamp(void);

void 
net_abs_time_get(struct timespec *pts, int timeout);

unsigned int 
net_sem_create(NET_SEM *psem, int count);

unsigned int 
net_sem_destroy(NET_SEM *psem);

unsigned int 
net_sem_wait(NET_SEM *psem, int timeout);

unsigned int 
net_sem_post(NET_SEM *psem);

unsigned int 
net_mutex_create(NET_MUTUX *pmutex);

unsigned int 
net_mutux_destory(NET_MUTUX *pmutex);

unsigned int 
net_mutex_trylock(NET_MUTUX *pmutex);

unsigned int 
net_mutex_lock(NET_MUTUX *pmutex);

unsigned int 
net_mutex_unlock(NET_MUTUX *pmutex);

unsigned int 
net_rwlock_create(NET_RWLOCK *p_rwlock);

unsigned int 
net_rwlock_destory(NET_RWLOCK *p_rwlock);

unsigned int 
net_rwlock_rd_trylock(NET_RWLOCK *p_rwlock);

unsigned int 
net_rwlock_wr_trylock(NET_RWLOCK *p_rwlock);

unsigned int 
net_rwlock_rd_lock(NET_RWLOCK *p_rwlock);

unsigned int 
net_rwlock_rd_timelock(NET_RWLOCK *p_rwlock, struct timespec *p_timeout);

unsigned int 
net_rwlock_wr_lock(NET_RWLOCK *p_rwlock);

unsigned int 
net_rwlock_wr_timelock(NET_RWLOCK *p_rwlock, struct timespec *p_timeout);

unsigned int
net_rwlock_unlock(NET_RWLOCK *p_rwlock);

void*
net_malloc(NET_BLK_POOL* p_mem_pool, unsigned int size);

void*
net_calloc(NET_BLK_POOL* p_mem_pool, unsigned int num, unsigned int size);

void 
net_safe_free(void* p_buffer);

unsigned int 
net_blk_pool_create( NET_BLK_POOL *pool, 
                                         const NET_BLK_POOL_PARM *parm);
unsigned int 
net_blk_pool_destroy(NET_BLK_POOL *pool);

void*
net_blk_pool_alloc(NET_BLK_POOL *pool);

void*
net_blk_pool_calloc(NET_BLK_POOL *pool);

void 
net_blk_pool_free(void *ptr);

unsigned int 
net_blk_pool_query( const NET_BLK_POOL *pool, 
                                        NET_BLK_POOL_INFO  *info);
unsigned int 
net_blk_pool_get_next(const NET_BLK_POOL **prev);

unsigned int 
net_tid_get(void);

unsigned int 
net_mem_load_get(unsigned int *p_occupy);

unsigned int 
net_cpu_time_get(NET_CPU_OCCUPY *p_time);

unsigned int 
net_cpu_cal_occupy (NET_CPU_OCCUPY *p_old, NET_CPU_OCCUPY *p_new, unsigned int *p_cpu_use);

void 
net_bitlist_set(unsigned char *p_pit_list, unsigned int bit);

void 
net_bitlist_clear(unsigned char *p_pit_list, unsigned int bit);

unsigned char 
net_bitlist_check(unsigned char *p_pit_list, unsigned int bit);

unsigned int 
net_bitlist_check_all(unsigned char *p_pit_list, unsigned int bit_list_len);

unsigned int
net_data_compress(
                            const void*             p_in, 
                            const unsigned long     in_len, 
                            void*                   p_out, 
                            unsigned long*          p_out_len,
                            unsigned int            level);
                          
unsigned int
net_data_uncompress(
                            void*           p_in, 
                            unsigned long   in_len, 
                            void*           p_out, 
                            unsigned long*  p_out_len);

unsigned int 
net_systemv_mq_create (
                int             *p_queue_id, 
                SYS_V_MQ_ID_E   sub_key, 
                unsigned int    queue_size);

unsigned int
net_systemv_mq_get (
                int             queue_id, 
                long            type, 
                void            *p_data, 
                unsigned int    size, 
                int             timeout, /* ms  */
                unsigned int    *p_size_copied);
                
unsigned int
net_systemv_mq_put (
                int             queue_id, 
                void            *p_data, 
                unsigned int    size, 
                int             timeout);

void 
net_zc_mq_os_msg_free(NET_OS_MSG *p_msg);

unsigned int
net_zc_mq_create(NET_ZC_MQ *p_queue, const char *p_name);

unsigned int 
net_zc_mq_destroy(NET_ZC_MQ *p_queue);

void
net_zc_mq_os_msg_put(NET_ZC_MQ *p_queue, NET_OS_MSG *p_msg);

unsigned int 
net_zc_mq_os_msg_get(
                                NET_ZC_MQ   *p_queue, 
                                unsigned int    timeout, 
                                NET_OS_MSG  **p_msg);

unsigned int 
net_config_file_paser(
                    const char          *p_file_name, 
                    const char          *p_target_name, 
                    char*               p_target,
                    const unsigned int  target_len);                               

#endif
