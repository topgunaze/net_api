/*
 *
 * keith.gong
 *
 * 2014/03/06 - ?
 *
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <types.h>

#define QUEUE_NO_WAIT                      (0)
#define QUEUE_WAIT_FOREVER              (-1)


typedef enum
{
    QUEUE_SWITCH_IGMP,
    QUEUE_SWITCH_RSTP,
    QUEUE_RSTP_TIMER,

    QUEUE_NUM,
}tfQueueSubKey;


typedef enum
{
    SW_MSG_ANY = 0,
    SW_MSG_ARP,
    SW_MSG_DHCP,
    SW_MSG_IGMP,
    SW_MSG_RSTP,
    SW_MSG_LACP,
    SW_MSG_DOT1X,
    SW_MSG_PPPOEPLUS,

	SW_MSG_MAX,
}tfSwQueueMsgType;


#define SW_MSG_TEXT_SIZE                1550
#define SW_MSG_MAX_NUM                  30
#define DHCP_PKT_LEN_MAX 900     /*���͸�DHCP ģ�����Ϣ����*/
#define DHCP_PKT_LEN_LIMIT 800   /*���յ���DHCP������󳤶�*/
#define DHCP_PKT_LEN_EXPEND 100     /*���͸�DHCP ģ�����Ϣ����=pkt_len+DHCP_PKT_LEN_EXPEND*/

typedef struct
{
    long                                    type;
    UINT8                                   srcPort;
    UINT16                                  pktLen;
    UINT16                                  llid;
    UINT16                                  devId;
    UINT16                                  vid;
    UINT16                                  onuId;
    UINT8                                   igmpMode;
}tfSwitchIgmpMsgHeader_t;

typedef struct
{
    long        type;
    INT8        port;
    INT8        trunk;
    UINT8       noVlan;
    UINT16      vid;
    UINT16      len;
    UINT8       data[SW_MSG_TEXT_SIZE];
}tfRstpBpduMsg_t;

typedef struct
{
    tfSwitchIgmpMsgHeader_t                hdr;
    UINT8                                   data[SW_MSG_TEXT_SIZE];
}tfSwitchIgmpMsg_t;


typedef struct
{
    long type;
    UINT8 tfId;
    UINT8 onuId;
    UINT8 llid;
    UINT8 uniId;
    UINT8 srcMac[6];
    UINT8 srcport;
    UINT8 direction;                /*���ķ���;client->server/server->client*/
    UINT32 pktLen;  		/*���ĳ���*/
    UINT16 ipHeadOffset;   /*IP ͷ��ƫ��ֵ*/
    UINT16 vid;
    UINT32  dhcpOffset;            /*dhcp���ĵ�ƫ��ֵ*/
}tfSwitchDhcpMsgHeader_t;

typedef struct
{
    tfSwitchDhcpMsgHeader_t hdr;
    UINT8 data[SW_MSG_TEXT_SIZE];
}tfSwitchDhcpMsg_t;

typedef struct
{
	long                                    type;
	UINT8                                   srcPort;
	UINT16                                  pktLen;
	UINT16                                  vid;
}tfSwitchArpMsgHeader_t;

typedef struct
{
    tfSwitchArpMsgHeader_t hdr;
    UINT8 data[SW_MSG_TEXT_SIZE];
}tfSwitchArpMsg_t;

typedef struct
{
    long        type;       /* SW_MSG_LACP */
    UINT8       srcPort;    /* ��ӿ����� */
    UINT16      pktLen;
    UINT16      vid;        /* ʵ�ʲ��漰 */
}tfSwitchLacpMsgHeader_t;

typedef struct
{
    tfSwitchLacpMsgHeader_t hdr;
    UINT8 data[SW_MSG_TEXT_SIZE];
}tfSwitchLacpMsg_t;

typedef struct
{
    long        type;       /* SW_MSG_DOT1X */
    UINT8       srcPort;    /* ��ӿ����� */
    UINT16      pktLen;
    UINT16      vid;        /* ��ʱ������ */
}tfSwitchDot1xMsgHeader_t;

typedef struct
{
    tfSwitchDot1xMsgHeader_t hdr;
    UINT8 data[SW_MSG_TEXT_SIZE];
}tfSwitchDot1xMsg_t;

typedef struct
{
    long        type;       /* SW_MSG_DOT1X */
    UINT8       srcPort;    /* ��ӿ����� */
    UINT16      pktLen;
    UINT16      vid;        /* ��ʱ������ */
}tfSwitchPplusMsgHeader_t;

typedef struct
{
    tfSwitchPplusMsgHeader_t hdr;
    UINT8 data[SW_MSG_TEXT_SIZE];
}tfSwitchPplusMsg_t;

INT32 tfQueueCreate (INT32 *queue_id, INT32 subKey, UINT32 queue_size);
INT32 tfQueueGet (INT32 queue_id, long type, void *data, UINT32 size, UINT32 *size_copied, INT32 timeout);
INT32 tfQueuePut (INT32 queue_id, void *data, UINT32 size, INT32 timeout);


#endif

