#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int 
main(void)
{
	pid_t pid;
	int status;
#if 1
	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(-1);
	}
	if(pid == 0){
		printf("[%d]: this is child \n", getpid());
		*(int *)0 = 500;
		sleep(3);	
		exit(0);
	}
#endif

	pid = wait(&status);
	//sleep(1);
	printf("[%d]: this is parent \n", getpid());
	printf("wait return val : %d\n", pid);
	printf("status = %d\n", status);
	if(WIFEXITED(status)){
		printf("program is exited nomally\n");
	}
	else{
		printf("program is not exited nomally\n");
	}
	return 0;
}
