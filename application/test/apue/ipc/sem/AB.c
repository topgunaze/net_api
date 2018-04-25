#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
int  semdig;
void P(int semid)
{
      struct sembuf op;
      op.sem_num = 0;
      op.sem_op = -1;
      op.sem_flg = SEM_UNDO;
      semop(semid, &op, 1);
}
void V(int semid)
{
      struct sembuf op;
      op.sem_num = 0;
      op.sem_op = 1;
      op.sem_flg = SEM_UNDO;
      semop(semid, &op, 1);
}
int
main(int argc, char * argv[])
{
	pid_t pid;
	int semid;
	semdig = semget(IPC_PRIVATE, 1, IPC_CREAT|0655);
	if(semdig < 0){
		perror("semget()");
		exit(-1);
	}
	semid = semget(IPC_PRIVATE, 1, IPC_CREAT|0633);
	if(semid < 0){
		perror("semget()");
		exit(-1);
	}
	semctl(semid, 0, SETVAL, 1);
	semctl(semdig, 0, SETVAL, 1);
	pid = fork();
	if(pid == 0){
		while(1){
			P(semid);
			while(semctl(semdig, 0, GETVAL) == 1){
				V(semid);
				usleep(1);
				P(semid);
			}
			write(1, "B", 1);
			semctl(semdig, 0, SETVAL, 1);
			V(semid);
		}
		exit(0);
	}
		while(1){
			P(semid);
			while(semctl(semdig, 0, GETVAL) == 0){
				V(semid);
				usleep(1);
				P(semid);
			}
			write(1, "A", 1);
			semctl(semdig, 0, SETVAL, 0);
			V(semid);
		}
	semctl(semid, 0, IPC_RMID);
	semctl(semdig, 0, IPC_RMID);
	return 0;
}