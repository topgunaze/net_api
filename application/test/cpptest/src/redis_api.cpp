#include <stdio.h>  
#include <stdlib.h>  
#include <stddef.h>  
#include <stdarg.h>  
#include <string.h>  
#include <assert.h>  

#if 0

#include <hiredis/hiredis.h>  

#include <iostream>  
#include <string>  
#include <sys/time.h>  
#include <time.h>  

using namespace std;  


//c api
void doTest()  
{  
    //redis默认监听端口为6387 可以再配置文件中修改  
    redisContext* c = redisConnect("127.0.0.1", 6379);  
    if ( c->err)  
    {  
        redisFree(c);  
        printf("Connect to redisServer faile\n");  
        return ;  
    }
    printf("Connect to redisServer Success\n");  
      
    const char* command1 = "set stest1 value1";  
    redisReply* r = (redisReply*)redisCommand(c, command1);
    if( NULL == r)  
    {  
        printf("Execut command1 failure\n");  
        redisFree(c);  
        return;  
    }
	
    if( !(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str,"OK")==0))  
    {  
        printf("Failed to execute command[%s]\n", command1);  
        freeReplyObject(r);  
        redisFree(c);  
        return;  
    }
	
    freeReplyObject(r);  
    printf("Succeed to execute command[%s]\n", command1);  
      
    const char* command2 = "strlen stest1";  
    r = (redisReply*)redisCommand(c, command2);  
    if ( r->type != REDIS_REPLY_INTEGER)  
    {  
        printf("Failed to execute command[%s]\n",command2);  
        freeReplyObject(r);  
        redisFree(c);  
        return;  
    }
    int length =  r->integer;  
    freeReplyObject(r);  
    printf("The length of 'stest1' is %d.\n", length);  
    printf("Succeed to execute command[%s]\n", command2);  
      
    const char* command3 = "get stest1";  
    r = (redisReply*)redisCommand(c, command3);  
    if ( r->type != REDIS_REPLY_STRING)  
    {  
        printf("Failed to execute command[%s]\n",command3);  
        freeReplyObject(r);  
        redisFree(c);  
        return;  
    }  
    printf("The value of 'stest1' is %s\n", r->str);  
    freeReplyObject(r);  
    printf("Succeed to execute command[%s]\n", command3);  
      
    const char* command4 = "get stest2";  
    r = (redisReply*)redisCommand(c, command4);  
    if ( r->type != REDIS_REPLY_NIL)  
    {  
        printf("Failed to execute command[%s]\n",command4);  
        freeReplyObject(r);  
        redisFree(c);  
        return;  
    }  
    freeReplyObject(r);  
    printf("Succeed to execute command[%s]\n", command4);     
      
    redisFree(c); 
}  

//c++ api
//redisContext *redis_ctx_及redisReply *redis_reply_，并初始化
void redisConnect(string serverAddr, uint16_t port, string password)  
{  
    LOG_INFO << "开始连接redis服务器..." << serverAddr << ":" << port;  
  
    // 连接  
    timeval timeout = { 3, 500000 };  
    redis_ctx_ = redisConnectWithTimeout(serverAddr.c_str(), port, timeout);  
    if (redis_ctx_ == NULL || redis_ctx_->err)  
    {  
        if (redis_ctx_ != NULL)  
        {  
            LOG_INFO << "连接异常: " << redis_ctx_->errstr;  
            redisFree(redis_ctx_);  
            redis_ctx_ = NULL;  
        }  
        else  
        {  
            LOG_INFO << "连接异常: redis context初始化出错";  
        }  
  
        // todo 延迟30秒重新连接redis服务器  
        return;  
    }  
    else  
    {  
        LOG_INFO << "连接redis服务器成功..." << serverAddr << ":" << port;  
    }  
  
    // 验证  
    redis_reply_ = (redisReply *)redisCommand(redis_ctx_, "auth %s", password.c_str());  
    if (redis_reply_->type == REDIS_REPLY_ERROR)  
    {  
        LOG_INFO << "Authentication failure";  
    }  
    else  
    {  
        LOG_INFO << "Authentication success";  
    }
	
    freeReplyObject(redis_reply_);  
}

//分发数据至redis服务器（redis发布订阅模式）
void distributeRedisMessage(const string &content, const string &topic)  
{  
    if (redis_ctx_ == NULL)  
    {  
        return;  
    }  
  
    redis_reply_ = (redisReply *)redisCommand(redis_ctx_, "publish %s %s", topic.c_str(), content.c_str());  
    if (redis_reply_ != NULL)  
    {  
        if (redis_reply_->type == REDIS_REPLY_ERROR)  
        {  
            LOG_INFO << "命令发送失败: " << redis_reply_->type << " " << redis_reply_->str;  
        }  
		
        freeReplyObject(redis_reply_);  
    }  
    else  
    {  
        LOG_INFO << "与redis服务器连接异常, 命令发送失败: " << redis_ctx_->err << " " << redis_ctx_->errstr;  
        redisFree(redis_ctx_);  
        redis_ctx_ = NULL;  
  
        // todo 延迟30秒重新连接redis服务器  
    }  
}

