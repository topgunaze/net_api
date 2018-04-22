//ls -l -a /etc/ppp

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(void)
{
	pid_t pid;
	char * arg[] = {
		"ls", "-l", "-a",
		"/etc/ppp", NULL,
	};
	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(-1);
	}
	if(pid == 0){
		execvp("ls", arg);
		exit(0);
	}
	wait(NULL);
	return 0;
}
