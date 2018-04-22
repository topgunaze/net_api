#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
FILE * mpopen(char * cmd, char * type)
{
	int pfd[2];
	FILE * fp;
	int ret;
	pid_t pid;
	ret = pipe(pfd);
	if(ret < 0)
		return NULL;
	pid = fork();
	if(pid < 0)
		return NULL;
	if(pid == 0){
		if(type[0] == 'r'){
			close(pfd[0]);
			ret = dup2(pfd[1], 1);
			if(ret < 0){
				perror("dup2()");
				return NULL;
			}
		}
		else if(type[0] == 'w'){
			close(pfd[1]);
			ret = dup2(pfd[0], 0);
			if(ret < 0)
				return NULL;
		}
		

		//ret = system(cmd);
		//ret = execlp(cmd, cmd, NULL);
		ret = execlp("/bin/bash", "bash", "-c", cmd, NULL);
		if(ret < 0){
			perror("system()");
			exit(-1);
			//return NULL;
		}
		exit(0);
	}

	//wait(NULL);
	if(type[0] == 'r'){
		close(pfd[1]);
		fp = fdopen(pfd[0], "r");
	}
	else if(type[0] == 'w'){
		close(pfd[0]);
		fp = fdopen(pfd[1], "w");
	}
	else{
		fprintf(stderr, "type error\n");
	}
	return fp;
}
int mpclose(FILE * fp)
{
	int ret;
	ret = wait(NULL);
	fclose(fp);
	return ret;
}
int
main(int argc, char * argv[])
{
	FILE * fp;
	char buf[256];
	int len;
	char * s;
	fp = mpopen("ls", "r");
	if(fp == NULL){
		perror("popen() === ");
		exit(-1);
	}
	while(1){
		s = fgets(buf, sizeof(buf), fp);
		if(s == NULL)
			break;
		printf("%s", buf);
	}
	mpclose(fp);
	return 0;
}
