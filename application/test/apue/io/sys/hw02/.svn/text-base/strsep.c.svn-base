#include <stdio.h>
#include <string.h>

char * mstrsep(char ** s, char * delim)
{
	char * p = *s;
	int i;
	int flag = 0;
	for(i=0; p[i] != '\0'; i++){
		if(p[i] == *delim){
			p[i] = '\0';
			*s = &p[i]+1;
			//flag = 1;
			return p;
		}			
	} 
	if(i != 0){
		*s = p + i;
		return p;
	}
	else{
		*s = NULL;
		return NULL;
	}
}
int
main(void)
{
	char s[] = {"ls       -l -a"};
	//char s[] = {"ls  -l"};

	char * p, *r;
	int i;
	p = s;
	while(1){	
		r = mstrsep(&p, " ");
		if(r == NULL)
			break;
		printf("r = %s\n", r);
	}	
	for(i=0; i<sizeof(s); i++)
		printf("%c", s[i]);
	return 0;
}
