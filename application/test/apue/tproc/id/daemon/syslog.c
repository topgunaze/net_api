#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <syslog.h>

int
main(int argc, char * argv[])
{
	unsigned long long count = 0;
	openlog("hello world syslog:", LOG_CONS|LOG_NDELAY, LOG_USER);
	while(1){
		syslog(LOG_INFO, "count is %llu\n", count);
		count++;
		sleep(1);
	}
	closelog();
	return 0;
}
