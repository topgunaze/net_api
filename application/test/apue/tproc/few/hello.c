#include <stdio.h>
#include <unistd.h>

int
main(void)
{	
	int ret;
	printf("test execl()\n");
	ret =execlp("ls", "ls", NULL);
	if(ret < 0){
		perror("execl()");
		return -1;
	}
	printf("hello\n");
	return 0;
}
