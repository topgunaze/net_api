#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define TNUM 4
#define START 20000
#define END   20200

static int num;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t * cond;
void * thr_fun(void * arg)
{
	int i, j, flag;

	while(1){
		pthread_mutex_lock(&mutex);
		while(num == -1){
			//pthread_mutex_unlock(&mutex);
			//usleep(1);
			//pthread_mutex_lock(&mutex);
			pthread_cond_wait(cond, &mutex); 
		}
		i = num;
		if(i==0){
			pthread_cond_broadcast(cond);
			pthread_mutex_unlock(&mutex);
			break;
		}
		else{
			num = -1;
		}
		pthread_cond_broadcast(cond);
		pthread_mutex_unlock(&mutex);
		
		flag = 1;
		for(j=2; j<i; j++){
			if(i%j == 0){
				flag = 0;
				break;
			}
		}
		if(flag == 1)
			printf("%d\n", i);
	}
	pthread_exit(NULL);
}
int
main(int argc, char * argv[])
{
	pthread_t tid[TNUM];
	int i, ret;
	num = -1;
	cond = malloc(sizeof(*cond));
	pthread_cond_init(cond, NULL);
	for(i=0; i<TNUM; i++){
		ret = pthread_create(tid+i, NULL, thr_fun, (void*)i);
		if(ret != 0){
			fprintf(stderr, "pthread_create():%s\n", strerror(ret));
			exit(-1);
		}
	}
	for(i=START; i<=END; i++){
		pthread_mutex_lock(&mutex);
		while(num != -1){
			//pthread_mutex_unlock(&mutex);
			//usleep(1);
			//pthread_mutex_lock(&mutex);
			pthread_cond_wait(cond, &mutex);	
		}
		num = i;
		pthread_cond_signal(cond);
		pthread_mutex_unlock(&mutex);
	}
	num = 0;
	pthread_cond_broadcast(cond);

	for(i=0; i<TNUM; i++){
		pthread_join(tid[i], 0);
	}
	pthread_cond_destroy(cond);
	free(cond);
	return 0;
}
