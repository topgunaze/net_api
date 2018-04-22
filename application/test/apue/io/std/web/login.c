#include <stdio.h>

int
main(void)
{
	char * s = NULL;
	size_t n = 0;
	getline(&s, &n, stdin);
	printf("Conten-type:text/html \r\n\r\n");
	printf("<h1>");
	printf("%s", s);
	printf("</h1>");
	return 0;
}
