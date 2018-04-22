#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int 
main(void)
{
	pid_t pid;
	int status;
#if 0 
	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(-1);
	}
	if(pid == 0){
		printf("[%d] :child \n", getpid());
		exit(0);
	}
#endif
	sleep(1);
	pid = waitpid(-1, NULL, WNOHANG);
	printf("pid = %d\n", pid);
	return 0;
}
