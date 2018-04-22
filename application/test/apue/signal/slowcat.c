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
	char buf[15];
	signal(SIGINT, int_hande);
	signal(SIGALRM, alarm_hande);
	signal(SIGALRM, alarm_hande);                             /*clock*/
	alarm(1);
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
		pause();
		
	}
	return 0;
}
