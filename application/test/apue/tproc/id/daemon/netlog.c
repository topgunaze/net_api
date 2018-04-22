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
	sfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket()");
		exit(-1);
	}
	recvaddr.sin_family = AF_INET;
	recvaddr.sin_port = htons(514);
	inet_pton(PF_INET, "192.168.11.254", &recvaddr.sin_addr);
	len = sizeof(recvaddr);
	ret = sendto(sfd, "uplooking net message\n", strlen("uplooking net message\n"), 0, (struct sockaddr*)&recvaddr, len);
	if(ret < 0){
		perror("sendto()");
		exit(-1);
	}
	//ineth:
	//iph:
	//192.168.11.254(param 5 recvaddr) - 192.168.11.254(sfd)
	//udph:
  //0x1990(param 5 recvaddr)  - 0x****(sfd)
	//data
	close(sfd);
	return 0;
}
