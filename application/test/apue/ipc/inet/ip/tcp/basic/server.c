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
	struct stu stu1;
	int sfd, ret, newsfd;
	socklen_t len;
	char strip[40];
	struct sockaddr_in sendaddr, recvaddr;
	sfd = socket(PF_INET, SOCK_STREAM, 0);
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
	ret = listen(sfd, 40);
	if(ret < 0){
		perror("listen()");
		exit(-1);
	}
	while(1){
		newsfd = accept(sfd, (struct sockaddr *)&sendaddr, &len);
		if(newsfd < 0){
			perror("accept()");
			exit(-1);
		}
		ret = read(newsfd, &stu1, sizeof(stu1));
		if(ret < 0){
			perror("read()");
			exit(-1);
		}
		inet_ntop(PF_INET, &sendaddr.sin_addr, strip, 40);
		printf("recive byte of : %d \n", ret);
		printf("port of sender : %x\n", ntohs(sendaddr.sin_port));
		printf("ip of sender   : %s\n", strip);
		printf("id = %d\n", ntohl(stu1.id));
		printf("name = %s\n", stu1.name);	
		printf("math = %d\n", ntohl(stu1.math));
		close(newsfd);
	}
	close(sfd);
	return 0;
}
