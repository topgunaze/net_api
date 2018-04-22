#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	int ret;
	ret = execl(argv[1], argv[1], NULL);
	if(ret < 0){
		perror("execl()");
		return -1;
	}
	return 0;
}
