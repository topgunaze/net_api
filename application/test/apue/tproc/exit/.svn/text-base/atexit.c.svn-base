#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void fun_01(void)
{
	printf("test atexit()_)01\n");
}
void fun_02(void)
{
	printf("test atexit()_)02\n");
}
int
main(void)
{
	void (* t)(void);
	atexit(fun_01);
	atexit(fun_02);
	_exit(0);
	//exit(0);
	//printf("atexit has return \n");
	//while(1){}
	return 0;
}
