#include <stdio.h>
#include <unistd.h>
int
main(void)
{
	printf("uid  = %d\n", getuid());
	printf("euid = %d\n", geteuid());
	return 0;
}
