#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define START 20000
#define END   20200

int
main(void)
{
      int i, j;
      int flag;
      pid_t pid;
      for(j=START; j<=END; j++){
	    pid = fork();
	    if(pid < 0){
		  perror("fork()");
		  exit(-1);
	    }
	    if(pid == 0){
		  flag = 1;
		  for(i=2; i<j; i++){
			if(j % i == 0){
			      flag = 0;
			      break;
			}
		  }
		  if(flag == 1)
			printf("%d\n", j);
		  exit(0);
	    }
	    else{
		  //parent
	    }
		
      }
      return 0;
}
