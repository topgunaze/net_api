#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "proto.h"

int
main(int argc, char * argv[])
{
	int msgid, ret;
	struct stu stu1;
	key_t key;	
	key = ftok(PATH, PROJ);
	if(key < 0){
		perror("ftok()");
		exit(-1);
	}
	msgid = msgget(key, IPC_CREAT|0600);
	if(msgid < 0){
		perror("msgget()");
		exit(-1);
	}
	ret = msgrcv(msgid, &stu1, sizeof(stu1)-sizeof(long)-0, 0, MSG_NOERROR);
	if(ret < 0){
		//perror("msgrcv()");
		if(errno == E2BIG){
			fprintf(stderr, "msgrcv():%s", "The message to big\n");
			exit(-1);
		}
		else if(errno == EINTR){
			//continue;
		}
		else{
			perror("msgrcv()");
			exit(-1);
		}
		exit(-1);
	}
	printf("read byte form msgqueue: %d\n", ret);
	printf("type = %ld\n", stu1.type);
	printf("id = %d\n", stu1.id);
	printf("name = %s\n", stu1.name);
	printf("math = %d\n", stu1.math);
	ret = msgctl(msgid, IPC_RMID, NULL);
	if(ret < 0){
		perror("msgctl()-- 01");
		exit(-1);
	}
#if 0
	ret = msgctl(msgid, IPC_RMID, NULL);
	if(ret < 0){
		perror("msgctl()-- 02");
		exit(-1);
	}
#endif
	return 0;
}
