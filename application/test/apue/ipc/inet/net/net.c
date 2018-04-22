#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/socket.h>

struct test {
	char a;
	int b;
}__attribute__((packed));
struct test x;


char * src, * dest;

struct inet{
	uint8_t dest[6];
	uint8_t src[6];
	uint16_t  type;
	uint8_t data[46];
}__attribute__((packed));

//84:2B:2B:A6:85:BF
int a2d(int c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return -1;
}
void str2mac(char * s, char * buf)
{
	int i;
	for(i=0; i<6; i++){
		buf[i] = a2d(s[i*3])*16 + a2d(s[i*3+1]);
		printf("%02hhX:", buf[i]);
	}
	printf("\n");
}
int set_inet(struct inet * inet)
{
	str2mac(dest, inet->dest);
	str2mac(src, inet->src);
	inet->type = htons(0x0001);
	memset(inet->data, '\0', sizeof(inet->data));
}
//./net destmac srcmac 
int
main(int argc, char * argv[])
{
	//printf("%d\n", sizeof(x));
	int sfd, ret;
	struct inet inet;
	struct sockaddr sa;
	
	dest = argv[1];
	src  = argv[2];
	set_inet(&inet);
  sfd = socket(PF_INET, SOCK_PACKET, 0);
	if(sfd < 0){
		perror("socket()");
		exit(-1);
	}
	sa.sa_family = AF_INET;
	strcpy(sa.sa_data, "eth0");
	ret = sendto(sfd, &inet, sizeof(inet), 0, &sa, sizeof(sa));
	if(ret < 0){
		perror("sendto()");
		exit(-1);
	}
	close(sfd);
	return 0;
}
