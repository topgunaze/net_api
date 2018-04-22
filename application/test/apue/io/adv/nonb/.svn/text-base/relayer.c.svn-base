#include <stdio.h>
#include <fcntl.h>
#define TTY0 "/dev/tty10"
#define TTY1 "/dev/tty11"

int
main(void)
{
	int fd0, fd1;
	char buf[256];
	int len;
	fd0 = open(TTY0, O_RDWR);
	if(fd0 < 0){
		perror("open()");
		return -1;
	}
	fd1 = open(TTY1, O_RDWR);
	if(fd1 < 0){
		perror("open()");
		return -1;
	}
	write(fd0, "TTY0:\n", 6); 
	write(fd1, "TTY1:\n", 6); 
	while(1){
		len = read(fd0, buf, sizeof(buf));
		if(len < 0){
			perror("read()");
			return -1;
		}
		if(len == 0)
			break;
		write(fd1, buf, len);
		len = read(fd1, buf, sizeof(buf));
		if(len < 0){
			perror("read()");
			return -1;
		}
		if(len == 0)
			break;
		write(fd0, buf, len);
	}
	close(fd0);
	close(fd1);
	return 0;
}
