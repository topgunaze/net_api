/**************************************************************
 * �ļ�����:  tfSysCtrlPub.h
 * ��           ��:  steven.tian
 * ��           ��:  2015.08.20
 * �ļ�����:  system configuration interface public header
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/
#ifndef _TF_SYS_CTRL_PUB_H
#define _TF_SYS_CTRL_PUB_H

#include <stdint.h>

#define SYSTEM_CONF_PATH            "/mnt/cfg/"

#define TF_SYSTEM_CFG_IP_TYPE_INIT    0
#define TF_SYSTEM_CFG_IP_TYPE_STATIC  1
#define TF_SYSTEM_CFG_IP_TYPE_DYNAMIC 2

#define TF_SYSTEM_CFG_MGMG_IP_DEFAULT          "192.168.1.100"
#define TF_SYSTEM_CFG_MGMG_IP_NETMASK_DEFAULT  "255.255.255.0"

#define IF_NAME_STR_LEN 16
#define TF_SYSTEM_CFG_VLAN_IF_NUM  32
#define TF_IP_ROUTE_STATIC_MAX_NUM   32
#define TF_SYS_LOG_DESC_STR_LEN    32
#define TF_SYS_LOG_HOST_NUM        1

#define IF_DESC_STR_LEN 128

#define STR_LEN(len) ((len) + 1)

enum
{
    TF_IP_ROUTE_STATIC_FLAG_VALID  = (1 << 0),
    TF_IP_ROUTE_STATIC_DEFAULT     = (1 << 1)
};

enum
{
    TF_IP_ROUTE_PROTO_STATIC   = 0,
    TF_IP_ROUTE_PROTO_DIRECT   = 1,
    TF_IP_ROUTE_PROTO_UNKNOWN
};

enum
{
    TF_SYS_CTRL_NO_ERR = 0,
    TF_SYS_CTRL_ERR_EXCEED_LIMIT = -1,
    TF_SYS_CTRL_GEN_ERR = -2,
    TF_SYS_CTRL_PARAM_ERR = -3,
    TF_SYS_CTRL_EXIST_ERR = -4,
};

enum
{
    TF_SYS_LOG_HOST_INACTIVE = 0,
    TF_SYS_LOG_HOST_ACTIVE   = 1,
};

enum
{
    SYSCTRL_DEBUG_GEN,
    SYSCTRL_DEBUG_HW_MONITOR,
    
    SYSCTRL_DEBUG_MOD_NUMS
};

enum
{
    TF_FILE_TYPE_TAR = 0,
    TF_FILE_TYPE_TXT,
    TF_FILE_TYPE_GZ,
    TF_FILE_TYPE_BZIP,
};

enum
{
    SYS_REBOOT_NONE,
    SYS_REBOOT_DISABLE,
    SYS_REBOOT_ENABLE,
    SYS_REBOOT_DELAY,
    SYS_REBOOT_NORMAL,
    SYS_REBOOT_FORCE,
};

enum
{
    SYS_OPER_STATUS_TYPE_INTERACTION = 0,
    
};

typedef struct
{
    unsigned char   type;    /* static or dhcp */
    unsigned long   ipaddr;  /* network address */
    unsigned long   netmask; /* subnet mask */
}tfIpAddr_t;

typedef struct
{
    tfIpAddr_t     ip;
    unsigned short  mvlan;
    unsigned char type;		/*0-static, 1-dynamic*/
    unsigned int  leaseTime;
}tfVlanIp_t;

typedef struct
{
    char         name[IF_NAME_STR_LEN];
    char         desc[STR_LEN(IF_DESC_STR_LEN)];
    uint8_t      mac[6];
    int          mtu;
    tfIpAddr_t  ip;
    uint32_t     txPkts;
    uint32_t     rxPkts;
    uint64_t     txBytes;
    uint64_t     rxBytes;
}tfMgmtIfInfo_t;

typedef struct
{
    uint32_t   dst;/* Ŀ������ */
    uint32_t   netmask;/* �������� */
    uint32_t   gateway;/* ��һ��·�� */
    uint32_t   cost;
    uint16_t   flag; /* bit0:��Ч*/
    struct 
    {
        uint8_t type;/* �ӿ����� */
        union 
        {
            uint16_t vlanId;
            uint16_t nullId;
        }id;
    }intf;

    uint8_t    type;/* ·����Ŀ���� */
    uint8_t    prio;
    char       ifname[IF_NAME_STR_LEN];/* �����Ľӿ����� */
}tfIpRoute_t;

typedef struct
{
    uint32_t   ipAddr;
    char       desc[TF_SYS_LOG_DESC_STR_LEN];
    uint8_t    state;
}tfSysLogHost_t;

typedef tfMgmtIfInfo_t tfVlanIfInfo_t;

typedef struct
{
    int type;
    union {
        int cause;
        int status;
    }val;
}tfTypeVal_t;

typedef tfTypeVal_t tfSysRebootCtrl_t;

typedef tfTypeVal_t tfSysOperStatus_t;

typedef struct
{
    int port;
    union 
    {
        int disable;/* �����ʾ�ر�;���ʾʹ�� */
    }val;
}tfPortVal_t;

typedef tfPortVal_t tfPortOperDisable_t;
#endif

