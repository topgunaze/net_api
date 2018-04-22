#define _FILE_OFFSET_BITS 64
#include <stdio.h>
//#define MSIZE  (5LL*1024*1024*1024)
#define MSIZE  (1LL*1024*1024)
#define FNAME  "/tmp/bigf"
//off_t

int 
main(void)
{
	FILE * fp ;
	fp = fopen(FNAME, "w");
	if(fp == NULL){
		fprintf(stderr, "fopen() error\n");
		return -1;
	}
	fseeko(fp, MSIZE, SEEK_SET);
	
	fputc('a', fp);
	fclose(fp);
	return 0;
}
