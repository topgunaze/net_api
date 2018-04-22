#include <stdio.h>
#include <signal.h>

void int_handle(int sig)
{
	printf("hello \n");
}
int
main(void){
	int i;
	struct sigaction act, oact;
	act.sa_handler = int_handle; 
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, &oact);
	while(1){
		for(i=0; i<5; i++){
			write(1, ".", 1);
			sleep(1);
		}
		write(1, "\n", 1);
		
	}
	sigaction(SIGINT, &oact, NULL);
	return 0;
}
