#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(void)
{
	char buf[256];
	int ret, len, pos;
	int pfd[2];
	pid_t pid;
	ret = pipe(pfd);
	if(ret < 0){
	}
	pid = fork();
	if(pid < 0){
	}
	if(pid == 0){
		//child
		close(pfd[1]);
		while(1){
			len = read(pfd[0], buf, sizeof(buf));
			if(len <= 0){
				break;	
			}
			write(1, buf, len);
		}
		exit(0);
	}
	//parent
	close(pfd[0]);
	while(1){
		len = read(0, buf, sizeof(buf));
		if(len <= 0)
			break;
		pos = 0;
		while(len > 0){
			ret = write(pfd[1], buf+pos, len); 
			pos += ret;
			len -= ret;
		}
	}	
	//*************
  close(pfd[1]);
	//***********/
	waitpid(-1, NULL, 0);
	return 0;
}

