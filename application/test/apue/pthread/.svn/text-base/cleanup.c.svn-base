#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void fun(void * arg)
{
	printf("fun has run , arg = %p\n", arg);

}
int
main(void)
{
	pthread_cleanup_push(fun, (void *)0x100);
	pthread_cleanup_push(fun, (void *)0x101);
	pthread_cleanup_push(fun, (void *)0x102);
	return 0;
	pthread_exit(NULL);
	pthread_cleanup_pop(100);
	pthread_cleanup_pop(100);
	pthread_cleanup_pop(100);
}
