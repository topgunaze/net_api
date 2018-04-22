#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "proto.h"
void child_handle(int sig)
{
	int ret;
	while(1){
		ret = waitpid(-1,NULL, WNOHANG);
		if(ret <= 0)
			break;
	}
}

int send_file(int sfd, int filefd, struct sockaddr_in * client)
{
	struct data data;
	int ret, slen, len;
	len = sizeof(*client);
	while(1){
		ret = read(filefd, data.buf, sizeof(data.buf));
		if(ret < 0){
			break;
		}
		usleep(2000);
		slen = sendto(sfd, &data, ret+4, 0, (void*)client, len);
		if(slen < 0){
			perror("sendto()");
			exit(-1);
		}
		if(ret == 0){
			break;
		}

	}
	return 0;	
}

int
main(int argc, char * argv[])
{
	struct data data;
	int sfd, ret, slen, childsfd;
	int fd;
	socklen_t len;
	char strip[40];
	pid_t pid;
	  
	struct sockaddr_in sendaddr, recvaddr;
	signal(SIGCHLD, child_handle);
	sfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket()");
		exit(-1);
	}
	len = sizeof(sendaddr);
	recvaddr.sin_family = PF_INET;
	recvaddr.sin_port = htons(SERVERPORT);
	//inet_pton(PF_INET, "192.168.11.254", &recvaddr.sin_addr);
	inet_pton(PF_INET, "0.0.0.0", &recvaddr.sin_addr);
	
	ret = bind(sfd, (struct sockaddr*)&recvaddr, len);
	if(ret < 0){
		perror("bind()");
		exit(-1);
	}
	while(1){
		ret = recvfrom(sfd, &data, sizeof(data), 0, (struct sockaddr*)&sendaddr, &len);
		if(ret < 0){
			perror("recvfrom()");
			exit(-1);
		}
		inet_ntop(PF_INET, &sendaddr.sin_addr, strip, 40);
		printf("recive byte of : %d \n", ret);
		printf("port of sender : %x\n", ntohs(sendaddr.sin_port));
		printf("ip of sender   : %s\n", strip);
		printf("file name : %s\n", data.buf);
		fd = open(data.buf, O_RDONLY);
		if(fd < 0){
			//perror("open()");
			//exit(-1);
			fprintf(stdout, "the file is not exist\n");
			continue;
		}	
		pid = fork();
		if(pid == 0){
			childsfd = socket(PF_INET, SOCK_DGRAM, 0);
			if(childsfd < 0){
				perror("socket()");
				exit(-1);
			}
			send_file(sfd, fd, &sendaddr);
			close(fd);
			close(childsfd);
			exit(0);
		}
		close(fd);
		//wait(NULL);
	}
	close(sfd);
	return 0;
}
