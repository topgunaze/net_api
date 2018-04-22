#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

static void alarm_hande(int sig)
{
	alarm(1);
}
static void int_hande(int sig)
{
	printf("catch a signal of SIGINT\n");
}
int
main(void)
{
	int len;
	char buf[10];
	sigset_t set, oset;
	signal(SIGINT, int_hande);
	signal(SIGALRM, alarm_hande);
	alarm(1);
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	while(1){
		len = read(0, buf, sizeof(buf));
		if(len < 0){
			if(errno == EINTR)
				continue;
			break;
		}
		if(len == 0)
			break;
		//if(len > 0)
		write(1, buf, len);
		//sleep(1);

		//sigprocmask(SIG_BLOCK, &set, &oset);
		//pause();
		//sigprocmask(SIG_SETMASK, &oset, NULL);		
		sigsuspend(&set);
	}
	return 0;
}
