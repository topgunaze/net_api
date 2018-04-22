#include <stdio.h>

int
main(void)
{
	char s[256];
	sprintf(s, "%d", 1000);
	printf("%s", s);
	//s[0] == '1'
	//s[1] == '0'
	//s[1] == '0'
	//s[1] == '0'
	//s[1] == '\0'

	return 0;
}
