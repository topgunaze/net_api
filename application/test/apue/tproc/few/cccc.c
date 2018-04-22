#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// p  
// c0             c1 1 c2 1 c3 1
// c0c0    c0c1 c0c2
// ......

// p    0
// c    4
// cc   3 + 2 + 1
// ccc  2 + 1
// cccc 1
// cccc 1

int
main(void)
{
	pid_t pid;
	pid = fork();
	if(pid == 0){
		printf("[%d] : hello_0\n", getpid());
	}
	pid = fork();
	if(pid == 0){
		printf("[%d] : hello_1\n", getpid());
	}
	pid = fork();
	if(pid == 0){
		printf("[%d] : hello_2\n", getpid());
	}
	pid = fork();
	if(pid == 0){
		printf("[%d] : hello_3\n", getpid());
	}
	return 0;
}
