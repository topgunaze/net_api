#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
int
main(int argc, char * argv[])
{
	FILE * fp;
	fp = popen("cat", "w");
	if(fp == NULL){
		perror("popen()");
		exit(-1);
	}
	fprintf(fp, "hello world\n");
	fprintf(fp, "uplooking\n");
	//pclose(fp);
	while(1);
	return 0;
}
