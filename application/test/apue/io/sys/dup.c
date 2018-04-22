#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int
main(void)
{
	int fd, newfd, ret;
	char buf[256];
	fd = open("t.txt", O_RDONLY);
	if(fd < 0){
		perror("open()");
		return -1;
	}
	newfd = dup(fd);
	printf("newfd = %d\n", newfd);
	printf("fd = %d\n", fd);
	ret = read(newfd, buf, sizeof(buf));
	buf[ret] = '\0';
	printf("%s", buf);
	close(fd);
	return 0;
}
