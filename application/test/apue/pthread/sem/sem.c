#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

static int num;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
int sem_init(int n)
{
	pthread_mutex_lock(&mutex);
	num = n;
	pthread_mutex_unlock(&mutex);
	return 0;
}

int sem_op(int n)
{
	pthread_mutex_lock(&mutex);
	while(num + n < 0){
		pthread_cond_wait(&cond, &mutex);
	}
	num += n;
	if(n > 0)
		pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
	return 0;
}



