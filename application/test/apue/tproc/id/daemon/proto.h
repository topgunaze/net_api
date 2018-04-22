#ifndef PROTO_H
#define PROTO_H
#include <stdio.h>
#include <stdint.h>

struct stu{
	uint32_t id;
	char name[20];
	uint32_t math;
}__attribute__((packed));
#define RECVPORT  0x1990

#endif
