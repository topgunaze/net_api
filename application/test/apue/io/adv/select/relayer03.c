#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>

#define TTY0 "/dev/tty10"
#define TTY1 "/dev/tty11"
//#define STATE_R   1
//#define STATE_W   2
enum {STATE_R = 100, STATE_W, STATE_E, STATE_T};
struct relay_fm {
	int  state;
	int  fsrc, fdest;
	char buf[256];
	int  pos, len;
	char * err;
};
int relayer_element(struct relay_fm * fm)
{
	int r;
	switch(fm->state){
		case STATE_R:	
			r = read(fm->fsrc, fm->buf, sizeof(fm->buf));
			if(r < 0){
				if(errno == EAGAIN){
				}
				else{
					fm->state = STATE_E;
					fm->err = "read()";
				}
			}
			else if(r == 0){
				fm->state = STATE_T;
			}
			else{
				fm->state = STATE_W;
				fm->len = r;
				fm->pos = 0;
			}
			
			break;
		case STATE_W:	
			r = write(fm->fdest, fm->buf+fm->pos, fm->len);
			if(r < 0){
				fm->state = STATE_E;
				fm->err = "write()";
			}
			else{
				fm->len -= r;
				fm->pos += r;
				
			}
			if(fm->len <= 0)
				fm->state = STATE_R;
			else{
				
			}
			break;
			case STATE_E:
				perror(fm->err);
				fm->state = STATE_T;
			break;
			case STATE_T:
			
			break;
	}
}
int relayer(int fd0, int fd1)
{
	int fl0, fl1, t;
	int len, ret, mfd;
	int buf[256];
	struct relay_fm fm01, fm10;
	fd_set rfds, wfds;
	fl0 = fcntl(fd0, F_GETFL);
	t = fl0|O_NONBLOCK;
	fcntl(fd0, F_SETFL, t);
	fl1 = fcntl(fd1, F_GETFL);
	t = fl1|O_NONBLOCK;
	fcntl(fd1, F_SETFL, t);
	
	fm01.state = STATE_R;
	fm01.fsrc = fd0;
	fm01.fdest = fd1;

	fm10.state = STATE_R;
	fm10.fsrc = fd1;
	fm10.fdest = fd0;
	while(1){
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		if(fm01.state == STATE_R)
			FD_SET(fd0, &rfds);
		if(fm01.state == STATE_W)
			FD_SET(fd1, &wfds);
		if(fm10.state == STATE_R)
			FD_SET(fd1, &rfds);
		if(fm10.state == STATE_W)
			FD_SET(fd0, &wfds);
		mfd = fd0 > fd1 ? fd0 : fd1;
		ret = select(mfd+1, &rfds, &wfds, NULL, NULL);
		if(ret < 0){
			perror("select()");
			return -1;
		}
		if(FD_ISSET(fd0, &rfds) || FD_ISSET(fd1, &wfds))
			relayer_element(&fm01);
		if(FD_ISSET(fd1, &rfds) || FD_ISSET(fd0, &wfds))
			relayer_element(&fm10);
		if(fm01.state == STATE_T || \
			fm10.state == STATE_T)
			break;
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
