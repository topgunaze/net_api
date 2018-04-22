#include <stdio.h>

int
main(int argc, char ** argv)
{
	FILE * fp;
	int c;
	if(argc < 2)
	{	
		fprintf(stderr, "usage ./mcat t.txt\n");
		return -1;
	}
	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		fprintf(stderr, "fopen() err ...\n");
		return -1;
	}
	while(1){
		c = fgetc(fp);
		if(c == EOF)
			break;
		printf("%c", c);
	}
	fclose(fp);
	return 0;
}
