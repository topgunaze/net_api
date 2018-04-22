#include <stdio.h>
#include <dirent.h>
void travel_dir(char * path, int level)
{
	DIR * dir;
	char name[1024];
	int i;
	struct dirent * d;
	dir = opendir(path);
	if(dir == NULL)
		return ;
	while(1){
		d = readdir(dir);
		if(d == NULL)
			break;
		if(d->d_name[0] == '.')
			continue; 
		for(i=0; i<level; i++)
			printf("  ");
		printf("%s\n", d->d_name);
		sprintf(name, "%s/%s", path, d->d_name);
		travel_dir(name, level+1);
	}
}
int 
main(int argc, char * argv[])
{
	travel_dir(argv[1], 0);
	return 0;
}
