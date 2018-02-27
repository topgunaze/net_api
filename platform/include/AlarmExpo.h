#ifndef _SYS_ALARM_ID_H_
#define _SYS_ALARM_ID_H_

extern unsigned long  gAlarmDebug;

/*#define ALARM_SYS_MAIN_BOARD */    /*����*/
#ifdef  ALARM_SYS_MAIN_BOARD
#define ALARM_POOL_LEN          1024*10     /*�澯�س���*/
#define ALARM_POOL_NUMS         250         /*����ظ澯�����Ŀ��*/
#define _HISTORY_LEN_             10*1024     /*��ʷ�澯������*/

#else   /*�߿�*/
#define ALARM_POOL_LEN          1024        /*�澯�س���*/
#define ALARM_POOL_NUMS         25         /*����ظ澯�����Ŀ��*/
#define _HISTORY_LEN_             1024     /*��ʷ�澯������*/

#endif

/*�澯ID������*/
#define ALARM_NUM           250

#define ALARM_NAME_LEN        128    /*�澯���Ƶĳ���*/
/*�澯����*/
#define DEVICE_ALARM            0x1<<24
#define SERVIEC_QUALITY_ALARM   0x2<<24
#define COMMUNICATION_ALARM     0x3<<24
#define ENVIRONMENT_ALARM       0x4<<24
#define HANDLE_FAIL_ALARM       0x5<<24
#define SYSTEM_EVENT            0x6<<24

/*�澯ģ��*/
#define OAM_MODULE              0x1<<16
#define VIFDRV_MODULE           0x2<<16
#define STP_MODULE              0x3<<16
#define DEVMAN_MODULE           0x4<<16
#define ONURA_MODULE            0x5<<16
#define ETF_MODULE             0x6<<16
#define GTF_MODULE             0x7<<16
#define PORTMAN_MODULE          0x8<<16


extern UINT32  gifTrapEnable;
extern UINT32  gifLogEnable;

typedef enum ALARM_LEVEL
{
    CRITICAL_LEVEL = 1,/*����*/
    MAJOR_LEVEL    = 2,/*��Ҫ*/
    MINOR_LEVEL    = 3,/*��Ҫ*/
    WARNING_LEVEL  = 4,/*����*/
    NOTIFY_LEVEL = 5/* ����澯����*/
}SYS_ALARM_LEVEL;

typedef enum
{
    NODE_TYPE       = 2000,  /*����*/
    SHELF_TYPE      = 2001 , /*��������*/ 
    MODULE_TYPE     = 2002,  /*�忨����*/
    CLOCK_TYPE      = 2003,  /*CLOCK����*/
    ETHERNET_TYPE   = 2005, /*ETHERNET����*/ 
    TF_TYPE        = 2006,  /*TF����*/  
    ONU_TYPE        = 2007,  /*ONU����*/
    ONU_ETHERTYPE_TYPE = 2009,/*ONU��̫������*/
    ONU_TFIF_TYPE  = 2010, /*ONUtfif����*/
    ONU_CARD_TYPE   = 2011, /*ONU_card����*/
    ONU_POTS_TYPE = 2012,  /*ONU_pots����*/
    ONU_E1_TYPE     = 2013 /*ONU_pots����*/
}SYS_ALARM_ENTITY_TYPE;

typedef enum
{
    DEVICE_ALARM_TYPE           = 1,            /*�豸�澯*/
    SERVIEC_QUALITY_ALARM_TYPE  = 2,            /*������������*/
    COMMUNICATION_ALARM_TYPE    = 3,            /*ͨ������*/
    ENVIRONMENT_ALARM_TYPE      = 4,             /*��������*/
    HANDLE_FAIL_ALARM_TYPE      = 5,             /*����ʧ������*/
    SYSTEM_EVENT_TYPE           = 6,             /*�¼�����*/
}SYS_ALARM_TYPE;

/*���͸�trap�Ľṹ��*/
typedef struct
{
    UINT32   SessionId;
    UINT32   SequenceNum;
    UINT32   ReportTime;
    UINT32   ReportClass;
    UINT32   EntityType;
    UINT32   EntityInstance;
    UINT32   EnitySeverity;
    UINT32   ProbableCause;
    UINT32   CorrelationId;
    UINT8   VersionId[32]; 
    UINT8   SourceIp[64];
    UINT8   AdditionalText[64];
}SYS_SEND_ALARM_TO_TRAP_ST;

typedef struct
{
    UINT32   SessionId;
    UINT32   SequenceNum;
    UINT32   ReportTime;
    UINT32   EntityType;
    UINT32   EntityInstance;
    UINT32   EventType;
    UINT8   EventInfo[128];
    UINT8   AdditionalText[64];
}SYS_SEND_EVENT_TO_TRAP_ST;

