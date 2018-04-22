#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int
main(int argc, char * argv[])
{
	int semid;
	key_t key;
	int i, ret;
	key = IPC_PRIVATE;

	semid = semget(key, 8, IPC_CREAT|0633);
	if(semid < 0){
		perror("semget()");
		exit(-1);
	}
	for(i=0; i<8; i++){
		semctl(semid, i, SETVAL, 100);
	}
	for(i=0; i<8; i++){
		ret = semctl(semid, i, GETVAL);
		printf("%d\n", ret);
	}
	struct sembuf op[1];
	op[0].sem_num = 0;
	op[0].sem_op = -200;
	op[0].sem_flg = 0;
	semop(semid, op, 1);
	for(i=0; i<8; i++){
		ret = semctl(semid, i, GETVAL);
		printf("%d\n", ret);
	}
	semctl(semid, 4, IPC_RMID);
	return 0;
}
