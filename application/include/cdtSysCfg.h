/*
 *
 * panger123   
 *
 * 2014/02/27 - ?
 *
 */
#ifndef __SYS_CFG_H__
#define __SYS_CFG_H__

#include <types.h>

//#define XINDE_VERSION

#define SYS_VERSION				"2.1.13"

#ifndef XINDE_VERSION
#define SYS_VERSION			SYS_VERSION
#else
#define VERSION_PULS			"s"
#define SYS_VERSION			SYS_VERSION VERSION_PULS
#endif

enum
{
    SYSTEM_CFG_RESTORING      = 1,
    SYSTEM_CFG_RESTORE_OK     = 2,
    SYSTEM_CFG_RESTORE_ERR    = 3,
    SYSTEM_CFG_RESTORE_WAIT   = 4
};

#define L2_CFG_FILE         "l2.cfg"
#define IGMP_CFG_FILE       "igmp.cfg"
#define DHCP_CFG_FILE      "dhcp.cfg"
#define RSTP_CFG_FILE       "rstp.cfg"
#define TRUNK_CFG_FILE      "trunk.cfg"
#define GCFG_FILE       "gtf.cfg"
#define SYSCTRL_CFG_FILE    "sysctrl.cfg"

/******************************* Sys Base Info *************************************/
#ifdef 1U4TF
#define OLT_MODEL		    0x01070000
#else
#define OLT_MODEL		    0x01060000	
#endif

#ifdef 1U4TF
#define CORP_TS_S 				"C-Data FD1104"
#else
#define CORP_TS_S 				"C-Data FD1108"
#endif
#define CORP_TS_L 				"C-Data Co.,Ltd."

#ifdef 1U4TF
#define OLT_DEF_DESC		"CDATA FD1104 OLT"
#else
#define OLT_DEF_DESC		"CDATA FD1108 OLT"
#endif
#define MAX_DESC_LEN		256

#define OLT_DEF_LOCATION	"Flat 6, Bldg 4,South 2 of Honghualing Industrial Zone, Liuxian Road, Xili Town, Shenzhen, Guangdong, China(518055)"
#define MAX_LOCATION_LEN	512

#define OLT_DEF_CONTACT		"marketing@cdatatec.com"
#define MAX_CONTACT_LEN		256

#define MAX_ALARM_DESC_LEN  128

#define DEF_FTP_IP 			0xC0A80165
#define DEF_FTP_USER		"admin"
#define DEF_FTP_PASS		"admin"
#ifdef 1U4TF
#define DEF_FTP_FILE		"FD1104S.image.gz"
#else
#define DEF_FTP_FILE		"FD1108S.image.gz"
#endif

#ifdef 1U4TF
/***************/
/**  FD1104S  **/
/***************/
/******************************* Chassis Info *************************************/
#define CHASSIC_TYPE 		0x01070101


/******************************* Card Module *************************************/
#define MAIN_CARN_TYPE 		0x01070201

/******************************* Card Module *************************************/
#define CARD_TYPE 		0x01070301

#else
/***************/
/**  FD1108S  **/
/***************/
/******************************* Chassis Info *************************************/
#define CHASSIC_TYPE 		0x01060101


/******************************* Card Module *************************************/
#define MAIN_CARN_TYPE 		0x01060201

/******************************* Card Module *************************************/
#define CARD_TYPE 		0x01060302

#endif
typedef struct sysInfo_t
{
	UINT8										mac[6];
	UINT8										hardware[20];
	UINT8										serialNum[20];
}sysInfo_t;

typedef struct cliUser_t
{
	UINT8										cliUserName[20];
	UINT8										cliUserPassword[20];
}cliUser_t;

typedef struct tfSys_t
{
	UINT8										console;/*1--enable to output, 0--disable*/
	cliUser_t									cliUser;
	UINT8										sysDesc[MAX_DESC_LEN];
	UINT8										sysLocation[MAX_LOCATION_LEN];
	UINT8										sysContact[MAX_CONTACT_LEN];
}tfSys_t;

tfSys_t	sysCfg;

#endif
