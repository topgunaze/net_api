#include <stdio.h>
#include <fcntl.h>
#define TTY0 "/dev/tty10"
#define TTY1 "/dev/tty11"
int relayer(int fd0, int fd1)
{
	int fl0, fl1, t;
	int len;
	int buf[256];
	fl0 = fcntl(fd0, F_GETFL);
	t = fl0|O_NONBLOCK;
	fcntl(fd0, F_SETFL, t);
	fl1 = fcntl(fd1, F_GETFL);
	t = fl1|O_NONBLOCK;
	fcntl(fd1, F_SETFL, t);
	while(1){
		len = read(fd0, buf, sizeof(buf));
		if(len < 0){
		}
		else if(len == 0)
			break;
		else{
			write(fd1, buf, len);
		}
		len = read(fd1, buf, sizeof(buf));
		if(len < 0){
		}
		else if(len == 0)
			break;
		else{
			write(fd0, buf, len);
		}
		
	}
	fcntl(fd0, F_SETFL, fl0);
	fcntl(fd1, F_SETFL, fl1);
	
}
int
main(void)
{
	int fd0, fd1;
	char buf[256];
	int len;
	fd0 = open(TTY0, O_RDWR);
	if(fd0 < 0){
		perror("open()");
		return -1;
	}
	fd1 = open(TTY1, O_RDWR);
	if(fd1 < 0){
		perror("open()");
		return -1;
	}
	write(fd0, "TTY0:\n", 6); 
	write(fd1, "TTY1:\n", 6); 
	relayer(fd0, fd1);
	close(fd0);
	close(fd1);
	return 0;
}
