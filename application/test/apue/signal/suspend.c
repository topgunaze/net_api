#include <stdio.h>
#include <signal.h>

void int_handle(int sig)
{
	//alarm(1);
}
void alarm_handle(int sig)
{
	alarm(1);
}
int
main(void){
	int i;
	sigset_t set, oset;
	signal(SIGINT, int_handle); 
	signal(SIGALRM, alarm_handle);
	alarm(1);
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	while(1){
		for(i=0; i<5; i++){
			write(1, ".", 1);
			//sleep(1);
			//sigprocmask(SIG_BLOCK, &set, &oset);
			//pause();
			//sigprocmask(SIG_SETMASK, &oset, NULL);
			sigsuspend(&set);
		}
		write(1, "\n", 1);
		
	}
	return 0;
}
