#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define START 20000
#define END   20200
void P(int semid)
{
	struct sembuf op;
	int ret;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = IPC_NOWAIT;
	while(1){
		ret = semop(semid, &op, 1);
		if(ret < 0){
			if(errno == EAGAIN)
				continue;
			else if(errno == EINTR)
				continue;
			else{
				perror("semop()");
				exit(-1);
			}
		}
		break;
	}
}
void V(int semid)
{
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}
int
main(void)
{
	int i, j;
	int flag;
	pid_t pid;
	int semid;	
	struct sembuf op[1];
	
	semid = semget(IPC_PRIVATE, 1, IPC_CREAT|0644);
	if(semid < 0){
		perror("semget()");
		exit(-1);
	}
	semctl(semid, 0, SETVAL, 1);
	for(j=START; j<=END; j++){
		P(semid);
		pid = fork();
		if(pid < 0){
			perror("fork()");
			exit(-1);
		}
		if(pid == 0){
			usleep(200);
			flag = 1;
			for(i=2; i<j; i++){
				if(j % i == 0){
					flag = 0;
					break;
				}
			}
			if(flag == 1)
				printf("%d\n", j);
			V(semid);
			exit(0);
		}
		else{
			//parent
		}
	}
	for(i=START; i<=END; i++)
		wait(NULL);
	semctl(semid, 0, IPC_RMID);
	return 0;
}
