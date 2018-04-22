#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char * argv[])
{
	int fd, dfd;
	char buf[256];
	int len, pos, ret;
	if(argc < 3){
		fprintf(stderr, "usage ./mcp t.txt x.txt\n");
		return -1;
	}
	
	fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		perror("open()");
		return -1;
	}
	dfd = open(argv[2], O_RDWR|O_TRUNC|O_CREAT, 0655);
	if(dfd < 0){
		perror("open()");
		return -1;
	}
	
	while(1){
		len = read(fd, buf, sizeof(buf));
		if(len <= 0)
			break;
		pos = 0;
		while(len > 0){
			ret = write(dfd, buf+pos, len);
			pos += ret;
			len -= ret;
		}		
	}
	close(fd);
	close(dfd);
	return 0;
}
