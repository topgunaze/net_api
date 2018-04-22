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
	char * name = "Peter"; 
	struct sockaddr_in sendaddr, recvaddr;
	stu1 = malloc(sizeof(*stu1)+strlen(name));
	stu1->id = htonl(100);
	strcpy(stu1->name, name);
	stu1->math = htonl(99);
	sfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket()");
		exit(-1);
	}
	ret = setsockopt(sfd, SOL_SOCKET, SO_BINDTODEVICE, "eth0", 5);
	if(ret < 0){
		perror("setsockopt()");
		exit(-1);
	}
	//sfd :
	//PF_INET
	//192.168.11.2**
	//port : 0x****
	// -->
	//sfd :
	//PF_INET
	//192.168.11.2**
	//port : 0x1991

	// bind
	// struct sockaddr_in sendaddr
  // sendaddr.sin_port = htons(0x1991)
	// sendaddr.sin_family = PF_INET;
	// sendaddr.sin_addr = 192.168.11.2**

#if 1
	sendaddr.sin_port = htons(0x1991);
	sendaddr.sin_family = PF_INET;
	inet_pton(PF_INET, "192.168.11.254", &sendaddr.sin_addr);
	len = sizeof(sendaddr);
	ret = bind(sfd, (void*)&sendaddr, len);
	if(ret < 0){
		perror("bind()");
		exit(-1);
	}
#endif
#if 0
	recvaddr.sin_family = PF_INET;
	recvaddr.sin_port = htons(RECVPORT);
	//inet_pton(PF_INET, "192.168.11.254", &recvaddr.sin_addr);
	inet_pton(PF_INET, "0.0.0.0", &recvaddr.sin_addr);
#endif
	recvaddr.sin_family = AF_INET;
	recvaddr.sin_port = htons(RECVPORT);
	//inet_pton(PF_INET, "192.168.11.254", &recvaddr.sin_addr);
	inet_pton(PF_INET, MADDR, &recvaddr.sin_addr);
	ret = sendto(sfd, stu1, sizeof(*stu1)+strlen(name), 0, (struct sockaddr*)&recvaddr, len);
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
