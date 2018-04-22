#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>

int
main(int argc, char * argv[])
{
	int sfd, ret, slen, fd;
	char buf[256];
	struct sockaddr_in saddr;	
	socklen_t len ;
	len = sizeof(saddr);
	sfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sfd < 0)
	{
		perror("socket()");
		exit(-1);
	}
	saddr.sin_family = PF_INET;
	saddr.sin_port = htons(80);
	inet_pton(PF_INET, "192.168.11.254", &saddr.sin_addr);
	ret = connect(sfd, (void*)&saddr, len);
	if(ret < 0){
		perror("connect()");
		exit(-1);
	}
	char * s = "GET /1.png \r\n\r\n";
	fd = open("tmpfile", O_WRONLY|O_TRUNC|O_CREAT, 0644);
	if(fd < 0){
		perror("open()");
		exit(-1);
	}
	write(sfd, s, strlen(s));

	while(1){
		slen = read(sfd, buf, sizeof(buf));
		if(slen <= 0)
			break;
		ret = write(fd, buf, slen);
		if(ret < 0)
			break;
	}
	close(sfd);
	close(fd);
	return 0;
}
