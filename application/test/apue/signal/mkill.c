#include <stdio.h>
#include <signal.h>
#include <string.h>

//./kill 2   pid
//./kill 2   sig 

int
main(int argc, char * argv[])
{
	int ret;
	pid_t pid;
	char buf[256];
	FILE * fp;
	int pfd[2];
	size_t n;
	char * p, * s;
	ret = pipe(pfd);
	if(ret < 0){
	}
	pid = fork();
	if(pid == 0){
		close(pfd[0]);
		ret = dup2(pfd[1], 1);
		if(ret < 0){
		}
		sprintf(buf, "%s %s", "ps -eo pid,comm | grep", argv[2]);
		system(buf);
	}
	close(pfd[1]);
	n = 0;
	p = NULL;
	fp = fdopen(pfd[0], "r");
	if(fp == NULL){
	}
	while(1){
		ret = getline(&p, &n, fp);
		if(ret < 0)	
			break;
		s = strsep(&p, " ");
		printf("%s\n", s);
		kill(atoi(s), atoi(argv[1]));
	}
	wait(NULL);
	return 0;
}
