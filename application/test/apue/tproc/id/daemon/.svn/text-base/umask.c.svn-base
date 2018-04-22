
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>

int
main(int argc, char * argv[])
{
	int fd;
	fd = open("t.txt", O_WRONLY|O_CREAT, 0777);
	if(fd < 0){
		perror("open()");
		exit(-1);
	}
	printf("file of t.txt access mode should be: %o\n",  0777 & ~0555);
	return 0;
}
