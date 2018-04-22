#include <stdio.h>
#include <stdlib.h>
#define MSIZE 10
#define DSIZE 25

int mgetline_01(char ** s, size_t *n, FILE * stream)
{
	char * q;
	int i;
	i = 0;
	//q = malloc(10);
	q = realloc(NULL, 10);
	q = realloc(q, 20);
	q = realloc(q, 30);
	*s = q;
	*n = 30;
	return DSIZE;
}
int mgetline_02(char ** s, size_t *n, FILE * stream)
{
	char * q;
	int i, c;
	q = *s;
	i = 0;
	for(;;){
		c = fgetc(stream);
		if(c == EOF)
			break;
		if(i+1 > *n){
			*n += MSIZE;
			q = realloc(q, *n);
		}
		else{
		}
		q[i] = c;
		i++;
		if(c == '\n')
			break;
	}
	q[i+1] = '\0';
	*s = q;
	if(i == 0)
		return -1;
	return i;
}
int
main(void)
{
	char * p = NULL;
	size_t n = 0;
	FILE *fp ;
	int ret;
	fp = fopen("t.txt", "r");
	if(fp == NULL){
		perror("fopen()");
		return -1;
	}
	while(1){
		ret = mgetline_02(&p, &n, fp);
		printf("ret = %d\n", ret);
		printf("%s", p);
		if(ret < 0)
			break;
	}
	fclose(fp);
	return 0;
}
