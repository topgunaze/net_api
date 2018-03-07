/**************************************************************
 * �ļ�����:  gtfSyslog.h
 * ��           ��:  keith.gong
 * ��           ��:  2015.09.23
 * �ļ�����:  GTF ϵͳ��־
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/

#ifndef __GSYSLOG_H__
#define __GSYSLOG_H__


#include <stdio.h>
#include <time.h>
#include "log.h"


typedef enum
{
    GSYSLOG_MODULE_DRV,
    GSYSLOG_MODULE_HWAL,
    GSYSLOG_MODULE_OMCI,
    GSYSLOG_MODULE_ONUREG,
    GSYSLOG_MODULE_PROFILE,
    GSYSLOG_MODULE_ONUCFG,
    GSYSLOG_MODULE_TFCFG,
    GSYSLOG_MODULE_DEVCTRL,
    GSYSLOG_MODULE_SNMP,
    GSYSLOG_MODULE_ROGUE,
    
    GSYSLOG_MODULE_MAX_NUM,
}GSYSLOG_MODULE_ID_ST;

#if 1
#define DRV_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define DRV_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define DRV_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_DRV, "[DRV ] "fmt, ##arg)

#define HWAL_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define HWAL_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define HWAL_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_HWAL, "[HWAL] "fmt, ##arg)

#define ROGUE_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define ROGUE_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define ROGUE_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_ROGUE, "[ROGUE] "fmt, ##arg)


#define OMCI_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define OMCI_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define OMCI_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_OMCI, "[OMCI] "fmt, ##arg)

#define ONURA_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define ONURA_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define ONURA_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_ONUREG, "[RA ] "fmt, ##arg)

#define PROFILE_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define PROFILE_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define PROFILE_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_PROFILE, "[ROFILE] "fmt, ##arg)

#define ONUCFG_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define ONUCFG_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define ONUCFG_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_ONUCFG, "[OCFG] "fmt, ##arg)

#define TFCFG_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define TFCFG_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define TFCFG_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_TFCFG, "[PCFG] "fmt, ##arg)

#define TFDEVCTRL_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define TFDEVCTRL_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_DEVCTRL, "[DCTL] "fmt, ##arg)

#define SNMP_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define SNMP_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define SNMP_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_SNMP, "[SNMP] "fmt, ##arg)

#define ALARM_MY_PRINT(fmt,arg...) tflog_info(fmt, ##arg)
#define ALARM_MY_ERROR(fmt,arg...) tflog_crit(fmt, ##arg)
#define ALARM_MY_DEBUG(fmt,arg...) tflog_debug(GSYSLOG_MODULE_SNMP, "[ALARM] "fmt, ##arg)

#else
#if 111
#define DRV_MY_PRINT(fmt,arg...) printf(fmt"\r\n", ##arg)
#define DRV_MY_ERROR(fmt,arg...) printf(fmt"\r\n", ##arg)
#define DRV_MY_DEBUG(fmt,arg...) printf("[DRV ] "fmt"\r\n", ##arg)

#define HWAL_MY_PRINT(fmt,arg...) printf(fmt"\r\n", ##arg)
#define HWAL_MY_ERROR(fmt,arg...) printf(fmt"\r\n", ##arg)
#define HWAL_MY_DEBUG(fmt,arg...) printf("[HWAL] "fmt"\r\n", ##arg)

#define OMCI_MY_PRINT(fmt,arg...) printf(fmt"\r\n", ##arg)
#define OMCI_MY_ERROR(fmt,arg...) printf(fmt"\r\n", ##arg)
#define OMCI_MY_DEBUG(fmt,arg...) printf("[OMCI] "fmt"\r\n", ##arg)

#define ONURA_MY_PRINT(fmt,arg...) printf(fmt"\r\n", ##arg)
#define ONURA_MY_ERROR(fmt,arg...) printf(fmt"\r\n", ##arg)
#define ONURA_MY_DEBUG(fmt,arg...) printf("[RA ] "fmt"\r\n", ##arg)

#define PROFILE_MY_PRINT(fmt,arg...) printf(fmt"\r\n", ##arg)
#define PROFILE_MY_ERROR(fmt,arg...) printf(fmt"\r\n", ##arg)
#define PROFILE_MY_DEBUG(fmt,arg...) printf("[PROF] "fmt"\r\n", ##arg)

#define ONUCFG_MY_PRINT(fmt,arg...) printf(fmt"\r\n", ##arg)
#define ONUCFG_MY_ERROR(fmt,arg...) printf(fmt"\r\n", ##arg)
#define ONUCFG_MY_DEBUG(fmt,arg...) printf("[OCFG] "fmt"\r\n", ##arg)

#define TFCFG_MY_PRINT(fmt,arg...) printf(fmt"\r\n", ##arg)
#define TFCFG_MY_ERROR(fmt,arg...) printf(fmt"\r\n", ##arg)
#define TFCFG_MY_DEBUG(fmt,arg...) printf("[PCFG] "fmt"\r\n", ##arg)

#define TFDEVCTRL_MY_PRINT(fmt,arg...) printf(fmt"\r\n", ##arg)
#define MY_ERROR(fmt,arg...) printf(fmt"\r\n", ##arg)
#define TFDEVCTRL_MY_DEBUG(fmt,arg...) printf("[DCTL] "fmt"\r\n", ##arg)

#define SNMP_MY_PRINT(fmt,arg...) printf(fmt"\r\n", ##arg)
#define SNMP_MY_ERROR(fmt,arg...) printf(fmt"\r\n", ##arg)
#define SNMP_MY_DEBUG(fmt,arg...) printf("[SNMP] "fmt, ##arg)

#else

//#define DRV_MY_PRINT(fmt,arg...) tflog_err(fmt, ##arg)
//#define DRV_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
//#define DRV_MY_DEBUG(fmt,arg...) tflog_err(fmt, ##arg)

//#define HWAL_MY_PRINT(fmt,arg...) tflog_err(fmt, ##arg)
//#define HWAL_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
//#define HWAL_MY_DEBUG(fmt,arg...) tflog_err(fmt, ##arg)

#define OMCI_MY_PRINT(fmt,arg...) tflog_err(fmt, ##arg)
#define OMCI_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
#define OMCI_MY_DEBUG(fmt,arg...) tflog_err(fmt, ##arg)

//#define ONURA_MY_PRINT(fmt,arg...) tflog_err(fmt, ##arg)
//#define ONURA_MY_ERROR(fmt,arg...)tflog_err(fmt, ##arg)
//#define ONURA_MY_DEBUG(fmt,arg...)tflog_err(fmt, ##arg)

//#define PROFILE_MY_PRINT(fmt,arg...) tflog_err(fmt, ##arg)
//#define PROFILE_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
//#define PROFILE_MY_DEBUG(fmt,arg...) tflog_err(fmt, ##arg)

//#define ONUCFG_MY_PRINT(fmt,arg...) tflog_err(fmt, ##arg)
//#define ONUCFG_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
//#define ONUCFG_MY_DEBUG(fmt,arg...) tflog_err(fmt, ##arg)

//#define TFCFG_MY_PRINT(fmt,arg...) tflog_err(fmt, ##arg)
//#define TFCFG_MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
//#define TFCFG_MY_DEBUG(fmt,arg...) tflog_err(fmt, ##arg)


//#define TFDEVCTRL_MY_PRINT(fmt,arg...) tflog_err(fmt, ##arg)
//#define MY_ERROR(fmt,arg...) tflog_err(fmt, ##arg)
//#define TFDEVCTRL_MY_DEBUG(fmt,arg...) tflog_err(fmt, ##arg)

#endif
#endif

#endif /* __GSYSLOG_H__ */


