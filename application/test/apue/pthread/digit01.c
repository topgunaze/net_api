#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
static int  num;
pthread_mutex_t * mutex;
void * thr_fun(void *arg)
{
	if((int)arg == 0){
		//thread 0
		while(1){
			pthread_mutex_lock(mutex);
			while(num % 2 != 0){
				pthread_mutex_unlock(mutex);
				usleep(1);
				pthread_mutex_lock(mutex);
			}
			printf("%d ", num);
			fflush(stdout);
			num++;
			if(num >= 19){
				pthread_mutex_unlock(mutex);
				break;
			}
			pthread_mutex_unlock(mutex);
		}
	}
	else{
		//thread 1
		while(1){
			pthread_mutex_lock(mutex);
			while(num % 2 == 0){
				pthread_mutex_unlock(mutex);
				usleep(1);
				pthread_mutex_lock(mutex);
			}
			printf("%d ", num);
			fflush(stdout);
			num++;
			if(num >= 19){
				pthread_mutex_unlock(mutex);
				break;
			}
			pthread_mutex_unlock(mutex);
		}
	}
	pthread_exit(NULL);
}
int
main(int argc, char * argv[])
{
	int i, ret;
	pthread_t tid[2];
	num = 0;
	mutex = malloc(sizeof(pthread_mutex_t));
	//*mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(mutex, NULL);
	for(i=0; i<2; i++){
		ret = pthread_create(tid+i, NULL, thr_fun, (void*)i);
		if(ret != 0){
			fprintf(stderr, "pthread_create():%s\n", strerror(ret));
			exit(-1);
		}
	}
	for(i=0; i<2; i++){
		pthread_join(tid[i], NULL);
	}
	printf("\n");
	pthread_mutex_destroy(mutex);
	free(mutex);
	return 0;
}
