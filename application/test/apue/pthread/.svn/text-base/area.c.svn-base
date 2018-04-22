#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>

#define  PI     3.1415926
#define  PARTS  1000000
double get_area(void)
{
	int i;
	double a, area;
	a = sin(2*PI/PARTS) * cos(2*PI/PARTS) / 2;
	area = 0.0;
	for(i=0; i<PARTS; i++)
		area += a;
	return area;
}
int 
main(int argc, char * argv[]) 
{ 
	printf("area = %f\n", get_area());
	return 0; 
}
