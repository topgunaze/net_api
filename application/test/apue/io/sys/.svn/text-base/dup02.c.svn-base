#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int
main(void)
{
	int a;
	int fd, newfd;
	fd = open("t.txt", O_RDONLY);
	if(fd < 0){
		perror("open()");
		return 0;
	}
	//close(0);
	//newfd = dup(fd);
	newfd = dup2(fd, 0);
	printf("newfd = %d\n", newfd);
//===================
	scanf("%d", &a);
	printf("a = %d\n", a);
	return 0;
}
