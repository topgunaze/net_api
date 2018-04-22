#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define START 20000
#define END   20200
void child_hander(int sig)
{
	int ret;
	while(1){
		ret = waitpid(-1, NULL, WNOHANG);
		if(ret <= 0)
			break;
	}
}
int
main(void)
{
	int i, j;
	int flag;
	pid_t pid;
	struct sigaction act, oact;
	act.sa_handler = child_hander;
	sigemptyset(&act.sa_mask);
	//act.sa_flags = SA_NOCLDWAIT;
	act.sa_flags = 0;

	sigaction(SIGCHLD, &act, &oact);
	for(j=START; j<=END; j++){
		pid = fork();
		if(pid < 0){
			perror("fork()");
			exit(-1);
		}
		if(pid == 0){
			flag = 1;
			for(i=2; i<j; i++){
				if(j % i == 0){
					flag = 0;
					break;
				}
			}
			if(flag == 1)
				printf("%d\n", j);
			exit(0);
		}
		else{
			//parent
		}
		
	}
	sigaction(SIGCHLD, &oact, NULL);
	while(1){
	}
	return 0;
}
