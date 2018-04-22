#include <stdio.h>

int
main(void)
{
	FILE * fp;
	int len;
	char buf[256];
	fp = fopen("/tmp/1.cgi.jpeg", "r");
	printf("Content-type:image/jpg \r\n\r\n");
	while(1){
		len = fread(buf, 1, sizeof(buf), fp);
		if(len <= 0)
			break;
		fwrite(buf, 1, len, stdout);
	}	
	fclose(fp);
	return 0;
}
