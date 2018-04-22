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
      int msgid;
      int fd;
      int len,ret;
      struct mdata d;
      key = ftok(MFILE, PROJ);
      if(key < 0){
	    perror("ftok()");
	    exit(-1);
      }
      msgid = msgget(key, 0);
      if(msgid < 0){
	    perror("msgget()");
	    exit(-1);
      }
      fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
      if(fd < 0){
	    perror("open()");
	    exit(-1);
      }
      while(1){
	    ret = msgrcv(msgid, &d, sizeof(d.data), 0, 0);
	    if(ret < 0){
		  perror("msgsnd()");
		  exit(-1);
	    }
	    if(ret == 0)
		  break;
	    len = write(fd, d.data, ret);
	    if(len < 0){
		  perror("write()");
		  exit(-1);
	    }
      }
      close(fd);
      return 0;
}
