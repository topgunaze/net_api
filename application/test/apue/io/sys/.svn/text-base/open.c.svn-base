#include <stdio.h>
#include <fcntl.h>


int
main(void)
{
	int fd, fd01;
	close(0);
	fd = open("t.txt", O_RDWR);
	if(fd < 0){
		perror("open()===:");
		return -1;
	}
	printf("fd = %d\n", fd);
	fd01 = open("x.txt", O_RDONLY);
	if(fd01 < 0){
		perror("open()");
		return -1;
	}
	printf("fd01 = %d\n", fd01);
	return 0;
}