typedef enum 
{
    /*-----------------------------------------�豸�澯-------------------------------*/
    /*�豸�ϱ��澯*/
    SYS_ALARM_DEVMAN_TFCARD_PULL_OUT                   =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x0001  ,
    SYS_ALARM_DEVMAN_OLT_LOOPBACK_DETECT                =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x0002  ,
    SYS_ALARM_DEVMAN_BOARD_PULL_OUT                     =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x0003  ,
    SYS_ALARM_DEVMAN_OLT_FAN_PULL_OUT                   =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x0004  ,
    SYS_ALARM_VIF_LINKDOWN                              =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x0027  ,

    /*�豸����澯*/
    SYS_ALARM_DEVMAN_TFCARD_PLUG_IN                    =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x1001  ,
    SYS_ALARM_DEVMAN_OLT_LOOPBACK_CLEAR                 =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x1002  ,
    SYS_ALARM_DEVMAN_BOARD_PLUG_IN                      =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x1003  ,
    SYS_ALARM_DEVMAN_OLT_FAN_PLUG_IN                    =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x1004  ,
    SYS_ALARM_VIF_LINKUP                                =        DEVICE_ALARM   |   DEVMAN_MODULE   |   0x1007  ,
    /*GTFģ���ϱ��澯*/
    SYS_ALARM_GTF_PLAOM_LOSI_ALARM                     =        DEVICE_ALARM   |   GTF_MODULE |   0x0001  ,
    SYS_ALARM_GTF_PLAOM_LOFI_ALARM                     =        DEVICE_ALARM   |   GTF_MODULE |   0x0002  ,
    SYS_ALARM_GTF_PLAOM_SF_ALARM                       =        DEVICE_ALARM   |   GTF_MODULE |   0x0003  ,
    SYS_ALARM_GTF_PLAOM_SD_ALARM                       =        DEVICE_ALARM   |   GTF_MODULE |   0x0004  ,
    SYS_ALARM_GTF_PLAOM_LCDGI_ALARM                    =        DEVICE_ALARM   |   GTF_MODULE |   0x0005  ,
    SYS_ALARM_GTF_PLAOM_DFI_ALARM                      =        DEVICE_ALARM   |   GTF_MODULE |   0x0006  ,

    /*GTFģ������澯*/                    
    SYS_ALARM_GTF_PLAOM_LOSI_ALARM_CLEAR               =        DEVICE_ALARM   |   GTF_MODULE |   0x1001  ,
    SYS_ALARM_GTF_PLAOM_LOFI_ALARM_CLEAR               =        DEVICE_ALARM   |   GTF_MODULE |   0x1002  ,
    SYS_ALARM_GTF_PLAOM_SF_ALARM_CLEAR                 =        DEVICE_ALARM   |   GTF_MODULE |   0x1003  ,
    SYS_ALARM_GTF_PLAOM_SD_ALARM_CLEAR                 =        DEVICE_ALARM   |   GTF_MODULE |   0x1004  ,
    SYS_ALARM_GTF_PLAOM_LCDGI_ALARM_CLEAR              =        DEVICE_ALARM   |   GTF_MODULE |   0x1005  ,
    SYS_ALARM_GTF_PLAOM_DFI_ALARM_CLEAR                =        DEVICE_ALARM   |   GTF_MODULE |   0x1006  ,


    #if 1
    /*ONU*/
    SYS_ALARM_OAM_CTC_EQUIPMENT_ALARM                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x0001  ,
    SYS_ALARM_OAM_CTC_POWER_ALARM                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x0002  ,
    SYS_ALARM_OAM_CTC_BATTERY_MISSING                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x0003  ,
    SYS_ALARM_OAM_CTC_BATTERY_FAILURE                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x0004  ,
    SYS_ALARM_OAM_CTC_BATTERY_VOLT_LOW                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x0005  ,
    SYS_ALARM_OAM_CTC_PHYSICAL_INSTRUSION_ALARM         =        DEVICE_ALARM   |   OAM_MODULE  |   0x0006  ,
    SYS_ALARM_OAM_CTC_ONU_SELFTEST_FAILURE              =        DEVICE_ALARM   |   OAM_MODULE  |   0x0007  ,
    SYS_ALARM_OAM_CTC_ONU_TEMP_HIGH_ALARM               =        DEVICE_ALARM   |   OAM_MODULE  |   0x0009  ,
    SYS_ALARM_OAM_CTC_ONU_TEMP_LOW_ALARM                =        DEVICE_ALARM   |   OAM_MODULE  |   0x000a  ,
    SYS_ALARM_OAM_CTC_IAD_CONNECTION_FAILURE            =        DEVICE_ALARM   |   OAM_MODULE  |   0x000b  ,
    SYS_ALARM_OAM_CTC_TF_IFSWITCH                      =        DEVICE_ALARM   |   OAM_MODULE  |   0x000c  ,
    SYS_ALARM_OAM_CTC_SLEEP_STATUS_UPDATE               =        DEVICE_ALARM   |   OAM_MODULE  |   0x000d  ,
    /*0x000D~000FF  reserved */
    
    /*OAM TF-IF*/
    SYS_ALARM_OAM_CTC_RX_POWER_HIGH_ALARM               =        DEVICE_ALARM   |   OAM_MODULE  |   0x0101  ,
    SYS_ALARM_OAM_CTC_RX_POWER_LOW_ALARM                =        DEVICE_ALARM   |   OAM_MODULE  |   0x0102  ,
    SYS_ALARM_OAM_CTC_TX_POWER_HIGH_ALARM               =        DEVICE_ALARM   |   OAM_MODULE  |   0x0103  ,
    SYS_ALARM_OAM_CTC_TX_POWER_LOW_ALARM                =        DEVICE_ALARM   |   OAM_MODULE  |   0x0104  ,
    SYS_ALARM_OAM_CTC_TX_BIAS_HIGH_ALARM                =        DEVICE_ALARM   |   OAM_MODULE  |   0x0105  ,
    SYS_ALARM_OAM_CTC_TX_BIAS_LOW_ALARM                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x0106  ,
    SYS_ALARM_OAM_CTC_VCC_HIGH_ALARM                    =        DEVICE_ALARM   |   OAM_MODULE  |   0x0107  ,
    SYS_ALARM_OAM_CTC_VCC_LOW_ALARM                     =        DEVICE_ALARM   |   OAM_MODULE  |   0x0108  ,
    SYS_ALARM_OAM_CTC_TEMP_HIGH_ALARM                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x0109  ,
    SYS_ALARM_OAM_CTC_TEMP_LOW_ALARM                    =        DEVICE_ALARM   |   OAM_MODULE  |   0x010a  ,
    SYS_ALARM_OAM_CTC_RX_POWER_HIGH_WARNING             =        DEVICE_ALARM   |   OAM_MODULE  |   0x010b  ,
    SYS_ALARM_OAM_CTC_RX_POWER_LOW_WARNING              =        DEVICE_ALARM   |   OAM_MODULE  |   0x010c  ,
    SYS_ALARM_OAM_CTC_TX_POWER_HIGH_WARNING             =        DEVICE_ALARM   |   OAM_MODULE  |   0x010d  ,
    SYS_ALARM_OAM_CTC_TX_POWER_LOW_WARNING              =        DEVICE_ALARM   |   OAM_MODULE  |   0x010e  ,
    SYS_ALARM_OAM_CTC_TX_BIAS_HIGH_WARNING              =        DEVICE_ALARM   |   OAM_MODULE  |   0x010f  ,
    SYS_ALARM_OAM_CTC_TX_BIAS_LOW_WARNING               =        DEVICE_ALARM   |   OAM_MODULE  |   0x0110  ,
    SYS_ALARM_OAM_CTC_VCC_HIGH_WARNING                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x0111  ,
    SYS_ALARM_OAM_CTC_VCC_LOW_WARNING                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x0112  ,
    SYS_ALARM_OAM_CTC_TEMP_HIGH_WARNING                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x0113  ,
    SYS_ALARM_OAM_CTC_TEMP_LOW_WARNING                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x0114  ,

    SYS_ALARM_OAM_CTC_DS_DROP_EVENTS                    =        DEVICE_ALARM   |   OAM_MODULE  |   0x0115  ,
    SYS_ALARM_OAM_CTC_US_DROP_EVENTS                    =        DEVICE_ALARM   |   OAM_MODULE  |   0x0116  ,
    SYS_ALARM_OAM_CTC_DS_CRC_ERRORFRAMES                =        DEVICE_ALARM   |   OAM_MODULE  |   0x0117  ,
    SYS_ALARM_OAM_CTC_DS_UNDERSIZE_FRAMES               =        DEVICE_ALARM   |   OAM_MODULE  |   0x0118  ,
    SYS_ALARM_OAM_CTC_US_UNDERSIZE_FRAMES               =        DEVICE_ALARM   |   OAM_MODULE  |   0x0119  ,
    SYS_ALARM_OAM_CTC_DS_OVERSIZE_FRAMES                =        DEVICE_ALARM   |   OAM_MODULE  |   0x011a  ,
    SYS_ALARM_OAM_CTC_US_OVERSIZE_FRAMES                =        DEVICE_ALARM   |   OAM_MODULE  |   0x011b  ,
    SYS_ALARM_OAM_CTC_DS_FRAGMENTS                      =        DEVICE_ALARM   |   OAM_MODULE  |   0x011c  ,
    SYS_ALARM_OAM_CTC_DS_JABBERS                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x011d  ,
    SYS_ALARM_OAM_CTC_DS_COLLISIONS                     =        DEVICE_ALARM   |   OAM_MODULE  |   0x011e  ,
    SYS_ALARM_OAM_CTC_DS_DISCARDS                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x011f  ,
    SYS_ALARM_OAM_CTC_US_DISCARDS                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x0120  ,
    SYS_ALARM_OAM_CTC_DS_ERRORS                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x0121  ,
    SYS_ALARM_OAM_CTC_DS_DROP_EVENTS_WARNING            =        DEVICE_ALARM   |   OAM_MODULE  |   0x0122  ,
    SYS_ALARM_OAM_CTC_US_DROP_EVENTS_WARNING            =        DEVICE_ALARM   |   OAM_MODULE  |   0x0123  ,
    SYS_ALARM_OAM_CTC_DS_CRC_ERRORFRAMES_WARNING        =        DEVICE_ALARM   |   OAM_MODULE  |   0x0124  ,
    SYS_ALARM_OAM_CTC_DS_UNDERSIZE_FRAMES_WARNING       =        DEVICE_ALARM   |   OAM_MODULE  |   0x0125  ,
    SYS_ALARM_OAM_CTC_US_UNDERSIZE_FRAMES_WARNING       =        DEVICE_ALARM   |   OAM_MODULE  |   0x0126  ,
    SYS_ALARM_OAM_CTC_DS_OVERSIZE_FRAMES_WARNING        =        DEVICE_ALARM   |   OAM_MODULE  |   0x0127  ,
    SYS_ALARM_OAM_CTC_US_OVERSIZE_FRAMES_WARNING        =        DEVICE_ALARM   |   OAM_MODULE  |   0x0128  ,
    SYS_ALARM_OAM_CTC_DS_FRAGMENTS_WARNING              =        DEVICE_ALARM   |   OAM_MODULE  |   0x0129  ,
    SYS_ALARM_OAM_CTC_DS_JABBERS_WARNING                =        DEVICE_ALARM   |   OAM_MODULE  |   0x012a  ,
    SYS_ALARM_OAM_CTC_DS_COLLISIONS_WARNING             =        DEVICE_ALARM   |   OAM_MODULE  |   0x012b  ,
    SYS_ALARM_OAM_CTC_DS_DISCARDS_WARNING               =        DEVICE_ALARM   |   OAM_MODULE  |   0x012c  ,
    SYS_ALARM_OAM_CTC_US_DISCARDS_WARNING               =        DEVICE_ALARM   |   OAM_MODULE  |   0x012d  ,
    SYS_ALARM_OAM_CTC_DS_ERRORS_WARNING                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x012e  ,
    /*0x012F��0x01FF reserved*/

    /*OAM PROT*/
    SYS_ALARM_OAM_CTC_ETH_PORT_AUTO_NEG_FAILURE                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x0301  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_LOS                              =        DEVICE_ALARM   |   OAM_MODULE  |   0x0302  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_FAILURE                          =        DEVICE_ALARM   |   OAM_MODULE  |   0x0303  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_LOOPBACK                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x0304  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_CONGESTION                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x0305  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_DROP_EVENTS                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x0306  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_DROP_EVENTS                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x0307  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_CRC_ERRORFRAMES               =        DEVICE_ALARM   |   OAM_MODULE  |   0x0308  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_UNDERSIZE_FRAMES              =        DEVICE_ALARM   |   OAM_MODULE  |   0x0309  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_UNDERSIZE_FRAMES              =        DEVICE_ALARM   |   OAM_MODULE  |   0x030a  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_OVERSIZE_FRAMES               =        DEVICE_ALARM   |   OAM_MODULE  |   0x030b  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_OVERSIZE_FRAMES               =        DEVICE_ALARM   |   OAM_MODULE  |   0x030c  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_FRAGMENTS                     =        DEVICE_ALARM   |   OAM_MODULE  |   0x030d  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_JABBERS                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x030e  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_COLLISIONS                    =        DEVICE_ALARM   |   OAM_MODULE  |   0x030f,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_DISCARDS                      =        DEVICE_ALARM   |   OAM_MODULE  |   0x0310  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_DISCARDS                      =        DEVICE_ALARM   |   OAM_MODULE  |   0x0311  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_ERRORS                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x0312  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_STATUS_CHANGE_TIMES              =        DEVICE_ALARM   |   OAM_MODULE  |   0x0313  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_DROP_EVENTS_WARNING           =        DEVICE_ALARM   |   OAM_MODULE  |   0x0314  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_DROP_EVENTS_WARNING           =        DEVICE_ALARM   |   OAM_MODULE  |   0x0315  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_CRC_ERRORFRAMES_WARNING       =        DEVICE_ALARM   |   OAM_MODULE  |   0x0316  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_UNDERSIZE_FRAMES_WARNING      =        DEVICE_ALARM   |   OAM_MODULE  |   0x0317  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_UNDERSIZE_FRAMES_WARNING      =        DEVICE_ALARM   |   OAM_MODULE  |   0x0318  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_OVERSIZE_FRAMES_WARNING       =        DEVICE_ALARM   |   OAM_MODULE  |   0x0319  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_OVERSIZE_FRAMES_WARNING       =        DEVICE_ALARM   |   OAM_MODULE  |   0x031a  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_FRAGMENTS_WARNING             =        DEVICE_ALARM   |   OAM_MODULE  |   0x031b  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_JABBERS_WARNING               =        DEVICE_ALARM   |   OAM_MODULE  |   0x031c  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_COLLISIONS_WARNING            =        DEVICE_ALARM   |   OAM_MODULE  |   0x031d,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_DISCARDS_WARNING              =        DEVICE_ALARM   |   OAM_MODULE  |   0x031e  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_DISCARDS_WARNING              =        DEVICE_ALARM   |   OAM_MODULE  |   0x031f  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_ERRORS_WARNING                =        DEVICE_ALARM   |   OAM_MODULE  |   0x0320  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_STATUS_CHANGE_TIMES_WARNING      =        DEVICE_ALARM   |   OAM_MODULE  |   0x0321  ,
    /*0x0322��0x03FF reserved*/
    SYS_ALARM_OAM_CTC_POTS_PORT_FAILURE                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x0401  ,
    /*0x0402��0x04FF reserved*/
    SYS_ALARM_OAM_CTC_E1_PORT_FAILURE                           =        DEVICE_ALARM   |   OAM_MODULE  |   0x0501  ,
    SYS_ALARM_OAM_CTC_E1_PORT_TIMING_UNLOCK                     =        DEVICE_ALARM   |   OAM_MODULE  |   0x0502  ,
    SYS_ALARM_OAM_CTC_E1_PORT_LOS                               =        DEVICE_ALARM   |   OAM_MODULE  |   0x0503  ,
    /*0x0504��0x05FF reserved*/
    SYS_ALARM_OAM_CTC_ONU_VCC_HIGH_ALARM                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x0601  ,
    SYS_ALARM_OAM_CTC_ONU_VCC_LOW_ALARM                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x0602  ,
    SYS_ALARM_OAM_CTC_ONU_BIAS_HIGH_ALARM                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x0603  ,
    SYS_ALARM_OAM_CTC_ONU_BIAS_LOW_ALARM                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x0604  ,
    SYS_ALARM_OAM_CTC_ONU_TEMP_HIGH_WARNING                     =        DEVICE_ALARM   |   OAM_MODULE  |   0x0605  ,
    SYS_ALARM_OAM_CTC_ONU_TEMP_LOW_WARNING                      =        DEVICE_ALARM   |   OAM_MODULE  |   0x0606  ,
    SYS_ALARM_OAM_CTC_ONU_VCC_HIGH_WARNING                      =        DEVICE_ALARM   |   OAM_MODULE  |   0x0607  ,
    SYS_ALARM_OAM_CTC_ONU_VCC_LOW_WARNING                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x0608  ,
    SYS_ALARM_OAM_CTC_ONU_BIAS_HIGH_WARNING                     =        DEVICE_ALARM   |   OAM_MODULE  |   0x0609  ,
    SYS_ALARM_OAM_CTC_ONU_BIAS_LOW_WARNING                      =        DEVICE_ALARM   |   OAM_MODULE  |   0x060a  ,
            
    SYS_ALARM_OAM_ONU_OAM_RESTFSE_TIMEOUT                      =        SYSTEM_EVENT   |   OAM_MODULE  |   0x060b  ,
    SYS_ALARM_OAM_ONU_EXT_OAM_NOTSUPPORT                        =        SYSTEM_EVENT   |   OAM_MODULE  |   0x060c  ,


    /*OAM clear*/
    SYS_ALARM_OAM_CTC_EQUIPMENT_ALARM_CLEAR             =        ENVIRONMENT_ALARM  |   OAM_MODULE  |   0x1001  ,
    SYS_ALARM_OAM_CTC_POWER_ALARM_CLEAR                 =        ENVIRONMENT_ALARM  |   OAM_MODULE  |   0x1002  ,
    SYS_ALARM_OAM_CTC_BATTERY_MISSING_CLEAR             =        ENVIRONMENT_ALARM  |   OAM_MODULE  |   0x1003  ,
    SYS_ALARM_OAM_CTC_BATTERY_FAILURE_CLEAR             =        ENVIRONMENT_ALARM  |   OAM_MODULE  |   0x1004  ,
    SYS_ALARM_OAM_CTC_BATTERY_VOLT_LOW_CLEAR            =        ENVIRONMENT_ALARM  |   OAM_MODULE  |   0x1005  ,
    SYS_ALARM_OAM_CTC_PHYSICAL_INSTRUSION_ALARM_CLEAR   =        ENVIRONMENT_ALARM  |   OAM_MODULE  |   0x1006  ,
    SYS_ALARM_OAM_CTC_ONU_SELFTEST_FAILURE_CLEAR        =        HANDLE_FAIL_ALARM  |   OAM_MODULE  |   0x1007  ,

    SYS_ALARM_OAM_CTC_ONU_TEMP_HIGH_ALARM_CLEAR             =        DEVICE_ALARM   |   OAM_MODULE  |   0x1009  ,
    SYS_ALARM_OAM_CTC_ONU_TEMP_LOW_ALARM_CLEAR              =        DEVICE_ALARM   |   OAM_MODULE  |   0x100a  ,
    SYS_ALARM_OAM_CTC_IAD_CONNECTION_FAILURE_CLEAR          =        DEVICE_ALARM   |   OAM_MODULE  |   0x100b  ,
    SYS_ALARM_OAM_CTC_TF_IFSWITCH_CLEAR                    =        DEVICE_ALARM   |   OAM_MODULE  |   0x100c  ,
    SYS_ALARM_OAM_CTC_SLEEP_STATUS_UPDATE_CLEAR             =        DEVICE_ALARM   |   OAM_MODULE  |   0x100d  ,

    
    SYS_ALARM_OAM_CTC_RX_POWER_HIGH_ALARM_CLEAR             =        DEVICE_ALARM   |   OAM_MODULE  |   0x1101  ,
    SYS_ALARM_OAM_CTC_RX_POWER_LOW_ALARM_CLEAR              =        DEVICE_ALARM   |   OAM_MODULE  |   0x1102  ,
    SYS_ALARM_OAM_CTC_TX_POWER_HIGH_ALARM_CLEAR             =        DEVICE_ALARM   |   OAM_MODULE  |   0x1103  ,
    SYS_ALARM_OAM_CTC_TX_POWER_LOW_ALARM_CLEAR              =        DEVICE_ALARM   |   OAM_MODULE  |   0x1104  ,
    SYS_ALARM_OAM_CTC_TX_BIAS_HIGH_ALARM_CLEAR              =        DEVICE_ALARM   |   OAM_MODULE  |   0x1105  ,
    SYS_ALARM_OAM_CTC_TX_BIAS_LOW_ALARM_CLEAR               =        DEVICE_ALARM   |   OAM_MODULE  |   0x1106  ,
    SYS_ALARM_OAM_CTC_VCC_HIGH_ALARM_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x1107  ,
    SYS_ALARM_OAM_CTC_VCC_LOW_ALARM_CLEAR                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x1108  ,
    SYS_ALARM_OAM_CTC_TEMP_HIGH_ALARM_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1109  ,
    SYS_ALARM_OAM_CTC_TEMP_LOW_ALARM_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x110a  ,
    SYS_ALARM_OAM_CTC_RX_POWER_HIGH_WARNING_CLEAR           =        DEVICE_ALARM   |   OAM_MODULE  |   0x110b  ,
    SYS_ALARM_OAM_CTC_RX_POWER_LOW_WARNING_CLEAR            =        DEVICE_ALARM   |   OAM_MODULE  |   0x110c  ,
    SYS_ALARM_OAM_CTC_TX_POWER_HIGH_WARNING_CLEAR           =        DEVICE_ALARM   |   OAM_MODULE  |   0x110d  ,
    SYS_ALARM_OAM_CTC_TX_POWER_LOW_WARNING_CLEAR            =        DEVICE_ALARM   |   OAM_MODULE  |   0x110e  ,
    SYS_ALARM_OAM_CTC_TX_BIAS_HIGH_WARNING_CLEAR            =        DEVICE_ALARM   |   OAM_MODULE  |   0x110f  ,
    SYS_ALARM_OAM_CTC_TX_BIAS_LOW_WARNING_CLEAR             =        DEVICE_ALARM   |   OAM_MODULE  |   0x1110  ,
    SYS_ALARM_OAM_CTC_VCC_HIGH_WARNING_CLEAR                =        DEVICE_ALARM   |   OAM_MODULE  |   0x1111  ,
    SYS_ALARM_OAM_CTC_VCC_LOW_WARNING_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1112  ,
    SYS_ALARM_OAM_CTC_TEMP_HIGH_WARNING_CLEAR               =        DEVICE_ALARM   |   OAM_MODULE  |   0x1113  ,
    SYS_ALARM_OAM_CTC_TEMP_LOW_WARNING_CLEAR                =        DEVICE_ALARM   |   OAM_MODULE  |   0x1114  ,

    SYS_ALARM_OAM_CTC_DS_DROP_EVENTS_CLEAR                      =        DEVICE_ALARM   |   OAM_MODULE  |   0x1115  ,
    SYS_ALARM_OAM_CTC_US_DROP_EVENTS_CLEAR                      =        DEVICE_ALARM   |   OAM_MODULE  |   0x1116  ,
    SYS_ALARM_OAM_CTC_DS_CRC_ERRORFRAMES_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x1117  ,
    SYS_ALARM_OAM_CTC_US_CRC_ERRORFRAMES_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x1118  ,
    SYS_ALARM_OAM_CTC_DS_UNDERSIZE_FRAMES_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1119  ,
    SYS_ALARM_OAM_CTC_US_UNDERSIZE_FRAMES_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x111a  ,
    SYS_ALARM_OAM_CTC_DS_OVERSIZE_FRAMES_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x111b  ,
    SYS_ALARM_OAM_CTC_US_OVERSIZE_FRAMES_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x111c  ,
    SYS_ALARM_OAM_CTC_DS_FRAGMENTS_CLEAR                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x111d  ,
    SYS_ALARM_OAM_CTC_US_FRAGMENTS_CLEAR                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x111e  ,
    SYS_ALARM_OAM_CTC_DS_JABBERS_CLEAR                          =        DEVICE_ALARM   |   OAM_MODULE  |   0x111f  ,
    SYS_ALARM_OAM_CTC_US_JABBERS_CLEAR                          =        DEVICE_ALARM   |   OAM_MODULE  |   0x1120  ,
    SYS_ALARM_OAM_CTC_DS_DISCARDS_CLEAR                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x1121  ,
    SYS_ALARM_OAM_CTC_US_DISCARDS_CLEAR                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x1122  ,
    SYS_ALARM_OAM_CTC_DS_ERRORS_CLEAR                           =        DEVICE_ALARM   |   OAM_MODULE  |   0x1123  ,
    SYS_ALARM_OAM_CTC_US_ERRORS_CLEAR                           =        DEVICE_ALARM   |   OAM_MODULE  |   0x1124  ,
    SYS_ALARM_OAM_CTC_DS_DROP_EVENTS_WARNING_CLEAR              =        DEVICE_ALARM   |   OAM_MODULE  |   0x1125  ,
    SYS_ALARM_OAM_CTC_US_DROP_EVENTS_WARNING_CLEAR              =        DEVICE_ALARM   |   OAM_MODULE  |   0x1126  ,
    SYS_ALARM_OAM_CTC_DS_CRC_ERRORFRAMES_WARNING_CLEAR          =        DEVICE_ALARM   |   OAM_MODULE  |   0x1127  ,
    SYS_ALARM_OAM_CTC_US_CRC_ERRORFRAMES_WARNING_CLEAR          =        DEVICE_ALARM   |   OAM_MODULE  |   0x1128  ,
    SYS_ALARM_OAM_CTC_DS_UNDERSIZE_FRAMES_WARNING_CLEAR         =        DEVICE_ALARM   |   OAM_MODULE  |   0x1129  ,
    SYS_ALARM_OAM_CTC_US_UNDERSIZE_FRAMES_WARNING_CLEAR         =        DEVICE_ALARM   |   OAM_MODULE  |   0x112a  ,
    SYS_ALARM_OAM_CTC_DS_OVERSIZE_FRAMES_WARNING_CLEAR          =        DEVICE_ALARM   |   OAM_MODULE  |   0x112b  ,
    SYS_ALARM_OAM_CTC_US_OVERSIZE_FRAMES_WARNING_CLEAR          =        DEVICE_ALARM   |   OAM_MODULE  |   0x112c  ,
    SYS_ALARM_OAM_CTC_DS_FRAGMENTS_WARNING_CLEAR                =        DEVICE_ALARM   |   OAM_MODULE  |   0x112d  ,
    SYS_ALARM_OAM_CTC_US_FRAGMENTS_WARNING_CLEAR                =        DEVICE_ALARM   |   OAM_MODULE  |   0x112e  ,
    SYS_ALARM_OAM_CTC_DS_JABBERS_WARNING_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x112f  ,
    SYS_ALARM_OAM_CTC_US_JABBERS_WARNING_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x1130  ,
    SYS_ALARM_OAM_CTC_DS_DISCARDS_WARNING_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1131  ,
    SYS_ALARM_OAM_CTC_US_DISCARDS_WARNING_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1132  ,
    SYS_ALARM_OAM_CTC_DS_ERRORS_WARNING_CLEAR                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x1133  ,
    SYS_ALARM_OAM_CTC_US_ERRORS_WARNING_CLEAR                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x1134  ,

    SYS_ALARM_OAM_CTC_ETH_PORT_AUTO_NEG_FAILURE_CLEAR                   =        DEVICE_ALARM   |   OAM_MODULE  |   0x1301  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_LOS_CLEAR                                =        DEVICE_ALARM   |   OAM_MODULE  |   0x1302  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_FAILURE_CLEAR                            =        DEVICE_ALARM   |   OAM_MODULE  |   0x1303  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_LOOPBACK_CLEAR                           =        DEVICE_ALARM   |   OAM_MODULE  |   0x1304  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_CONGESTION_CLEAR                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x1305  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_DROP_EVENTS_CLEAR                     =        DEVICE_ALARM   |   OAM_MODULE  |   0x1306  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_DROP_EVENTS_CLEAR                     =        DEVICE_ALARM   |   OAM_MODULE  |   0x1307  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_CRC_ERRORFRAMES_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1308  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_CRC_ERRORFRAMES_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1309  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_UNDERSIZE_FRAMES_CLEAR                =        DEVICE_ALARM   |   OAM_MODULE  |   0x130a  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_UNDERSIZE_FRAMES_CLEAR                =        DEVICE_ALARM   |   OAM_MODULE  |   0x130b  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_OVERSIZE_FRAMES_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x130c  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_OVERSIZE_FRAMES_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x130d  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_FRAGMENTS_CLEAR                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x130e  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_FRAGMENTS_CLEAR                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x130f  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_JABBERS_CLEAR                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x1310  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_JABBERS_CLEAR                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x1311  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_DISCARDS_CLEAR                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x1312  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_DISCARDS_CLEAR                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x1313  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_ERRORS_CLEAR                          =        DEVICE_ALARM   |   OAM_MODULE  |   0x1314  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_ERRORS_CLEAR                          =        DEVICE_ALARM   |   OAM_MODULE  |   0x1315  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_STATUS_CHANGE_TIMES_CLEAR                =        DEVICE_ALARM   |   OAM_MODULE  |   0x1316  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_DROP_EVENTS_WARNING_CLEAR             =        DEVICE_ALARM   |   OAM_MODULE  |   0x1317  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_DROP_EVENTS_WARNING_CLEAR             =        DEVICE_ALARM   |   OAM_MODULE  |   0x1318  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_CRC_ERRORFRAMES_WARNING_CLEAR         =        DEVICE_ALARM   |   OAM_MODULE  |   0x1319  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_CRC_ERRORFRAMES_WARNING_CLEAR         =        DEVICE_ALARM   |   OAM_MODULE  |   0x131a  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_UNDERSIZE_FRAMES_WARNING_CLEAR        =        DEVICE_ALARM   |   OAM_MODULE  |   0x131b  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_UNDERSIZE_FRAMES_WARNING_CLEAR        =        DEVICE_ALARM   |   OAM_MODULE  |   0x131c  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_OVERSIZE_FRAMES_WARNING_CLEAR         =        DEVICE_ALARM   |   OAM_MODULE  |   0x131d  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_OVERSIZE_FRAMES_WARNING_CLEAR         =        DEVICE_ALARM   |   OAM_MODULE  |   0x131e  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_FRAGMENTS_WARNING_CLEAR               =        DEVICE_ALARM   |   OAM_MODULE  |   0x131f  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_FRAGMENTS_WARNING_CLEAR               =        DEVICE_ALARM   |   OAM_MODULE  |   0x1320  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_JABBERS_WARNING_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1321  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_JABBERS_WARNING_CLEAR                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1322  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_DISCARDS_WARNING_CLEAR                =        DEVICE_ALARM   |   OAM_MODULE  |   0x1323  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_DISCARDS_WARNING_CLEAR                =        DEVICE_ALARM   |   OAM_MODULE  |   0x1324  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_DS_ERRORS_WARNING_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x1325  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_US_ERRORS_WARNING_CLEAR                  =        DEVICE_ALARM   |   OAM_MODULE  |   0x1326  ,
    SYS_ALARM_OAM_CTC_ETH_PORT_STATUS_CHANGE_TIMES_WARNING_CLEAR        =        DEVICE_ALARM   |   OAM_MODULE  |   0x1327  ,

    SYS_ALARM_OAM_CTC_POTS_PORT_FAILURE_CLEAR                           =        DEVICE_ALARM   |   OAM_MODULE  |   0x1402  ,
    
    SYS_ALARM_OAM_CTC_E1_PORT_FAILURE_CLEAR                             =        DEVICE_ALARM    |  OAM_MODULE  |   0x1501  ,
    SYS_ALARM_OAM_CTC_E1_PORT_TIMING_UNLOCK_CLEAR                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x1502  ,
    SYS_ALARM_OAM_CTC_E1_PORT_LOS_CLEAR                                 =        DEVICE_ALARM   |   OAM_MODULE  |   0x1503  ,

    SYS_ALARM_OAM_CTC_ONU_VCC_HIGH_ALARM_CLEAR                          =        DEVICE_ALARM   |   OAM_MODULE  |   0x1601  ,
    SYS_ALARM_OAM_CTC_ONU_VCC_LOW_ALARM_CLEAR                           =        DEVICE_ALARM   |   OAM_MODULE  |   0x1602  ,
    SYS_ALARM_OAM_CTC_ONU_BIAS_HIGH_ALARM_CLEAR                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x1603  ,
    SYS_ALARM_OAM_CTC_ONU_BIAS_LOW_ALARM_CLEAR                          =        DEVICE_ALARM   |   OAM_MODULE  |   0x1604  ,
    SYS_ALARM_OAM_CTC_ONU_TEMP_HIGH_WARNING_CLEAR                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x1605  ,
    SYS_ALARM_OAM_CTC_ONU_TEMP_LOW_WARNING_CLEAR                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x1606  ,
    SYS_ALARM_OAM_CTC_ONU_VCC_HIGH_WARNING_CLEAR                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x1607  ,
    SYS_ALARM_OAM_CTC_ONU_VCC_LOW_WARNING_CLEAR                         =        DEVICE_ALARM   |   OAM_MODULE  |   0x1608  ,
    SYS_ALARM_OAM_CTC_ONU_BIAS_HIGH_WARNING_CLEAR                       =        DEVICE_ALARM   |   OAM_MODULE  |   0x1609  ,
    SYS_ALARM_OAM_CTC_ONU_BIAS_LOW_WARNING_CLEAR                        =        DEVICE_ALARM   |   OAM_MODULE  |   0x160a  ,
            
    SYS_ALARM_OAM_ONU_OAM_RESTFSE_TIMEOUT_CLEAR                        =        SYSTEM_EVENT   |   OAM_MODULE  |   0x160b  ,
    SYS_ALARM_OAM_ONU_EXT_OAM_NOTSUPPORT_CLEAR                          =        SYSTEM_EVENT   |   OAM_MODULE  |   0x160c  ,

#endif



    /*-----------------------------------------COMMUNICATION�澯-----------------------*/
    /*ETFģ���ϱ�*/
    SYS_ALARM_ETF_PORT_LINK_DOWN                       =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x0001  ,      
    SYS_ALARM_ETF_LOS                                  =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x0002  ,
    SYS_ALARM_ETF_PLUGOUT_OPTICAL_TRANSCEIVE           =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x0003  ,
    SYS_ALARM_ETF_STATICS_TX_POWER                     =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x0004  ,
    SYS_ALARM_ETF_STATICS_VCC                          =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x0005  ,
    SYS_ALARM_ETF_STATICS_BIAS                         =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x0006  ,
    SYS_ALARM_ETF_STATICS_TEMP                         =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x0007  ,
    SYS_ALARM_ETF_STATICS_RX_POWER                     =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x0008  ,
    SYS_ALARM_ETF_LINK_RX_POWER                        =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x0009  ,
    SYS_ALARM_ETF_LASER_IDLE_POWER                     =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x000a  ,
    SYS_ALARM_ETF_STATICS_LLID_ERROR                   =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x000b  ,
    SYS_ALARM_ETF_PORT_ERROR_CODE                      =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x000c  ,
    SYS_ALARM_ETF_STANDARD_DYING_GASP                  =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x000d  ,
    SYS_ALARM_VIF_TF_LOS_RAISE                         =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x000e  ,

    /*ETFģ�����*/
    SYS_ALARM_ETF_PORT_LINK_UP                         =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x1001  ,
    SYS_ALARM_ETF_ETF_LOS_CLEAR                       =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x1002  ,
    SYS_ALARM_ETF_PLUGIN_OPTICAL_TRANSCEIVE            =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x1003  ,
    SYS_ALARM_ETF_STATICS_TX_POWER_CLEAR               =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x1004  ,
    SYS_ALARM_ETF_STATICS_VCC_CLEAR                    =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x1005  ,
    SYS_ALARM_ETF_STATICS_BIAS_CLEAR                   =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x1006  ,
    SYS_ALARM_ETF_STATICS_TEMP_CLEAR                   =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x1007  ,
    SYS_ALARM_ETF_STATICS_RX_POWER_CLEAR               =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x1008  ,
    SYS_ALARM_ETF_LINK_RX_POWER_CLEAR                  =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x1009  ,
    SYS_ALARM_ETF_LASER_IDLE_POWER_CLEAR               =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x100a  ,
    SYS_ALARM_ETF_STATICS_LLID_ERROR_CLEAR             =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x100b  ,
    SYS_ALARM_ETF_PORT_ERROR_CODE_CLEAR                =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x100c  ,
    SYS_ALARM_ETF_STANDARD_DYING_GASP_CLEAR            =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x100d  ,
    SYS_ALARM_VIF_TF_LOS_CLEAR                         =        COMMUNICATION_ALARM    |   ETF_MODULE |   0x100e  ,

    /*---------------------------------------------�����澯---------------------*/
    /*�豸ģ���ϱ�*/                        
    SYS_ALARM_DEVMAN_OLT_TEMP_HIGH_ALARM                =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x0001  ,
    SYS_ALARM_DEVMAN_OLT_TEMP_LOW_ALARM                 =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x0002  ,
    SYS_ALARM_DEVMAN_OLT_FAN_STOP                       =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x0003  ,
    SYS_ALARM_DEVMAN_OLT_CPU_HIGH_RATIO                 =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x0004  ,
    SYS_ALARM_DEVMAN_OLT_RAM_HIGH_RATIO                 =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x0005  ,
    /*�豸ģ�����*/                    
    SYS_ALARM_DEVMAN_OLT_TEMP_HIGH_ALARM_CLEAR          =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x1001  ,
    SYS_ALARM_DEVMAN_OLT_TEMP_LOW_ALARM_CLEAR           =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x1002  ,
    SYS_ALARM_DEVMAN_OLT_FAN_STOP_CLEAR                 =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x1003  ,
    SYS_ALARM_DEVMAN_OLT_CPU_HIGH_RATIO_CLEAR           =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x1004  ,
    SYS_ALARM_DEVMAN_OLT_RAM_HIGH_RATIO_CLEAR           =        ENVIRONMENT_ALARM  |   DEVMAN_MODULE   |   0x1005  ,


    /*��ģ���ϱ�*/                      
    SYS_ALARM_PORTMAN_RX_POWER_HIGH_ALARM               =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x0001  ,
    SYS_ALARM_PORTMAN_RX_POWER_LOW_ALARM                =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x0002  ,
    SYS_ALARM_PORTMAN_TX_BIAS_HIGH_ALARM                =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x0003  ,
    SYS_ALARM_PORTMAN_TX_BIAS_LOW_ALARM                 =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x0004  ,
    SYS_ALARM_PORTMAN_VCC_HIGH_ALARM                    =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x0005  ,
    SYS_ALARM_PORTMAN_VCC_LOW_ALARM                     =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x0006  ,
    SYS_ALARM_PORTMAN_TEMP_HIGH_ALARM                   =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x0007  ,
    SYS_ALARM_PORTMAN_TEMP_LOW_ALARM                    =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x0008  ,
    SYS_ALARM_PORTMAN_TX_POWER_HIGH_ALARM               =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x0009  ,
    SYS_ALARM_PORTMAN_TX_POWER_LOW_ALARM                =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x000a  ,
    /*��ģ�����*/              
    SYS_ALARM_PORTMAN_RX_POWER_HIGH_ALARM_CLEAR         =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x1001  ,
    SYS_ALARM_PORTMAN_RX_POWER_LOW_ALARM_CLEAR          =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x1002  ,
    SYS_ALARM_PORTMAN_TX_BIAS_HIGH_ALARM_CLEAR          =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x1003  ,
    SYS_ALARM_PORTMAN_TX_BIAS_LOW_ALARM_CLEAR           =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x1004  ,
    SYS_ALARM_PORTMAN_VCC_HIGH_ALARM_CLEAR              =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x1005  ,
    SYS_ALARM_PORTMAN_VCC_LOW_ALARM_CLEAR               =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x1006  ,
    SYS_ALARM_PORTMAN_TEMP_HIGH_ALARM_CLEAR             =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x1007  ,
    SYS_ALARM_PORTMAN_TEMP_LOW_ALARM_CLEAR              =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x1008  ,
    SYS_ALARM_PORTMAN_TX_POWER_HIGH_ALARM_CLEAR         =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x1009  ,
    SYS_ALARM_PORTMAN_TX_POWER_LOW_ALARM_CLEAR          =        ENVIRONMENT_ALARM  |   PORTMAN_MODULE  |   0x100a  ,

    /*---------------------------------����ʧ�ܸ澯------------------------*/

    /*-----------------------------------------------�¼�------------------------------------*/

    /*�豸����ģ��*/                        
    SYS_ALARM_DEVMAN_OLT_REBOOT                         =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0001  ,
    SYS_ALARM_DEVMAN_COLDSTART                          =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0002  ,
    SYS_ALARM_DEVMAN_WARMSTART                          =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0003  ,
    SYS_ALARM_DEVMAN_OLT_SW_UPGRADE_FAILED              =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0004  ,
    SYS_ALARM_DEVMAN_OLT_SW_UPGRADE_SUCESS              =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0005  ,
    SYS_ALARM_DEVMAN_ONU_SW_UPGRADE_FAILED              =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0006  ,
    SYS_ALARM_DEVMAN_ONU_SW_UPGRADE_SUCESS              =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0007  ,
    SYS_ALARM_DEVMAN_ONU_SW_COMMIT_FAILED               =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0008  ,
    SYS_ALARM_DEVMAN_ONU_REBOOT                         =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0009  ,
    SYS_ALARM_DEVMAN_ONU_FAULT                          =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x000a  ,
    SYS_ALARM_DEVMAN_OLT_MGMT_SWITCH                    =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x000b  ,
    SYS_ALARM_DEVMAN_OLT_POWER_SWITCH                   =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x000c  ,
    SYS_ALARM_DEVMAN_DETECT_ONU_TX_POWERHIGH            =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x000d  ,
    SYS_ALARM_DEVMAN_DETECT_ONU_TX_POWERLOW             =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x000e  ,
    SYS_ALARM_DEVMAN_OLT_FW_UPGRADE_FAILED              =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x000f  ,
    SYS_ALARM_DEVMAN_OLT_FW_UPGRADE_SUCESS              =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0010  ,
    SYS_ALARM_DEVMAN_OLT_CFG_UPGRADE_FAILED             =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0011  ,
    SYS_ALARM_DEVMAN_OLT_CFG_UPGRADE_SUCESS             =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0012  ,
    SYS_ALARM_DEVMAN_ONU_FW_UPGRADE_FAILED              =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0013  ,
    SYS_ALARM_DEVMAN_ONU_FW_UPGRADE_SUCESS              =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0014  ,
    SYS_ALARM_DEVMAN_ONU_FW_COMMIT_FAILED               =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0015  ,
    SYS_ALARM_DEVMAN_ONU_CFG_UPGRADE_FAILED             =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0016  ,
    SYS_ALARM_DEVMAN_ONU_CFG_UPGRADE_SUCESS             =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0017  ,
    SYS_ALARM_DEVMAN_ONU_CFG_COMMIT_FAILED              =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0018  ,
    SYS_ALARM_DEVMAN_ONU_SECKEY_UPDATE_FAIL             =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0019  ,
    SYS_ALARM_DEVMAN_OLT_SWPATCH_UPGRADE_SUCESS         =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x001a  ,
    SYS_ALARM_DEVMAN_OLT_SWPATCH_UPGRADE_FAILED         =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x001b  ,
    SYS_ALARM_DEVMAN_BOARD_REBOOT                       =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x001c  ,
    SYS_ALARM_DEVMAN_BOARD_READY                        =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x001d  ,
    SYS_ALARM_DEVMAN_ETF_BUSY                          =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x001e  ,
    SYS_ALARM_DEVMAN_BOARD_MISMATCH                     =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x002f  ,
    SYS_ALARM_DEVMAN_PORTRESTART                        =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0020  ,
    SYS_ALARM_DEVMAN_PROTECT_GROUP_SWITCH               =        SYSTEM_EVENT   |   DEVMAN_MODULE   |   0x0021  ,
    /*ETFģ*/                  
    SYS_ALARM_ETF_LINK_FAULT                           =        SYSTEM_EVENT   |   ETF_MODULE |   0x0001  ,
    SYS_ALARM_ETF_TR_FAULT                             =        SYSTEM_EVENT   |   ETF_MODULE |   0x0002  ,
    SYS_ALARM_ETF_TR_DEGRADE                           =        SYSTEM_EVENT   |   ETF_MODULE |   0x0003  ,
    SYS_ALARM_ETF_QUEUE_OVERFLOW                       =        SYSTEM_EVENT   |   ETF_MODULE |   0x0004  ,
    SYS_ALARM_ETF_MACTABLE_OVERFLOW                    =        SYSTEM_EVENT   |   ETF_MODULE |   0x0005  ,
    SYS_ALARM_ETF_DUPLICATE_MAC_REG                    =        SYSTEM_EVENT   |   ETF_MODULE |   0x0006  ,
    SYS_ALARM_ETF_REPORT_TIMEOUT                       =        SYSTEM_EVENT   |   ETF_MODULE |   0x0007  ,
    SYS_ALARM_ETF_GATE_TIMEOUT                         =        SYSTEM_EVENT   |   ETF_MODULE |   0x0008  ,
    SYS_ALARM_ETF_OAM_KEEPALIVE_TIMEOUT                =        SYSTEM_EVENT   |   ETF_MODULE |   0x0009  ,
    SYS_ALARM_ETF_KEY_EXCHANGE_FAIL                    =        SYSTEM_EVENT   |   ETF_MODULE |   0x000a  ,
    SYS_ALARM_ETF_AUTO_NEGOTIATION_FAIL                =        SYSTEM_EVENT   |   ETF_MODULE |   0x000b  ,
    SYS_ALARM_ETF_EXT_LINK_FAULT_ALARM                 =        SYSTEM_EVENT   |   ETF_MODULE |   0x000c  ,
    SYS_ALARM_ETF_LOOPBACK                             =        SYSTEM_EVENT   |   ETF_MODULE |   0x000d  ,
    SYS_ALARM_ETF_PORTDISABLED                         =        SYSTEM_EVENT   |   ETF_MODULE |   0x000e  ,
    SYS_ALARM_ETF_SHUTDOWN                             =        SYSTEM_EVENT   |   ETF_MODULE |   0x000f  ,
    SYS_ALARM_ETF_EXT_DYING_GASP_ALARM                 =        SYSTEM_EVENT   |   ETF_MODULE |   0x0010  ,
    SYS_ALARM_ETF_STD_CRITICAL_EVENT                   =        SYSTEM_EVENT   |   ETF_MODULE |   0x0011  ,
    SYS_ALARM_ETF_EXT_CRITICAL_EVENT                   =        SYSTEM_EVENT   |   ETF_MODULE |   0x0012  ,
    SYS_ALARM_ETF_TEMPERATURE                          =        SYSTEM_EVENT   |   ETF_MODULE |   0x0013  ,
    SYS_ALARM_ETF_EXT_ALARM                            =        SYSTEM_EVENT   |   ETF_MODULE |   0x0014  ,
    SYS_ALARM_ETF_AUTH_INFO_UNAVAIABLE                 =        SYSTEM_EVENT   |   ETF_MODULE |   0x0015  ,
    SYS_ALARM_ETF_STATIS_ALARM                         =        SYSTEM_EVENT   |   ETF_MODULE |   0x0016  ,
    SYS_ALARM_ETF_NUM_OF_LINK_EXCEED                   =        SYSTEM_EVENT   |   ETF_MODULE |   0x0017  ,
    SYS_ALARM_ETF_JUMBO_FRAME_RX                       =        SYSTEM_EVENT   |   ETF_MODULE |   0x0018  ,
    SYS_ALARM_ETF_MIX_NETWORK                          =        SYSTEM_EVENT   |   ETF_MODULE |   0x0019  ,


    /*STPģ��*/                     
    SYS_ALARM_STP_PROTOCOL_CHANGE                       =        SYSTEM_EVENT   |   STP_MODULE  |   0x0001  ,

    /*ONURAģ��*/                   
    SYS_ALARM_RA_PORT_AUTH_MODE_CHANGE                  =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x0001  ,
    SYS_ALARM_RA_ONU_AUTH_LOID_FAILED                   =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x0002  ,
    SYS_ALARM_RA_ILLEGAL_ONU_REGISTER                   =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x0003  ,
    SYS_ALARM_RA_ONU_AUTH_INFO_CONFLICT                 =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x0004  ,
    SYS_ALARM_RA_ONU_AUTHSUCESS                         =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x0005  ,
    SYS_ALARM_RA_REGISTERED_ONU_LEAVE                   =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x0006  ,
    SYS_ALARM_RA_ONU_REGISTER_SUCESS                    =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x0007  ,
    SYS_ALARM_RA_DEAUTHORISE_ONU                        =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x0008  ,
    SYS_ALARM_RA_ONU_LINKDOWN                           =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x0009  ,
    SYS_ALARM_RA_ONU_SAME_MACADDR                       =        SYSTEM_EVENT   |   ONURA_MODULE    |   0x000a  ,
    /*GTFģ��*/
    SYS_ALARM_GTF_PLAOM_RANGE_FAIL_ALARM               =        SYSTEM_EVENT   |   GTF_MODULE |   0x0001  ,
    SYS_ALARM_GTF_PLAOM_PWD_AUTH_FAIL_ALARM            =        SYSTEM_EVENT   |   GTF_MODULE |   0x0002  ,

}SYSTEM_ALARM;

