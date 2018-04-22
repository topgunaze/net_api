#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#define  TNUM   4
#define  PI     3.1415926
#define  PARTS  1000000

double area;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//struct stu{int id ,char name[]};
//struct stu stu1 = {0, "xiangzhang"};
// stu1 = {0, "xiangzhang"}; error
// stu1.id = 0;
// strcpy(stu1.name, "xiaozhang");

void * thr_fun(void * arg)
{
	int i;
	double a;
	a = sin(2*PI/PARTS) * cos(2*PI/PARTS) / 2;
	//area = 0.0;
	for(i=0; i<PARTS/TNUM; i++){
		pthread_mutex_lock(&mutex);
		area += a;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);	
}
int 
main(int argc, char * argv[]) 
{ 
	pthread_t tid[TNUM];
	int i, ret;
	area = 0.0;
	for(i=0; i<TNUM; i++){
		ret = pthread_create(tid+i, NULL, thr_fun, NULL);
		if(ret != 0){
			fprintf(stderr, "thread error\n");
			exit(-1);
		}
	}
	for(i=0; i<TNUM; i++){
		pthread_join(tid[i], NULL);
	}
	printf("area = %f\n", area);
	return 0; 
}
