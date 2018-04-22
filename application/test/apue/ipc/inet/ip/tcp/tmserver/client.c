#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "proto.h"

int
main(int argc, char * argv[])
{
	char buf[256];
	int sfd, ret;
	socklen_t len;
	struct sockaddr_in sendaddr, recvaddr;
	sfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sfd < 0){
		perror("socket()");
		exit(-1);
	}
	recvaddr.sin_family = AF_INET;
	recvaddr.sin_port = htons(RECVPORT);
	inet_pton(PF_INET, "192.168.11.254", &recvaddr.sin_addr);
	len = sizeof(recvaddr);
	ret = connect(sfd, (void*)&recvaddr, len);
	if(ret < 0){
		perror("connect()");
		exit(-1);
	}
	ret = read(sfd, buf, sizeof(buf));
	if(ret < 0){
		perror("write()");
		exit(-1);
	}
	write(1, buf, ret);
	close(sfd);
	return 0;
}
