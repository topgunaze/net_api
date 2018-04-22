#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "proto.h"

int
main(int argc, char * argv[])
{
	key_t key;
	int len, ret;
	//char buf[256];
	struct mdata d;

	int fd;
	int msgid;
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
	//d.type = 100;
	while(1){
#if 0
		len = read(fd, d.data, sizeof(d.data));
		if(len < 0){
			break;
		}
		if(len == 0){
			break;
		}
#endif
//  switch(stat)
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
	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}