class KGRedisClient  
{  
public:  
    KGRedisClient(string ip, int port, int timeout = 2000);  
    virtual ~KGRedisClient();  
  
    bool ExecuteCmd(const char *cmd, size_t len, string &response);  
    redisReply* ExecuteCmd(const char *cmd, size_t len);  
  
private:  
    int m_timeout;  
    int m_serverPort;  
    string m_setverIp;  
    CCriticalSection m_lock;  
    std::queue<redisContext *> m_clients; //连接池 
  
    time_t m_beginInvalidTime;  
    static const int m_maxReconnectInterval = 3; //控制断掉时的频繁连接 
  
    redisContext* CreateContext();  
    void ReleaseContext(redisContext *ctx, bool active);  
    bool CheckStatus(redisContext *ctx);  
};  
  
KGRedisClient::KGRedisClient(string ip, int port, int timeout)  
{  
    m_timeout = timeout;  
    m_serverPort = port;  
    m_setverIp = ip;  
  
    m_beginInvalidTime = 0;  
}  
  
KGRedisClient::~KGRedisClient()  
{  
    CAutoLock autolock(m_lock);
	
    while(!m_clients.empty())  
    {  
        redisContext *ctx = m_clients.front();  
        redisFree(ctx);  
        m_clients.pop();  
    }  
}  
  
bool KGRedisClient::ExecuteCmd(const char *cmd, size_t len,string &response)  
{  
    redisReply *reply = ExecuteCmd(cmd, len);  
    if(reply == NULL) 
		return false;  
  
    boost::shared_ptr<redisReply> autoFree(reply, freeReplyObject);  
    if(reply->type == REDIS_REPLY_INTEGER)  
    {  
        response = _IntToStrA(reply->integer);  
        return true;  
    }  
    else if(reply->type == REDIS_REPLY_STRING)  
    {  
        response.assign(reply->str, reply->len);  
        return true;  
    }  
    else if(reply->type == REDIS_REPLY_STATUS)  
    {  
        response.assign(reply->str, reply->len);  
        return true;  
    }  
    else if(reply->type == REDIS_REPLY_NIL)  
    {  
        response = "";  
        return true;  
    }  
    else if(reply->type == REDIS_REPLY_ERROR)  
    {  
        response.assign(reply->str, reply->len);  
        return false;  
    }  
    else if(reply->type == REDIS_REPLY_ARRAY)  
    {  
        response = "Not Support Array Result!!!";  
        return false;  
    }  
    else  
    {  
        response = "Undefine Reply Type";  
        return false;  
    }  
}  
  
redisReply* KGRedisClient::ExecuteCmd(const char *cmd, size_t len)  
{  
    redisContext *ctx = CreateContext();  
    if(ctx == NULL) 
		return NULL;  
  
    redisReply *reply = (redisReply*)redisCommand(ctx, "%b", cmd, len);  
  
    ReleaseContext(ctx, reply != NULL);  
  
    return reply;  
}  
  
redisContext* KGRedisClient::CreateContext()  
{  
    {  
        CAutoLock autolock(m_lock);  
        if(!m_clients.empty())  
        {  
            redisContext *ctx = m_clients.front();  
            m_clients.pop();  
  
            return ctx;  
        }  
    }  
  
    time_t now = time(NULL);  
    if(now < m_beginInvalidTime + m_maxReconnectInterval) 
		return NULL;  
  
    struct timeval tv;  
    tv.tv_sec = m_timeout / 1000;  
    tv.tv_usec = (m_timeout % 1000) * 1000;
	
    redisContext *ctx = redisConnectWithTimeout(m_setverIp.c_str(), m_serverPort, tv);  
    if(ctx == NULL || ctx->err != 0)  
    {  
        if(ctx != NULL) 
			redisFree(ctx);  
  
        m_beginInvalidTime = time(NULL);  
          
        return NULL;  
    }  
  
    return ctx;  
}  
  
void KGRedisClient::ReleaseContext(redisContext *ctx, bool active)  
{  
    if(ctx == NULL) 
		return;
	
    if(!active) 
	{
		redisFree(ctx); 
		return;
	}  
  
    CAutoLock autolock(m_lock);  
    m_clients.push(ctx);  
}  
  
