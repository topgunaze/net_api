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
	msgid = msgget(key, IPC_CREAT|0644);
	if(msgid < 0){
		perror("msgget()");
		exit(-1);
	}
	fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		perror("open()");
		exit(-1);
	}
	d.type = 100;
	while(1){
		len = read(fd, d.data, sizeof(d.data));
		if(len < 0){
			break;
		}
		ret = msgsnd(msgid, &d, len, 0);
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
