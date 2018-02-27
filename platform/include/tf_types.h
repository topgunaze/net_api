/**************************************************************
 * �ļ�����:  tf_types.h
 * ��           ��:  keith.gong
 * ��           ��:  2014.12.20
 * �ļ�����:  ����ͨ���������Ͷ���
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/

#ifndef __TF_TYPES_H__
#define __TF_TYPES_H__

#include <unistd.h>
#include <stdlib.h>

#ifndef ERROR
#define ERROR                           (-1)
#endif

#ifndef OK
#define OK                              0
#endif

typedef enum
{
    RTN_ERROR                           = ERROR,
    RTN_OK                              = OK,
}RTN_STATUS;
#define RTN_ERR                     RTN_ERROR
#define	STDIN                           0
#define	STDOUT                          1
#define	STDERR                          2

#ifndef BOOL
typedef unsigned char                   BOOL;
#endif

#ifndef FALSE
#define FALSE                           0
#endif

#ifndef TRUE
#define TRUE                            1
#endif



typedef unsigned int               	UINT;
typedef char                        INT8;
typedef short                       INT16;
typedef int                         INT32;
typedef long long                   INT64;

typedef unsigned char                   UINT8;
typedef unsigned short                  UINT16;
typedef unsigned int                    UINT32;
typedef unsigned long long              UINT64;

#ifndef MIN
//#define MIN(a, b)                       ((a) > (b) ? b : a)
#endif

#ifndef DEF_FUNC
#define DEF_FUNC(x) 0x01
#endif
#ifndef TF_BIT_SET
#define TF_BIT_SET(val, bit, set)	\
	do \
	{ \
		if((set) == 1) \
			(val) |= (1<<(bit)); \
		else if((set) == 0) \
			(val) &= ~(1<<(bit)); \
	}while(0);
#endif

#ifndef TF_BIT_TEST
#define TF_BIT_TEST(val, bit) (((val) >> (bit)) & 0x01U)
#endif


#endif

