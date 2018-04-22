#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//ps -eo pid,ppid,comm | grep bash
int
main(void)
{
	pid_t pid;
	int pfd[2];
	int ret;
	ret = pipe(pfd);
	if(ret < 0){
		perror("pipe()");
		exit(-1);
	}
	pid = fork();
	if(pid == 0){
		//ps 
		close(pfd[0]);
		ret = dup2(pfd[1], 1);
		if(ret < 0){
			perror("dup2()");
			exit(-1);
		}
		ret = execlp("ps", "ps", "-eo", "pid,ppid,comm", NULL);
		if(ret < 0){
			perror("execlp()");
			exit(-1);
		}
		exit(0);		
	}
	pid = fork();
	if(pid == 0){
		close(pfd[1]);
		ret = dup2(pfd[0], 0);
		ret = execlp("grep", "grep", "bash", NULL);
		if(ret < 0){
			perror("execlp()");
			exit(-1);
		}
		exit(0);
	}
	close(pfd[0]);
	close(pfd[1]);
	wait(NULL);
	wait(NULL);
	return 0;
}

