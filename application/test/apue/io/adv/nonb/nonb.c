#include <stdio.h>
#include <fcntl.h>
#define O_NONBLOCK_TEST 0x01

int
main(void)
{
	char buf[256];
	int fl;
	int ret;
	ret = read(0, buf, sizeof(buf));
	fl = fcntl(0, F_GETFL);
	printf("fl = %x\n", fl);
	//fl = fl | O_NONBLOCK_TEST;
	fl = fl | O_NONBLOCK;
	fcntl(0, F_SETFL, fl);
	printf("noblock io has run\n");
	while(1){ 
		ret = read(0, buf, sizeof(buf));
		if(ret < 0)
			continue;
		break;
	}
	printf("ret = %d\n", ret);
	return 0;
}
