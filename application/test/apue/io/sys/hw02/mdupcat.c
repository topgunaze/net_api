#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char * argv[])
{
	int fd;
	char buf[10];
	int ret, len;
	fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		perror("open()");
	}
	ret = dup2(fd, 0);
	if(ret < 0){
		perror("dup2()");
		return -1;
	}
	while(1){
		len = read(0, buf, sizeof(buf));
		if(len <= 0)
			break;
		write(1, buf, len);
	}
	close(fd);
	return 0;
}
