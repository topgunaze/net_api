#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


//ls -a -l\n
//ls -a > t.txt -l    \n
struct cmd {
	char * c;
	char * arg[10];
	int in, out;
};
//============
//int add(int a, int b)
//{
//	return a + b;
//}
//x = add(5, 6);
void get_cmd(char * s, struct cmd * cmd)
{
	int i;
	char * r;
	for(i=0; i<10; i++){
		cmd->arg[i] = NULL;
	}
	cmd->out = cmd->in = -1;
	for(i=0;;){
		r = strsep(&s, " ");
		if(r == NULL)
			break;
		if(r[0] == '\0')
			continue;
		if(i == 0){
			cmd->c = r;
			cmd->arg[0] = r;
		}
		else{
			if(r[0] == '>'){
				if(strlen(r) == 1)
				{
					r = strsep(&s, " ");
					cmd->out = open(r, O_WRONLY|O_TRUNC|O_CREAT, 0644);
				}
				else{
					cmd->out = open(r+1, O_WRONLY|O_TRUNC|O_CREAT, 0644);
					
				}		
				continue;
			}
			else if(r[0] == '<'){
				if(strlen(r) == 1)
				{
					r = strsep(&s, " ");
					cmd->out = open(r, O_RDONLY);
					if(cmd->out < 0){
						perror("open()");
						exit(-1);
					}
				}
				else{
					cmd->out = open(r+1, O_WRONLY|O_TRUNC|O_CREAT, 0644);
					
				}		
				continue;
			}
				
			cmd->arg[i] = r;
		}
		//printf("%s\n", r);
		i++;
	}
}
int 
main(void)
{
	pid_t pid;
	char buf[256];
	struct cmd cmd;
	int ret;	
	char * p;
	while(1){
		printf("%s", "[root@msh]# ");
		p = fgets(buf, sizeof(buf), stdin);
		if(p == NULL){
			perror("fgets()");
			return -1;
		}
		ret = strlen(buf);
		buf[ret-1] = '\0'; 
		//printf("%s", buf);
		get_cmd(buf, &cmd);
		pid = fork();
		if(pid < 0){
			perror("fork()");
			exit(-1);
		}
		if(pid == 0){
			if(cmd.out != -1){
				ret = dup2(cmd.out, 1);
				if(ret < 0){
					perror("dup2()");
					exit(-1);
				}
			}
			ret = execvp(cmd.c, cmd.arg);
			if(ret < 0){
				perror("execvp()");
				exit(-1);
			}			
		}
		wait(NULL);
	}
	return 0;
}
