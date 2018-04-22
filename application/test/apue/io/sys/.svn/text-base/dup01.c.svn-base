#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int
main(void)
{	
	int ret, newfd;
	char buf[256];
	newfd = dup(0);
	ret = read(newfd, buf, sizeof(buf));
	buf[ret] = '\0';
	printf("%s", buf);
	return 0;
}
