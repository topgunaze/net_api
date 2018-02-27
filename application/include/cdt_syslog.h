/******************************************************************************
Copyright (C), 2014-2024, C-Data Tech. Co., Ltd.
  �� �� ��   : tf_syslog.h
  ��    ��   : jh.liang
  �� �� ��   : 1.0.0 
  ��������   : 2015��3��13��
  ��������   : IPCģ����ر������궨��
******************************************************************************/



#ifndef TF_SYSLOG_H
#define TF_SYSLOG_H

//#include <sys/syslog.h>
#include "tf_log.h"

struct tflog *
opentflog (const char *progname, int mods, int debugEn,int flags, int syslog_facility, int (*termOut)(char *));

#define log_print(level,exp, arg... )                                           \
    do {                                                                        \
        switch(level)                                                           \
        {                                                                       \
            case LOG_CRIT:                                                      \
                tflog_crit("[%s:%d] "exp, __FUNCTION__, __LINE__, ##arg);   \
            break;                                                              \
            case LOG_ERR:                                                       \
                tflog_err("[%s:%d] "exp, __FUNCTION__, __LINE__, ##arg);    \
            break;                                                              \
            case LOG_WARNING:                                                   \
                tflog_warn("[%s:%d] "exp, __FUNCTION__, __LINE__, ##arg);   \
            break;                                                              \
            case LOG_NOTICE:                                                    \
                tflog_notice("[%s:%d] "exp, __FUNCTION__, __LINE__, ##arg); \
            break;                                                              \
            case LOG_INFO:                                                      \
                tflog_info("[%s:%d] "exp, __FUNCTION__, __LINE__, ##arg);   \
            break;                                                              \
            case LOG_DEBUG:                                                     \
                tflog_debug(0, "[%s:%d] "exp, __FUNCTION__, __LINE__, ##arg);  \
            break;                                                              \
            default :                                                           \
                break;                                                          \
        }                                                                       \
    } while(0)       

 #define dump_pkt_print(exp, arg... )                                           \
    do {                                                                        \
                tflog_debug(0, exp, ##arg);  \
        } while(0)                 
#endif  //TF_SYSLOG_H


