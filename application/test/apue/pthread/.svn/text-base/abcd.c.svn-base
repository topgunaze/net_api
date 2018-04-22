#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#define TNUM   4
static int num;
pthread_mutex_t * mutex;
void * thr_fun(void * arg)
{
	int k = (int)arg;
	char buf[1];
	buf[0] = 'a' + k;
	while(1){
		pthread_mutex_lock(mutex);
		while(num != k){
			pthread_mutex_unlock(mutex);
			usleep(1);
			pthread_mutex_lock(mutex);
		}
		num = (num+1)%4;
		write(1, buf, 1);
		pthread_mutex_unlock(mutex);
	}
	pthread_exit(NULL);
}
int
main(int argc, char * argv[])
{
	pthread_t tid[TNUM];
	int i, ret ;
	num = 0;
	mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, NULL);
	for(i=0; i<TNUM; i++){
		ret = pthread_create(tid+i, NULL, thr_fun, (void*)i);
		if(ret != 0){
			fprintf(stderr, "thread create error\n");
			exit(-1);
		}
	}
	for(i=0; i<TNUM; i++){
		pthread_join(tid[i], NULL);
	}
	pthread_mutex_destroy(mutex);
	free(mutex);
	return 0;
}
