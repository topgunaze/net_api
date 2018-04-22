#include <stdio.h>

int
main(void)
{
	FILE * fp;
	int c;
	fp = fopen("t.txt", "a+");
	if(fp == NULL){
		perror("fopen()");
		return -1;
	}
	//c = fgetc(fp);
	//putchar(c);
	fputc('w', fp);
	fclose(fp);
	return 0;
}
