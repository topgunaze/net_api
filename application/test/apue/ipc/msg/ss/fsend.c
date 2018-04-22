#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<fcntl.h>
#include"proto.h"

int main(int argc,char * argv[])
{
      key_t key;
      int msgid,fd;
      int len,ret;
      struct mdata d;
      key = ftok(MFILE,PROJ);
      if(key < 0){
	    perror("ftok()");
	    exit(-1);
      }
      msgid = msgget(key,IPC_CREAT|0644);
      if(msgid < 0){
	    perror("msgget()");
	    exit(-1);
      }
      fd = open(argv[1],O_RDONLY);
      if(fd < 0){
	    perror("open()");
	    exit(-1);
      }
      d.type = 100;
      while(1){
	    len = read(fd,d.data,sizeof(d.data));
	    if(len < 0){
		  break;
	    }
	    ret = msgsnd(msgid,&d,len,0);
	    if(ret < 0){
		  perror("msgsnd()");
		  exit(-1);
	    }
	    if(len == 0){
		  break;
	    }
      }
      close(fd);
      return 0;
}
