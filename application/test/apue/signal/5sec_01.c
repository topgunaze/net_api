#include <stdio.h>
#include <time.h>
#include <signal.h>
int flag;
void alarm_hande(int sig)
{
	flag = 1;
	//printf("time reached\n");
}
int
main(void)
{
	long long  count = 0;
	time_t tm;
	flag = 0;
	signal(SIGALRM, alarm_hande);
	alarm(5);
	
	while(1){
		count++;
		if(flag == 1)
			break;
	}
	printf("count= %lld\n", count);
	return 0;
}
