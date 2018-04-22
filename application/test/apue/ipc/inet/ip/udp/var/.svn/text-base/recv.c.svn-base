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
	struct stu * stu1;
	int sfd, ret;
	socklen_t len;
	char strip[40];
	struct sockaddr_in sendaddr, recvaddr;
	sfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket()");
		exit(-1);
	}
	len = sizeof(sendaddr);
	recvaddr.sin_family = PF_INET;
	recvaddr.sin_port = htons(RECVPORT);
	//inet_pton(PF_INET, "192.168.11.254", &recvaddr.sin_addr);
	inet_pton(PF_INET, "0.0.0.0", &recvaddr.sin_addr);
	
	ret = bind(sfd, (struct sockaddr*)&recvaddr, len);
	if(ret < 0){
		perror("bind()");
		exit(-1);
	}
	char fatebuf[1024];
	int recvlen;
	while(1){
		recvlen = recvfrom(sfd, fatebuf, sizeof(fatebuf), MSG_PEEK, (struct sockaddr*)&sendaddr, &len);
		if(recvlen < 0){
			perror("recvfrom()");
			exit(-1);
		}
		
		stu1 = malloc(recvlen);
		ret = recvfrom(sfd, stu1, recvlen, 0, (struct sockaddr*)&sendaddr, &len);
		if(ret < 0){
			perror("recvfrom()");
			exit(-1);
		}
		inet_ntop(PF_INET, &sendaddr.sin_addr, strip, 40);
		printf("recive byte of : %d \n", ret);
		printf("port of sender : %x\n", ntohs(sendaddr.sin_port));
		printf("ip of sender   : %s\n", strip);
		printf("id = %d\n", ntohl(stu1->id));
		printf("name = %s\n", stu1->name);	
		printf("math = %d\n", ntohl(stu1->math));
	}
	close(sfd);
	return 0;
}
