#include <stdio.h>
#include <setjmp.h>

static jmp_buf save;

void fun(void)
{
	int i;
LOOP:
	for(i=0; i<5; i++){
	}
	return ;
}
int A(void)
{
	longjmp(save, 100);
	printf("A fun has run\n");
}
int
main(void)
{
	int ret;
	//goto LOOP;
	
	ret = setjmp(save);
	if(ret == 0){
		A();
	}
	else{
		printf("jmp form A ret = %d\n", ret);
	}

	return 0;
}
