#include <stdio.h>
#include <sys/select.h>

int
main(void)
{
	fd_set writefds;
	int ret;
	FD_ZERO(&writefds);
	FD_SET(1, &writefds);
	ret = select(2, NULL, &writefds, NULL, NULL);
	printf("ret = %d\n", ret);
	return 0;
}
