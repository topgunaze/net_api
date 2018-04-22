#include <stdio.h>
extern char ** environ;
//environ =={'4byte', '4byet', .... '0x00000000'};
//4byte == "hello world"
int
main(void)
{
	int i;
	for(i=0; ;i++){ 
		if(environ[i] == NULL)
			break;
		printf("%s\n", environ[i]); 
	}
	return 0;
}
