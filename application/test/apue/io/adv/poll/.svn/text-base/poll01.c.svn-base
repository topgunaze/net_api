#include <stdio.h>
#include <poll.h>

int
main(void)
{
	struct pollfd pfd[1];
	int ret;
	pfd[0].fd = 1;
	pfd[0].events = POLLOUT;
	ret = poll(pfd, 1, 2000);
	printf("ret = %d\n", ret);
	if(pfd[0].revents & POLLOUT)
		printf("1 is ready for write\n");
	else
		printf("1 is not ready for write\n");

	return 0;
}
