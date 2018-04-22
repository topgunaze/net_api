#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int 
main(void)
{
	pid_t pid;
	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(-1);
	}
	if(pid == 0){
		printf("[%d]: this is child \n", getpid());
		exit(0);
	}
	printf("[%d]: this is parent \n", getpid());
	return 0;
}
