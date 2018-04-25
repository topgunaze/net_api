#include "stdio.h"
#include "stdlib.h"
#include <setjmp.h>
#include <unistd.h>
#include <sys/types.h>

#include "tproc.h"

// "环境变量"
extern char ** environ;
//environ =={'4byte', '4byet', .... '0x00000000'};
//4byte == "hello world"
void
env_print(void)
{
    int i;
    
	for(i = 0; ; i++)
	{ 
		if(environ[i] == NULL)
			break;
			
		printf("%s\n", environ[i]); 
	}
	
	return;
}

int g_data = 1;

void
address_print(void)
{
    int         stack_i = 10;
    int         *p_heap = malloc(sizeof(int));
    static int  static_i = 10;
    char *      p_str = "hello world";
    
    unsigned int addr[6] = {(unsigned int)&stack_i, 
                            (unsigned int)p_heap, 
                            (unsigned int)&g_data, 
                            (unsigned int)&static_i, 
                            (unsigned int)p_str, 
                            (unsigned int)address_print};

    int i;
    for(i = 0; i < 6; ++i)
        printf("addr:%u\r\n", addr[i]);

    return ;
}

// "非局部跳转"

static jmp_buf save;

void goto_test(void)
{
	int i;
	
	for(i=0; i<5; i++)
	{   
        if (i == 3)
            goto LOOP;
	}

LOOP:
    
	return ;
}

static int jmp2pre_context(void)
{
	longjmp(save, 100);
	printf("A fun has run\n");
}

int
jmp_test(void)
{
	int ret;

	//jmp2pre_context();
	
	ret = setjmp(save);
	if(ret == 0)
	{
		jmp2pre_context();
	}
	else
	{
		printf("jmp form A ret = %d\n", ret);
		//jmp2pre_context();
	}

	return 0;
}

//exit & atexit
void exit_test()
{
    printf("uplooking");
	
	exit(0);
	
	while(1)
	{
	}
	
	return;
}

void fun_01(void)
{
	printf("test atexit()_)01\n");
}
void fun_02(void)
{
	printf("test atexit()_)02\n");
}

int
atexit_test(void)
{
	atexit(fun_01);
	atexit(fun_02);
	
	//_exit(0);
	exit(0);
	
	//printf("atexit has return \n");
	//while(1){}
	return 0;
}

void
getuid_euid_test(void)
{
	printf("uid  = %d\n", getuid());
	printf("euid = %d\n", geteuid());

	return ;
}



