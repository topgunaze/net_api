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
	struct stu stu1 = {.id=99, .name="Peter", .math=100, .type = 2};
	//stu1 = {99, "Peter", 100};
	key_t key;	
	key = ftok(PATH, PROJ);
	if(key < 0){
		perror("ftok()");
		exit(-1);
	}
	msgid = msgget(key, 0);
	if(msgid < 0){
		perror("msgget()");
		exit(-1);
	}
	ret = msgsnd(msgid, &stu1, sizeof(stu1)-sizeof(long), 0);
	//ret = msgsnd(msgid, &stu1, 0, 0);
	if(ret < 0){
		perror("msgsnd()");
		exit(-1);
	}
	


	
	return 0;
}
