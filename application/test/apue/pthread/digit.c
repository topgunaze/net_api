
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
static int digit;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

void * thr_fun0(void * arg)
{
	int i;
	if(arg == 0){
		for(i=0; i<10; i++){
			pthread_mutex_lock(&mutex);			
			while(digit == 1){
				pthread_mutex_unlock(&mutex);			
				usleep(1);
				pthread_mutex_lock(&mutex);			
			}
			printf("%d ", i*2);
			fflush(stdout);
			digit = 1;
			pthread_mutex_unlock(&mutex);			
		}
	}
	else{
		for(i=0; i<10; i++){
			pthread_mutex_lock(&mutex);			
			while(digit == 0){
				pthread_mutex_unlock(&mutex);			
				usleep(1);
				pthread_mutex_lock(&mutex);			
			}
			printf("%d ", i*2+1);
			fflush(stdout);
			digit = 0;
			pthread_mutex_unlock(&mutex);			
		}
		
	}
	pthread_exit(NULL);
}
#if 0
void * thr_fun1(void * arg)
{
	
}
#endif

int
main(int argc, char * argv[])
{
	int i, ret;
	pthread_t tid[2];
	void * retval;
	digit = 0;
	for(i=0; i<2; i++){
		ret = pthread_create(tid+i, NULL, thr_fun0, (void*)i); 
		if(ret != 0){
			fprintf(stderr, "pthread_create():%s\n", strerror(ret));
			exit(-1);
		}
	}
	for(i=0; i<2; i++){
		pthread_join(tid[i], &retval);	
	}
	printf("\n");
	return 0;
}
