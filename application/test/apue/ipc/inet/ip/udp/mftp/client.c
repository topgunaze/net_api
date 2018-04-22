#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "proto.h"

int
main(int argc, char * argv[])
{
	struct data data;
	int sfd, ret;
	socklen_t len;
	struct sockaddr_in sendaddr, recvaddr;
	sfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket()");
		exit(-1);
	}
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
	recvaddr.sin_port = htons(SERVERPORT);
	inet_pton(PF_INET, "192.168.11.254", &recvaddr.sin_addr);
	strcpy(data.buf, argv[1]);
	ret = sendto(sfd, &data, sizeof(data), 0, (struct sockaddr*)&recvaddr, len);
	if(ret < 0){
		perror("sendto()");
		exit(-1);
	}
	int fd;
	int slen;
	fd = open("tmpfile", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if(fd < 0){
		perror("open()");
		exit(-1);
	}
	while(1){
		len = sizeof(recvaddr);
		ret = recvfrom(sfd, &data, sizeof(data), 0, (struct sockaddr*)&recvaddr, &len);
		if(ret < 0){
			perror("recvfrom()");
			exit(-1);
		}
		if(ret == 4){
			break;
		}
		slen = write(fd, data.buf, ret-4);
		if(slen < 0)
		{
			perror("write()");
			exit(-1);
		}	

	}
	close(sfd);
	close(fd);
	return 0;
}
