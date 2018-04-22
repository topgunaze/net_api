#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
int
main(int argc, char * argv[])
{
	pid_t pid;
	pid = fork();
	if(pid == 0){
		setpgid(getpid(), getpid()); 
		sleep(2);
		//setpgid(0,0);
		execlp("/bin/sh", "sh", "-c", "ps -o pid,pgrp,tpgid,sid,comm", NULL); 
		exit(0);
	}
	sleep(1);
	tcsetpgrp(0, pid);	
	wait(NULL);
	return 0;
}
