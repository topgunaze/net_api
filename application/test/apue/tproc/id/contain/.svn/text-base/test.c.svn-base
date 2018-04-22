#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	int ret;
	ret = unlink("/etc/xxx");
	if(ret < 0){
		perror("unlink()");
		return -1;
	}
	return 0;
}