bool KGRedisClient::CheckStatus(redisContext *ctx)  
{  
    redisReply *reply = (redisReply*)redisCommand(ctx, "ping");  
    if(reply == NULL) 
		return false;  
  
    boost::shared_ptr<redisReply> autoFree(reply, freeReplyObject);  
  
    if(reply->type != REDIS_REPLY_STATUS) 
		return false;
	
    if(strcasecmp(reply->str,"PONG") != 0) 
		return false;  
  
    return true;  
}  

char *m_szRedisHost="127.0.0.1";  //redis地址  
uint16_t m_iRedisPort=6379;  //redis端口  
int timeout=4000000; //超时时间  
struct timeval m_RedisTimeout;  
m_RedisTimeout.tv_sec=timeout / 1000000;  
m_RedisTimeout.tv_usec=timeout % 1000000;  
  
bool RedisConnectWithTimeout(redisContext* redis_context)  
{  
    if (redis_context != NULL)  
    {  
        redisFree(redis_context);  
    }
	
    redis_context = redisConnectWithTimeout(  
                            m_szRedisHost,  
                            m_iRedisPort,  
                            m_RedisTimeout);  
    if (redis_context == NULL || redis_context->err)  
    {  
        printf("Connect to redisServer faile\n");  
        return false;  
    }
	
    if (redisSetTimeout(redis_context, m_RedisTimeout) != REDIS_OK)  
    {  
        printf("Connect to redisServer time out\n");  
        return false;  
    }
	
    return true;  
}

void GetData(redisContext *redis_context, string &strUserId, string &result)  
{  
    string strCommand = "HGET user_" + strUserId;  
    redisReply *reply = static_cast<redisReply*>(redisCommand(redis_context, strCommand.c_str()));  
    if (reply == NULL)  
    {  
        printf("Get Data failed\n");  
        redisFree(redis_context);  
        return;  
    }

    if (reply->type != REDIS_REPLY_STRING)  
    {  
        freeReplyObject(reply);  
        return;  
    }
	
    result = string(reply->str, reply->len);  
    freeReplyObject(reply);  
}

int main()  
{  
    redisContext* redis_context;  
    string userId = "01";  
    string result;  
    if (RedisConnectWithTimeout(redis_context))  
    {  
        GetData(redis_context,userId,result);  
    }  
    return 0;  
}  

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <hiredis.h>
 
