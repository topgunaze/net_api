#include <stdio.h>

int
main(int argc, char ** argv)
{
	FILE * s, * d;
	int len;
	char buf[256];
	s = fopen(argv[1], "r");
	if(s == NULL){
		perror("fopen()");
		return -1;
	}
	d = fopen(argv[2], "w");
	if(d == NULL){
		perror("fopen()");
		return -1;
	}
	
	while(1){
		len = fread(buf, 1, sizeof(buf), s);
		if(len == 0)
			break;
		fwrite(buf, 1, len, d);
	}
	fclose(s);
	fclose(d);
	return 0;
}
