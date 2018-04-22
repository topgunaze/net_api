#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void fun(int arg)
{
	printf("this is a fun\n");
}
void * thread_fun(void * arg)
{
	//*(int*)0 = 5;
	printf("[%u]:this is a sub thread arg = %d\n", pthread_self(), (int)arg);
	return (void*)0x300;
}
int
main(void)
{
	pthread_t tid;
	int ret;
	void * val;
	ret = pthread_create(&tid, NULL, thread_fun, (void*)10000);
	if(ret != 0){
		fprintf(stderr, "pthread_join():%s\n", strerror(ret));
		exit(-1);
		
	}
	//thread_fun((void*)10000);
	printf("this is a main thread\n");
	//sleep(1);
	//waitpid(tid, NULL, 0)
	ret = pthread_join(tid, &val);
	if(ret != 0){
		//errno -> ""
		fprintf(stderr, "pthread_join():%s\n", strerror(ret));
		exit(-1);
	}
	printf("sub thread ret = %p\n", val);
	return 0;
}
