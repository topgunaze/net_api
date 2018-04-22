#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char * argv[])
{
	int fd;
	char buf[256];
	int len, pos, ret;
	if(argc < 2){
		fd = 0;
	}
	else{
		fd = open(argv[1], O_RDONLY);
		if(fd < 0){
			perror("open()");
			return -1;
		}
	}
	while(1){
		len = read(fd, buf, sizeof(buf));
		if(len <= 0)
			break;
		pos = 0;
		while(len > 0){
			ret = write(1, buf+pos, len);
			pos += ret;
			len -= ret;
		}		
	}
	close(fd);
	return 0;
}
