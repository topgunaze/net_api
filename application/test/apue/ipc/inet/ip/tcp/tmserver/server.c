#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

#include "proto.h"
void child_handle(int s)
{
	int ret;
	while(1){
		ret = waitpid(-1, NULL, WNOHANG);
		if(ret <= 0)
			break;
	}
}
int
main(int argc, char * argv[])
{
	struct stu stu1;
	char buf[256];	
	int sfd, ret, newsfd;
	socklen_t len;
	char strip[40];
	pid_t pid;
	struct sockaddr_in sendaddr, recvaddr;
	signal(SIGCHLD, child_handle);
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
		pid = fork();
		if(pid == 0){
			ret = sprintf(buf, "This time is : %d\n", time(NULL));
			write(newsfd, buf, ret);
			inet_ntop(PF_INET, &sendaddr.sin_addr, strip, 40);
			printf("recive byte of : %d \n", ret);
			printf("port of sender : %x\n", ntohs(sendaddr.sin_port));
			printf("ip of sender   : %s\n", strip);
			close(newsfd);
		}
		close(newsfd);
		//wait(NULL);
	}
	close(sfd);
	return 0;
}
