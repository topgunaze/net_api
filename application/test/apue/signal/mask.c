#include <stdio.h>
#include <signal.h>

void int_handle(int sig)
{
	printf("hello \n");
}
int
main(void){
	int i;
	sigset_t set, oset;
	signal(SIGINT, int_handle); 
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	while(1){
		sigprocmask(SIG_BLOCK, &set, &oset);
		for(i=0; i<5; i++){
			write(1, ".", 1);
			sleep(1);
		}
		write(1, "\n", 1);
		sigprocmask(SIG_SETMASK, &oset, NULL);
	}
	//sigprocmask(SIG_UNBLOCK, &set, NULL);
	return 0;
}
