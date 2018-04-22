#include <stdio.h>

int
main(void)
{
	FILE * fp;
	int pos, c;
	fp = fopen("t.txt", "r");
	if(fp == NULL){
		fprintf(stderr, "fopen() error\n");
		return -1;
	}
	pos = fseek(fp, 1, SEEK_SET);
	printf("pos = %d\n", pos);
	c = fgetc(fp);
	putchar(c);
	fclose(fp);
	return 0;
}
