#include <stdio.h>
#include <unistd.h>
int
main(void)
{
	int ret;
	
	ret = seteuid(500);
	if(ret < 0)
	{
		perror("seteuid()");
		return -1;
	}
	
	printf("uid  = %d\n", getuid());
	printf("euid = %d\n", geteuid());
	
	ret = seteuid(400);
	if(ret < 0)
	{
		perror("seteuid() == 400");
		return -1;
	}
	
	ret = unlink("/etc/ttt");
	if(ret < 0)
	{
		perror("unlink()");
		return -1;
	}
	
	return 0;
}
