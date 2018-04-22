#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int
main(int argc, char * argv[])
{
	struct stat buf;
	int ret, i;
	char am[] = {"rwxrwxrwx"};
	struct tm * tm;
	struct passwd * pw;
	struct group * gr;
	mode_t m;
	ret = stat(argv[1], &buf);
	m = buf.st_mode;
	if(ret < 0){
		perror("stat()");
		return -1;
	}
	if(S_ISREG(m))
		printf("-");
	else if(S_ISDIR(m))
		printf("d");
	else if(S_ISCHR(m))
		printf("c");
	else if(S_ISBLK(m))
		printf("b");
	else if(S_ISFIFO(m))
		printf("p");
	else if(S_ISLNK(m))
		printf("l");
	else if(S_ISSOCK(m))
		printf("s");
	else{
		printf("File format error\n");
	}
	for(i=0; i<9; i++){
		if((m&(1<<i)) == 0)
			am[8-i] = '-';
	}
	printf("%s", am);
	printf(" %d", buf.st_nlink); 
#if 1
	pw = getpwuid(buf.st_uid);
	printf(" %s", pw->pw_name);
	gr = getgrgid(buf.st_gid);
	printf(" %s", gr->gr_name);
#endif
	printf(" %d", buf.st_size);
	tm = gmtime(&buf.st_mtime);
	printf(" %02d-%02d %02d:%02d", \
		tm->tm_mon+1, \
		tm->tm_mday, (tm->tm_hour+8)%24, \
		tm->tm_min);
	printf(" %s", argv[1]);
	printf("\n");

	return 0;
}