/*�澯��Դ���ݽṹ*/
struct SYS_OBJECT
{     
    UINT8 slotno;          /*��λ��,�߿��д˲���Ϊ0,�κβ�����������,Ϊ������*/
    UINT8 tfno;         /* tf�ں� */
    UINT8 onuno;         /* onu��Ȩ�� */
    UINT8 unino;         /* �û��Զ��壬��onu����Ϊ�˿� */
};
/*�ϱ�instance�е���Ϣ*/
typedef struct
{
    UINT32 ulShelfId;
    UINT32 ulSlotId;
    UINT32 ulPortId;
    UINT32 ulOnuId;
    UINT32 ulOnuCard;
    UINT32 ulUni;
}REPORT_INSTANCE_INFO;

#if 000
int  reportSysAlarm( /*�ϱ��澯��Ϣ*/
                        UINT32               vindex,     /*vifunit,��ʹ��ʹ�� vifphy2log(phy,vifunit)*/
                        UINT32               subindex,   /*subvifunit��ʹ��ʹ�� subvifphy2log(subphy,subvifunit)*/
                        UINT32                 alarmID,    /*��ʹ�����ϸ澯id*/
                        UINT8                 *alarmPayload,/*�澯������Ϣ*/
                        UINT32                 payloadLen);/*������Ϣ���ȣ� strlen(alarmPayload)+1 */
#else
#define reportSysAlarm(a,b,c,d,e) 
#endif

#if 000
int  GtfreportSysAlarm( 
                        UINT32               linkid,    
                        UINT32               onuid,
                        UINT32               portid,
                        UINT32               alarmID, 
                        UINT8               *alarmPayload,
                        UINT32              payloadLen
                        );
#else
#define GtfreportSysAlarm(...) 
#endif

#endif
