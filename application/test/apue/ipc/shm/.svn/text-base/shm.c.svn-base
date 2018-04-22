#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <pthread.h>
int
main(int argc, char * argv[])
{
	int shmid;
	char * mem;
	pid_t pid;
	shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT|0622);
	if(shmid < 0){
		perror("shmget()");
		exit(-1);
	}
	
	pid = fork();
	if(pid == 0){
		sleep(1);
		mem = shmat(shmid, NULL, 0);
		if(mem == (void  *) -1){
			perror("shmat()");
			exit(-1);
		}
		printf("%s\n", mem);
		shmdt(mem);
		exit(0);
	}
	mem = shmat(shmid, NULL, 0);
	if(mem == (void  *) -1){
		perror("shmat()");
		exit(-1);
	}
	memcpy(mem, "uplooking", 10);
	shmdt(mem);
	wait(NULL);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