void doTest()
{
  int timeout = 10000;
  struct timeval tv;
  tv.tv_sec = timeout / 1000;
  tv.tv_usec = timeout * 1000;
  //以带有超时的方式链接Redis服务器，同时获取与Redis连接的上下文对象。
  //该对象将用于其后所有与Redis操作的函数。
  redisContext* c = redisConnectWithTimeout("192.168.149.137",6379,tv);
  if (c->err) {
    redisFree(c);
    return;
  }
  const char* command1 = "set stest1 value1";
  redisReply* r = (redisReply*)redisCommand(c,command1);
  //需要注意的是，如果返回的对象是NULL，则表示客户端和服务器之间出现严重错误，必须重新链接。
  //这里只是举例说明，简便起见，后面的命令就不再做这样的判断了。
  if (NULL == r) {
    redisFree(c);
    return;
  }
  //不同的Redis命令返回的数据类型不同，在获取之前需要先判断它的实际类型。
  //至于各种命令的返回值信息，可以参考Redis的官方文档
  //有关Redis各种数据类型的博客。:)
  //字符串类型的set命令的返回值的类型是REDIS_REPLY_STATUS，然后只有当返回信息是"OK"
  //时，才表示该命令执行成功。后面的例子以此类推，就不再过多赘述了。
  if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str,"OK") == 0)) {
    printf("Failed to execute command[%s].\n",command1);
    freeReplyObject(r);
    redisFree(c);
    return;
  }
  //由于后面重复使用该变量，所以需要提前释放，否则内存泄漏。
  freeReplyObject(r);
  printf("Succeed to execute command[%s].\n",command1);
 
  const char* command2 = "strlen stest1";
  r = (redisReply*)redisCommand(c,command2);
  if (r->type != REDIS_REPLY_INTEGER) {
    printf("Failed to execute command[%s].\n",command2);
    freeReplyObject(r);
    redisFree(c);
    return;
  }
  int length = r->integer;
  freeReplyObject(r);
  printf("The length of 'stest1' is %d.\n",length);
  printf("Succeed to execute command[%s].\n",command2);
 
  const char* command3 = "get stest1";
  r = (redisReply*)redisCommand(c,command3);
  if (r->type != REDIS_REPLY_STRING) {
    printf("Failed to execute command[%s].\n",command3);
    freeReplyObject(r);
    redisFree(c);
    return;
  }
  printf("The value of 'stest1' is %s.\n",r->str);
  freeReplyObject(r);
  printf("Succeed to execute command[%s].\n",command3);
 
  const char* command4 = "get stest2";
  r = (redisReply*)redisCommand(c,command4);
  //这里需要先说明一下，由于stest2键并不存在，因此Redis会返回空结果，这里只是为了演示。
  if (r->type != REDIS_REPLY_NIL) {
    printf("Failed to execute command[%s].\n",command4);
    freeReplyObject(r);
    redisFree(c);
    return;
  }
  freeReplyObject(r);
  printf("Succeed to execute command[%s].\n",command4);
 
  const char* command5 = "mget stest1 stest2";
  r = (redisReply*)redisCommand(c,command5);
  //不论stest2存在与否，Redis都会给出结果，只是第二个值为nil。
  //由于有多个值返回，因为返回应答的类型是数组类型。
  if (r->type != REDIS_REPLY_ARRAY) {
    printf("Failed to execute command[%s].\n",command5);
    freeReplyObject(r);
    redisFree(c);
    //r->elements表示子元素的数量，不管请求的key是否存在，该值都等于请求是键的数量。
    assert(2 == r->elements);
    return;
  }
  for (int i = 0; i < r->elements; ++i) {
    redisReply* childReply = r->element[i];
    //之前已经介绍过，get命令返回的数据类型是string。
    //对于不存在key的返回值，其类型为REDIS_REPLY_NIL。
    if (childReply->type == REDIS_REPLY_STRING)
      printf("The value is %s.\n",childReply->str);
  }
  //对于每一个子应答，无需使用者单独释放，只需释放最外部的redisReply即可。
  freeReplyObject(r);
  printf("Succeed to execute command[%s].\n",command5);
 
  printf("Begin to test pipeline.\n");
  //该命令只是将待发送的命令写入到上下文对象的输出缓冲区中，直到调用后面的
  //redisGetReply命令才会批量将缓冲区中的命令写出到Redis服务器。这样可以
  //有效的减少客户端与服务器之间的同步等候时间，以及网络IO引起的延迟。
  //至于管线的具体性能优势，可以考虑该系列博客中的管线主题。
  if (REDIS_OK != redisAppendCommand(c,command1)
    || REDIS_OK != redisAppendCommand(c,command2)
    || REDIS_OK != redisAppendCommand(c,command3)
    || REDIS_OK != redisAppendCommand(c,command4)
    || REDIS_OK != redisAppendCommand(c,command5)) {
    redisFree(c);
    return;
  }
 
  redisReply* reply = NULL;
  //对pipeline返回结果的处理方式，和前面代码的处理方式完全一直，这里就不再重复给出了。
  if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
    printf("Failed to execute command[%s] with Pipeline.\n",command1);
    freeReplyObject(reply);
    redisFree(c);
  }
  freeReplyObject(reply);
  printf("Succeed to execute command[%s] with Pipeline.\n",command1);
 
  if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
    printf("Failed to execute command[%s] with Pipeline.\n",command2);
    freeReplyObject(reply);
    redisFree(c);
  }
  freeReplyObject(reply);
  printf("Succeed to execute command[%s] with Pipeline.\n",command2);
 
  if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
    printf("Failed to execute command[%s] with Pipeline.\n",command3);
    freeReplyObject(reply);
    redisFree(c);
  }
  freeReplyObject(reply);
  printf("Succeed to execute command[%s] with Pipeline.\n",command3);
 
  if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
    printf("Failed to execute command[%s] with Pipeline.\n",command4);
    freeReplyObject(reply);
    redisFree(c);
  }
  freeReplyObject(reply);
  printf("Succeed to execute command[%s] with Pipeline.\n",command4);
 
  if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
    printf("Failed to execute command[%s] with Pipeline.\n",command5);
    freeReplyObject(reply);
    redisFree(c);
  }
  freeReplyObject(reply);
  printf("Succeed to execute command[%s] with Pipeline.\n",command5);
  //由于所有通过pipeline提交的命令结果均已为返回，如果此时继续调用redisGetReply，
  //将会导致该函数阻塞并挂起当前线程，直到有新的通过管线提交的命令结果返回。
  //最后不要忘记在退出前释放当前连接的上下文对象。
  redisFree(c);
  return;
}
 
int main() 
{
  doTest();
  return 0;
}

//Succeed to execute command[set stest1 value1].
//The length of 'stest1' is 6.
//Succeed to execute command[strlen stest1].
//The value of 'stest1' is value1.
//Succeed to execute command[get stest1].
//Succeed to execute command[get stest2].
//The value is value1.
//Succeed to execute command[mget stest1 stest2].
//Begin to test pipeline.
//Succeed to execute command[set stest1 value1] with Pipeline.
//Succeed to execute command[strlen stest1] with Pipeline.
//Succeed to execute command[get stest1] with Pipeline.
//Succeed to execute command[get stest2] with Pipeline.
//Succeed to execute command[mget stest1 stest2] with Pipeline.


#endif


