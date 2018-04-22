#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char * argv[])
{
	int ret;
	pid_t pid;
	int fd;
	ret = umask(0);
	if(ret < 0)
		return -1;
	pid = fork();
	if(pid < 0)
		return -1;
	else if(pid == 0){
		pid = setsid();
		if(pid < 0)
			return -1;
		ret = chdir("/");
		if(ret < 0)
			return -1;
#if 1 
		close(0);
		close(1);
		close(2);
		fd = open("/dev/null", O_RDWR);
		if(fd < 0){
			perror("open()");
			return -1;
		}
		ret = dup2(fd, 0);
		if(ret < 0)
			return -1;
		ret = dup2(fd, 1);
		if(ret < 0)
			return -1;
		ret = dup2(fd, 2);
		if(ret < 0)
			return -1;
#endif
		while(1){	
			printf("test:\n");
			sleep(1);
		}
	}
	else{
		exit(0);
	}
	return 0;
}
