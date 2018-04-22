#include <stdio.h>
int flen(FILE * fp)
{
	int ret, len, pos;
	pos = ftell(fp);
	ret = fseek(fp, 0, SEEK_END);
	if(ret < 0)
		return -1;
	len = ftell(fp);
	ret = fseek(fp, pos, SEEK_SET);
	if(ret < 0)
		return -1;
	return len;
}
int
main(int argc, char * argv[])
{
	FILE * fp;
	int len;
	fp = fopen(argv[1], "r");
	if(fp == NULL){
		fprintf(stderr, "fopen() error\n");
		return -1;
	}

	len = flen(fp);
	printf("len = %d\n", len);
	fclose(fp);
	return 0;
}

