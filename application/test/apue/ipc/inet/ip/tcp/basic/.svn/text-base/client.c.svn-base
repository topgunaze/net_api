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
	struct stu stu1 = {.id=htonl(100), .name="Sam", .math=htonl(99)};
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
	ret = write(sfd, &stu1, sizeof(stu1));
	if(ret < 0){
		perror("write()");
		exit(-1);
	}
	close(sfd);
	return 0;
}
