#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// cmd
// ps -eo pid,comm,ppid
int
main(void)
{
	char * arg[] = {
		"ps", "-eo",
		"pid,comm,ppid",
		NULL,
	};
	int ret;

	ret = execvp("ps", arg);	
	if(ret < 0)
	{
		perror("execvp()");
		return -1;
	}
	return 0;
}
