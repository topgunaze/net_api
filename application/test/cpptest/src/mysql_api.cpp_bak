#include "mysql_api.hpp"

#if 0

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>

#define MAX_COLUMN_LEN    32

int main(int argc , char *argv[])
{
    MYSQL my_connection;
    MYSQL_RES *result;
    MYSQL_ROW sql_row;
    MYSQL_FIELD *fd;
    char column[MAX_COLUMN_LEN][MAX_COLUMN_LEN];
    int res;
    mysql_init(&my_connection);
    if(mysql_real_connect(&my_connection,"127.0.0.1","用户","密码","数据名称",3306,NULL,0))
    {
        perror("connect");
        res=mysql_query(&my_connection,"select * from app");//查询
        if(!res)
        {
            result=mysql_store_result(&my_connection);//保存查询到的数据到result
            if(result)
            {
                int i,j;
                printf("the result number is %lu\n ",(unsigned long)mysql_num_rows(result));
                for(i=0;fd=mysql_fetch_field(result);i++)//获取列名
                {
                    bzero(column[i],sizeof(column[i]));
                    strcpy(column[i],fd->name);
                }
                j=mysql_num_fields(result);
                for(i=0;i<j;i++)
                {
                    printf("%s\t",column[i]);
                }
                printf("\n");
                while(sql_row=mysql_fetch_row(result))//获取具体的数据
                {
                    for(i=0;i<j;i++)
                    {
                        printf("%s\t",sql_row[i]);
                    }
                    printf("\n");
                }
               
            }
        }
        else
        {
            perror("select");
        }
    }
    else
    {
        perror("connect:error");
    }
    mysql_free_result(MYSQL_RES *result);//释放结果资源
    mysql_close(&my_connection);//断开连接

}


static MYSQL     *p_conn=NULL;  
static MYSQL_RES *p_result=NULL;  
  
int main (int argc, char *argv[])  
{  
    mysql_init(p_conn);  
      
    if( NULL==mysql_real_connect(p_conn, "localhost", "root", "*****", "*****", 0, NULL, 0) )  
    {  
        printf("error %u: %s\n", mysql_errno(p_conn), mysql_error(p_conn));  
    }  
      
    if( mysql_query(p_conn, "select * from online_dev") )   
    {  
        printf("error %u: %s\n", mysql_errno(p_conn), mysql_error(p_conn));  
    }  
          
    while( NULL!=(row=mysql_fetch_row(p_result)) )  
    {         
        /******** 
        ********/         
    }  
      
    if( NULL!=p_result )  
    {  
        mysql_free_result(p_result);  
        p_result=NULL;  
    }  
	
    if( NULL!=p_conn )  
    {  
        mysql_close(p_conn);  
        p_conn=NULL;  
    }  
      
    return 0;     
}  


static MYSQL conn;  
static MYSQL_RES *p_result;  
  
int main (int argc, char *argv[])  
{  
    mysql_init(&conn);  
      
    if( NULL==mysql_real_connect(&conn, "localhost", "root", "*****", "*****", 0, NULL, 0) )  
    {  
        printf("error %u: %s\n", mysql_errno(&conn), mysql_error(&conn));  
    }  
      
    if( mysql_query(&conn, "select * from online_dev") )   
    {  
        printf("error %u: %s\n", mysql_errno(&conn), mysql_error(&conn));  
    }  
      
    p_result=mysql_use_result(&conn);  
    while( NULL!=(row=mysql_fetch_row(p_result)) )  
    {         
        /******** 
        ********/         
    }  
    mysql_free_result(p_result);  
      
    mysql_close(&conn);  
    return 0;     
}  


int main(void) 
{ 
　　MYSQL mysql, *sock; //声明MySQL的句柄 
　　const char * host = "127.0.0.1"; //因为是作为本机测试，所以填写的是本地IP 
　　const char * user = "root"; //这里改为你的用户名，即连接MySQL的用户名 
　　const char * passwd = "root"; //这里改为你的用户密码 
　　const char * db = "test"; //这里改为你要连接的数据库的名字,一个数据可能有几张表
　　unsigned int port = 3306; //这是MySQL的服务器的端口，如果你没有修改过的话就是3306。 
　　const char * unix_socket = NULL; //unix_socket这是unix下的，我在Windows下，所以就把它设置为NULL 
　　unsigned long client_flag = 0; //这个参数一般为0 

　　const char * i_query = "select * from person"; //查询语句，从那个表中查询,这里后面没有;

　　MYSQL_RES * result; //保存结果集的
　　MYSQL_ROW row; //代表的是结果集中的一行

　　//my_ulonglong row;

　　mysql_init(&mysql); //连接之前必须使用这个函数来初始化 
　　if ( (sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag) ) == NULL ) //连接MySQL 
　　{ 
　　　　printf("fail to connect mysql \n"); 
　　　　fprintf(stderr, " %s\n", mysql_error(&mysql)); 
　　　　exit(1); 
　　} 
　　else 
　　{ 
　　　　fprintf(stderr, "connect ok!!\n"); 
　　} 

　　if ( mysql_query(&mysql, i_query) != 0 ) //如果连接成功，则开始查询 .成功返回0
　　{ 
　　　　fprintf(stderr, "fail to query!\n"); 
　　　　exit(1); 
　　} 
　　else 
　　{ 
　　　　if ( (result = mysql_store_result(&mysql)) == NULL ) //保存查询的结果 
　　　　{ 
　　　　　　fprintf(stderr, "fail to store result!\n"); 
　　　　　　exit(1); 
　　　　} 
　　　　else 
　　　　{ 
　　　　　　　　while ( (row = mysql_fetch_row(result)) != NULL ) //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
　　　　　　　　{ 
　　　　　　　　　　printf("name is %s\t", row[0]); //打印当前行的第一列的数据 
　　　　　　　　　　printf("age is %s\t\n", row[1]); //打印当前行的第二列的数据

　　　　　　　　　　//row = mysql_num_row(result);
　　　　　　　　　　//printf("%lu\n", mysql_num_row(result));
　　　　　　　　} 
　　　　} 

　　}

　　mysql_free_result(result); //释放结果集 
　　mysql_close(sock); //关闭连接 
　　system("pause"); 
　　exit(EXIT_SUCCESS); 

}

#endif

