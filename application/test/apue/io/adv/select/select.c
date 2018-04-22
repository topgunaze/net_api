#include <stdio.h>
#include <sys/select.h>

int
main(void)
{
	char buf[256];
	int len, ret;
	struct timeval tm;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(0, &readfds);
	if(FD_ISSET(0, &readfds))
		printf("readfds op success\n");
	else
		printf("op failed\n");
	
	//FD_ZERO(&readfds);
	while(1){
		tm.tv_sec = 2;
		tm.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);
		ret = select(1, &readfds, NULL, NULL, NULL);
		printf("ret = %d\n", ret);
		if(FD_ISSET(0, &readfds))
			printf("0 is in readfds\n");
		else
			printf("0 is not in readfds\n");
		len = read(0, buf, sizeof(buf));
	}
	return 0;
}
