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
struct fm{
	int state;
	int fd;
	int msgid;
	char * err;
};
enum {STARE_W, STARE_E, STARE_T};

int trans_element(struct fm * fm)
{
	struct mdata d;
	int len, ret;
	switch(fm->state){
		case STARE_W:
			len = read(fm->fd, d.data, sizeof(d.data));
			if(len < 0){
				fm->err = "read()";
				fm->state = STARE_E;
				return 0;
			}
			else if(len == 0){
				fm->state = STARE_T;
			}
			else{
				
			}
			d.type = 100;
			printf("send msg len = %d\n", len);
			ret = msgsnd(fm->msgid, &d, len, 0);
			if(ret < 0){
				fm->err = "msgsnd()";
				fm->state = STARE_E;
			}
				
		break;
		case STARE_E:
			perror(fm->err);
			fm->state = STARE_T;
		break;
		case STARE_T:
		break;
	}
}
int
main(int argc, char * argv[])
{
	key_t key;
	int len, ret;
	//char buf[256];
	//struct mdata d;
	struct fm fms;

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
	fms.state = STARE_W;
	fms.fd = fd;
	fms.msgid = msgid;
	while(1)
	{
		trans_element(&fms);
		if(fms.state == STARE_T){
			break;
		}
	}
	close(fd);
	return 0;
}
