#include <stdio.h>
#include <poll.h>

int
main(void)
{
	struct pollfd pfd[1];
	int ret;
	pfd[0].fd = 0;
	pfd[0].events = POLLIN;
	ret = poll(pfd, 1, 2000);
	printf("ret = %d\n", ret);
	if(pfd[0].revents & POLLIN)
		printf("0 is ready for read\n");
	else
		printf("0 is not ready for read\n");

	return 0;
}
