#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>


char * src, * dest, * srcip, * destip;

struct arp {
  uint8_t dest[6];
  uint8_t src[6];
  uint16_t  type;

	uint16_t  ht;
	uint16_t  pt;
	uint8_t   hs;
	uint8_t   ps;
	uint16_t  op;
	uint8_t saddr[6];
	uint8_t sip[4];
	uint8_t daddr[6];
	uint8_t dip[4];
	uint8_t var[18];
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
int set_arp(struct arp * inet)
{
	str2mac(dest, inet->dest);
	str2mac(src, inet->src);
	inet->type = htons(0x0806);
	inet->ht = htons(0x0001);
	inet->pt = htons(0x0800);
	inet->hs = 6;
	inet->ps = 4;
	inet->op = htons(0x0001);
	str2mac(src,inet->saddr);
	inet_pton(PF_INET, srcip, inet->sip);
	str2mac("00:00:00:00:00:00:", inet->daddr);
	inet_pton(PF_INET, destip, inet->dip);
	memset(inet->var, '\0', sizeof(inet->var));
}
//./arp destmac destip srcmac srcip
//./arp ff:ff:ff:ff:ff:ff 192.168.11.216 84:2B:2B:A6:85:BF 192.168.11.254

int
main(int argc, char * argv[])
{
	int sfd, ret;
	struct arp inet;
	struct sockaddr sa;
	socklen_t len;
	int i;
	char buf[1024];	
#if 0
	dest = argv[1];
	src  = argv[3];
	destip = argv[2];
	srcip  = argv[4];
	set_arp(&inet);
#endif
  sfd = socket(PF_INET, SOCK_RAW, 1);
  //sfd = socket(PF_INET, SOCK_RAW, 3);
	if(sfd < 0){
		perror("socket()");
		exit(-1);
	}
	sa.sa_family = AF_INET;
	strcpy(sa.sa_data, "eth0");
	while(1){
	ret = recvfrom(sfd, buf, sizeof(buf), 0, &sa, &len);
	if(ret < 0){
		perror("recvfrom()");
		exit(-1);
	}
	
	for(i=0; i<ret; i++){
		printf("%02hhx ", buf[i]);
	}
	printf("\n");
	}
	close(sfd);
	return 0;
}
