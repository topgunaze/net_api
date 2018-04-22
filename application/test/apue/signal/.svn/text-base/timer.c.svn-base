#include <stdio.h>
#include <signal.h>
void alarm_hande(int s)
{
	printf("1 sec timer reached\n");
	alarm(1);
}
main(void)
{
	alarm(1);
	signal(SIGALRM, alarm_hande);
	while(1){
		
	}
	return 0;
}
