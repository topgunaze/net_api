#include <stdio.h>
#include <time.h>
#include <signal.h>

void alarm_hande(int sig)
{
	printf("time reached\n");
}
int
main(void)
{
	long long  count = 0;
	time_t tm;
	//alarm(2);
	tm = time(NULL);
	while(time(NULL)-tm <= 5){
		count++;
	}
	printf("count= %lld\n", count);
	return 0;
}
