#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(void)
{
	pid_t pid;
	int ret;
	int pfd[2];
	char buf[256];
	ret = pipe(pfd);
	if(ret < 0){
		perror("pipe()");
		exit(-1);
	}
	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(-1);
	}
	if(pid == 0){
		//child
		close(pfd[1]);
		close(pfd[0]);
		exit(0);
		ret = read(pfd[0], buf, sizeof(buf));
		printf("ret = %d\n", ret);

		if(ret < 0){
			perror("read()");
			exit(-1);
		}

	  write(1, buf, ret);
		exit(0);
	}
	//parent
	close(pfd[0]);
	sleep(1);
	printf("test close read pfd\n");
	ret = write(pfd[1], "uplooking\n", 10);
	printf("ret = %d\n", ret);
	printf("test close read pfd\n");
	//close(pfd[1]);
 	wait(NULL);
	return 0;
}
