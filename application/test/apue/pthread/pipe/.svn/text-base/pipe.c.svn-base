#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#define MSIZE  10
static char buf[MSIZE];
static int rear, head, size;
int writepoint;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
int pipe_init(void)
{
	rear = -1;
	head = 0;
	size = 0;
	writepoint = 1;
	return 0;
}
int enqueue(int d)
{
	if(size == MSIZE)
		return -1;
	
	rear = (rear + 1)%MSIZE;
	buf[rear] = d;
	size++;
	return 0;
}
int dequeue(int * d)
{
	if(size == 0)
		return -1;
	*d = buf[head];
	head = (head + 1) % MSIZE;
	size--;
	return 0;
}
int pipe_write(char * buf, int size)
{
	int i, ret;
	pthread_mutex_lock(&mutex);
	for(i=0; i<size; i++){
		ret = enqueue(buf[i]);
		if(ret < 0)
			break;
	}
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	return i;
}

int pipe_read(char * buf, int size)
{
	int i, ret, d;
	pthread_mutex_lock(&mutex);
	for(i=0; i<size; ){
		ret = dequeue(&d);
		if(ret < 0){
			if(i == 0){
				if(writepoint == 0)
					break;
				pthread_cond_wait(&cond, &mutex);
			}
			else{
				break;
			}
		}
		else{
			buf[i] = d;
			i++;
		}
	}
	pthread_mutex_unlock(&mutex);
	return i;
}
int pipe_close(void)
{
	pthread_mutex_lock(&mutex);
	writepoint--;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	return 0;
}
