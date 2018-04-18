#ifndef __REDIS_API_HPP__
#define __REDIS_API_HPP__

/*
	hiredis��redis���ݿ��C�ӿڣ�Ŀǰֻ����linux��ʹ�ã����������ĺ����Ϳ��Բ���redis���ݿ��ˡ�
����ԭ�ͣ�redisContext *redisConnect(const char *ip, int port)
	˵�����ú�����������redis���ݿ⣬����Ϊ���ݿ��ip��ַ�Ͷ˿ڣ�һ��redis���ݿ�Ķ˿�Ϊ6379
�ú�������һ���ṹ��redisContext��

����ԭ�ͣ�void *redisCommand(redisContext *c, const char *format, ...);
	˵�����ú���ִ���������sql���ݿ��е�SQL���һ����ֻ��ִ�е���redis���ݿ��еĲ������
��һ������Ϊ�������ݿ�ʱ���ص�redisContext��ʣ�µĲ���Ϊ��Σ�����C��׼����printf����һ���ı�Ρ�
����ֵΪvoid*��һ��ǿ��ת����ΪredisReply���͵Ľ��н�һ���Ĵ���

����ԭ�ͣ�void freeReplyObject(void *reply);
	˵�����ͷ�redisCommandִ�к󷵻ص�redisReply��ռ�õ��ڴ�

����ԭ�ͣ�void redisFree(redisContext *c);
	˵�����ͷ�redisConnect()�����������ӡ�
*/

//hiredis/hiredis.h  
/* Context for a connection to Redis */  
typedef struct redisContext {  
    int err; /* Error flags, 0 when there is no error */  
    char errstr[128]; /* String representation of error when applicable */  
    int fd;   
    int flags;  
    char *obuf; /* Write buffer */  
    redisReader *reader; /* Protocol reader */  
} redisContext;  
  
/* This is the reply object returned by redisCommand() */  
#define REDIS_REPLY_STRING 1  
#define REDIS_REPLY_ARRAY 2  
#define REDIS_REPLY_INTEGER 3  
#define REDIS_REPLY_NIL 4  
#define REDIS_REPLY_STATUS 5  
#define REDIS_REPLY_ERROR 6

typedef struct redisReply {  
    int type; /* REDIS_REPLY_* */  
    long long integer; /* The integer when type is REDIS_REPLY_INTEGER */  
    int len; /* Length of string */  
    char *str; /* Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING */  
    size_t elements; /* number of elements, for REDIS_REPLY_ARRAY */  
    struct redisReply **element; /* elements vector for REDIS_REPLY_ARRAY */  
} redisReply;  
  
redisContext *redisConnectWithTimeout(const char *ip, int port, struct timeval tv);  
void redisFree(redisContext *c);  


#endif
