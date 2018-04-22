#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "pipe.h"

void * thr_fun(void * arg)
{
	char buf[256];
	int ret;
	while(1){
		ret = pipe_read(buf, sizeof(buf));
		//printf("ret = %d\n", ret);
		if(ret <= 0)
			break;
		if(ret > 0)
			write(1, buf, ret);
	}
	pthread_exit(NULL);
}

int
main(int argc, char * argv[])
{
	pthread_t tid;
	int ret, len, pos;
	char buf[256];
	//int i, d;
	pipe_init();
#if 0
	for(i=0; i<10; i++){
		ret = enqueue(i);
		if(ret < 0){
			printf("queue is full\n");
			exit(-1);
		}
	}
	for(i=0; i<10; i++){
		ret = dequeue(&d);
		if(ret < 0){
			printf("queue is empty\n");
			exit(-1);
		}
		printf("%d\n", d);
		
	}
	for(i=0; i<10; i++){
		ret = enqueue(i);
		if(ret < 0){
			printf("queue is full\n");
			exit(-1);
		}
	}
	for(i=0; i<10; i++){
		ret = dequeue(&d);
		if(ret < 0){
			printf("queue is empty\n");
			exit(-1);
		}
		printf("%d\n", d);
		
	}
	return 0;
#endif
	ret = pthread_create(&tid, NULL, thr_fun, NULL);
	if(ret != 0){
		fprintf(stderr, "pthread_create():%s\n", strerror(ret));
		exit(-1);
	}
	//ret = pipe_write("uplooking", 9);
	//printf("write byte of %d\n", ret); 
	while(1){
		ret = read(0, buf, sizeof(buf));
		if(ret <= 0)
			break;
		pos = 0;
		while(ret > 0){
			len = pipe_write(buf+pos, ret);
			ret -= len;
			pos += len;
		}
		
	}
	pipe_close();
	pthread_join(tid, NULL);
	return 0;
}
