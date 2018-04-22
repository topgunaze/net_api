#include <stdio.h>
#include <signal.h>

void int_handle(int sig)
{

	printf("hello \n");
}
void int_ign(int sig)
{
}
int
main(void){
	int i;
	signal(SIGINT, int_handle); 
	//signal(SIGINT, SIG_IGN); 
	while(1){
		for(i=0; i<5; i++){
			write(1, ".", 1);
			sleep(1);
		}
		write(1, "\n", 1);
		
	}
	return 0;
}
