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
    //redisĬ�ϼ����˿�Ϊ6387 �����������ļ����޸�  
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
//redisContext *redis_ctx_��redisReply *redis_reply_������ʼ��
void redisConnect(string serverAddr, uint16_t port, string password)  
{  
    LOG_INFO << "��ʼ����redis������..." << serverAddr << ":" << port;  
  
    // ����  
    timeval timeout = { 3, 500000 };  
    redis_ctx_ = redisConnectWithTimeout(serverAddr.c_str(), port, timeout);  
    if (redis_ctx_ == NULL || redis_ctx_->err)  
    {  
        if (redis_ctx_ != NULL)  
        {  
            LOG_INFO << "�����쳣: " << redis_ctx_->errstr;  
            redisFree(redis_ctx_);  
            redis_ctx_ = NULL;  
        }  
        else  
        {  
            LOG_INFO << "�����쳣: redis context��ʼ������";  
        }  
  
        // todo �ӳ�30����������redis������  
        return;  
    }  
    else  
    {  
        LOG_INFO << "����redis�������ɹ�..." << serverAddr << ":" << port;  
    }  
  
    // ��֤  
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

//�ַ�������redis��������redis��������ģʽ��
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
            LOG_INFO << "�����ʧ��: " << redis_reply_->type << " " << redis_reply_->str;  
        }  
		
        freeReplyObject(redis_reply_);  
    }  
    else  
    {  
        LOG_INFO << "��redis�����������쳣, �����ʧ��: " << redis_ctx_->err << " " << redis_ctx_->errstr;  
        redisFree(redis_ctx_);  
        redis_ctx_ = NULL;  
  
        // todo �ӳ�30����������redis������  
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
    std::queue<redisContext *> m_clients; //���ӳ� 
  
    time_t m_beginInvalidTime;  
    static const int m_maxReconnectInterval = 3; //���ƶϵ�ʱ��Ƶ������ 
  
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

char *m_szRedisHost="127.0.0.1";  //redis��ַ  
uint16_t m_iRedisPort=6379;  //redis�˿�  
int timeout=4000000; //��ʱʱ��  
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
  //�Դ��г�ʱ�ķ�ʽ����Redis��������ͬʱ��ȡ��Redis���ӵ������Ķ���
  //�ö����������������Redis�����ĺ�����
  redisContext* c = redisConnectWithTimeout("192.168.149.137",6379,tv);
  if (c->err) {
    redisFree(c);
    return;
  }
  const char* command1 = "set stest1 value1";
  redisReply* r = (redisReply*)redisCommand(c,command1);
  //��Ҫע����ǣ�������صĶ�����NULL�����ʾ�ͻ��˺ͷ�����֮��������ش��󣬱����������ӡ�
  //����ֻ�Ǿ���˵���������������������Ͳ������������ж��ˡ�
  if (NULL == r) {
    redisFree(c);
    return;
  }
  //��ͬ��Redis����ص��������Ͳ�ͬ���ڻ�ȡ֮ǰ��Ҫ���ж�����ʵ�����͡�
  //���ڸ�������ķ���ֵ��Ϣ�����Բο�Redis�Ĺٷ��ĵ�
  //�й�Redis�����������͵Ĳ��͡�:)
  //�ַ������͵�set����ķ���ֵ��������REDIS_REPLY_STATUS��Ȼ��ֻ�е�������Ϣ��"OK"
  //ʱ���ű�ʾ������ִ�гɹ�������������Դ����ƣ��Ͳ��ٹ���׸���ˡ�
  if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str,"OK") == 0)) {
    printf("Failed to execute command[%s].\n",command1);
    freeReplyObject(r);
    redisFree(c);
    return;
  }
  //���ں����ظ�ʹ�øñ�����������Ҫ��ǰ�ͷţ������ڴ�й©��
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
  //������Ҫ��˵��һ�£�����stest2���������ڣ����Redis�᷵�ؿս��������ֻ��Ϊ����ʾ��
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
  //����stest2�������Redis������������ֻ�ǵڶ���ֵΪnil��
  //�����ж��ֵ���أ���Ϊ����Ӧ����������������͡�
  if (r->type != REDIS_REPLY_ARRAY) {
    printf("Failed to execute command[%s].\n",command5);
    freeReplyObject(r);
    redisFree(c);
    //r->elements��ʾ��Ԫ�ص����������������key�Ƿ���ڣ���ֵ�����������Ǽ���������
    assert(2 == r->elements);
    return;
  }
  for (int i = 0; i < r->elements; ++i) {
    redisReply* childReply = r->element[i];
    //֮ǰ�Ѿ����ܹ���get����ص�����������string��
    //���ڲ�����key�ķ���ֵ��������ΪREDIS_REPLY_NIL��
    if (childReply->type == REDIS_REPLY_STRING)
      printf("The value is %s.\n",childReply->str);
  }
  //����ÿһ����Ӧ������ʹ���ߵ����ͷţ�ֻ���ͷ����ⲿ��redisReply���ɡ�
  freeReplyObject(r);
  printf("Succeed to execute command[%s].\n",command5);
 
  printf("Begin to test pipeline.\n");
  //������ֻ�ǽ������͵�����д�뵽�����Ķ��������������У�ֱ�����ú����
  //redisGetReply����Ż��������������е�����д����Redis����������������
  //��Ч�ļ��ٿͻ����������֮���ͬ���Ⱥ�ʱ�䣬�Լ�����IO������ӳ١�
  //���ڹ��ߵľ����������ƣ����Կ��Ǹ�ϵ�в����еĹ������⡣
  if (REDIS_OK != redisAppendCommand(c,command1)
    || REDIS_OK != redisAppendCommand(c,command2)
    || REDIS_OK != redisAppendCommand(c,command3)
    || REDIS_OK != redisAppendCommand(c,command4)
    || REDIS_OK != redisAppendCommand(c,command5)) {
    redisFree(c);
    return;
  }
 
  redisReply* reply = NULL;
  //��pipeline���ؽ���Ĵ���ʽ����ǰ�����Ĵ���ʽ��ȫһֱ������Ͳ����ظ������ˡ�
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
  //��������ͨ��pipeline�ύ������������Ϊ���أ������ʱ��������redisGetReply��
  //���ᵼ�¸ú�������������ǰ�̣߳�ֱ�����µ�ͨ�������ύ�����������ء�
  //���Ҫ�������˳�ǰ�ͷŵ�ǰ���ӵ������Ķ���
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


