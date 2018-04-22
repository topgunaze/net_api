#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int msystem(char * cmd)
{
	pid_t pid;
	int ret;
	pid = fork();
	if(pid < 0)
		return -1;
	if(pid == 0){
		ret = execlp("/bin/bash", "sh", "-c", cmd, NULL);
		if(ret < 0){
			perror("execlp()");
			exit(-1);
		}
	}
	wait(NULL);
	return 0;
}
int
main(void)
{
	//ls -l
	int ret;
	ret = msystem("ls -l | cat | grep msh");
	if(ret < 0){
		perror("system()");
		exit(-1);
	}
	return 0;
}
