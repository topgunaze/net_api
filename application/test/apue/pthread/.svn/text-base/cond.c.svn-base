#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

void * thr_fun(void * arg)
{
	sleep(1);
	pthread_mutex_lock(&mutex);
	printf("sub thread lock success\n");
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
}
int
main(int argc, char * argv[])
{
	pthread_t tid;
	int ret;
	ret = pthread_create(&tid, NULL, thr_fun, NULL);
	if(ret != 0){
		fprintf(stderr, "pthread_create():%s\n", strerror(ret));
		exit(-1);
	}
	pthread_mutex_lock(&mutex);
	while(1){
		//pthread_mutex_unlock(&mutex);
		//usleep(1);
		//pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		printf("main thread wake up\n");
		
	}
	pthread_mutex_unlock(&mutex);
	pthread_join(tid, NULL);
	return 0;
}
