/*
 * panger123
 *
 * 2014/01-20 - ?
 *
 */ 
#ifndef __SYS_LOG__
#define __SYS_LOG__
#include "types.h"
#define LOG_FILE_SIZE	524288  /* 512KBytes*/

#define LOG_PAGE_LEN	16
#define LOG_LINE_LEN	256
#define LOG_FILE_SHORT_NAME "log.txt"
#define LOG_FILE_FULL_NAME  "/mnt/log.txt"

typedef struct tfSysLogLevel_t
{
	UINT8 onuOnOffLine;
	UINT8 alarmOnuDyingGasp;
	UINT8 alarmUniLoopBack;
}tfSysLogLevel_t;

typedef enum 
{
	LOG_CRIT=0,
	LOG_ALARM,
	LOG_NORMAL,	
	LOG_DEBUG,
}tfLogLevel;

tfSysLogLevel_t sysLogL;

void tfSyslog(tfLogLevel level, INT8 *String, ...);

#endif

