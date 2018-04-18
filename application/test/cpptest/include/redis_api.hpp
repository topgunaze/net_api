#ifndef __REDIS_API_HPP__
#define __REDIS_API_HPP__

/*
	hiredis是redis数据库的C接口，目前只能在linux下使用，几个基本的函数就可以操作redis数据库了。
函数原型：redisContext *redisConnect(const char *ip, int port)
	说明：该函数用来连接redis数据库，参数为数据库的ip地址和端口，一般redis数据库的端口为6379
该函数返回一个结构体redisContext。

函数原型：void *redisCommand(redisContext *c, const char *format, ...);
	说明：该函数执行命令，就如sql数据库中的SQL语句一样，只是执行的是redis数据库中的操作命令，
第一个参数为连接数据库时返回的redisContext，剩下的参数为变参，就如C标准函数printf函数一样的变参。
返回值为void*，一般强制转换成为redisReply类型的进行进一步的处理。

函数原型：void freeReplyObject(void *reply);
	说明：释放redisCommand执行后返回的redisReply所占用的内存

函数原型：void redisFree(redisContext *c);
	说明：释放redisConnect()所产生的连接。
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
