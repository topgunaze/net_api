#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "proto.h"

#define SENDPORT 0x1991
int
main(int argc, char * argv[])
{
	struct stu stu1 = {.id=htonl(234), .name="zhangwufeng", .math=htonl(99)};
	int sfd, ret;
	socklen_t len;
	struct sockaddr_in sendaddr, recvaddr;
	sfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket()");
		exit(-1);
	}
	len = sizeof(sendaddr);
#if 1
	sendaddr.sin_family = AF_INET;
	sendaddr.sin_port = htons(SENDPORT);
	inet_pton(PF_INET, "192.168.11.234", &sendaddr.sin_addr);
	ret = bind(sfd, (struct sockaddr*)&sendaddr, len);
//inet_pton(PF_INET, "192.168.11.254", &recvaddr.sin_addr);
//	inet_pton(PF_INET, "0.0.0.0", &recvaddr.sin_addr);
#endif
	recvaddr.sin_family = AF_INET;
	recvaddr.sin_port = htons(RECVPORT);
	inet_pton(PF_INET, "192.168.11.234", &recvaddr.sin_addr);
	ret = sendto(sfd, &stu1, sizeof(stu1), 0, (struct sockaddr*)&recvaddr,len);
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
