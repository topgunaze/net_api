/**************************************************************
 * �ļ�����:  tfMacCryptExpo.h
 * ��    ��:  tonson tang
 * ��    ��:  2015.05.11
 * �ļ�����:  �豸MAC����У��ӿ�
 * ��    ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
 
 -------------------------------------------------------------
 * tfKeyCheck����ʹ��˵��
 *
 * ���룺devId  CData�豸ID��
 * ��������  int tfKeyCheck(unsigned int devId, unsigned char mac[6], unsigned char secret[16]);
 * ����˵��  tfKeyCheck()���ݲ���devId��macͨ�������㷨�����new_secret,�����secret���бȽ�,
             new_secret��secretһ�±�ʾMACУ��ɹ�,����1;���򷵻�0��ʾMACУ��ʧ��
 * �������  devId    CData�豸��ID��
             mac      ����ʱд����豸MAC��ַ
             secret   ����ʱд����豸MAC��ַ��Ӧ��16�ֽڳ��ȵ���Կ
 *   ����ֵ  MACУ��ɹ�����1;���򷵻�0,��ͨ������secret�����豸���¼������new_secret

 *
 * CData��Ʒ��Ӧ���豸ID��:
 *      Product             Device ID
        W1014MA             0x05010201
        FD1108S             0x01060201
        FD1104S             0x01070201
        FD1104B             0x01070501
        FD1104Y             0x01070801
        FD1508GS            0x04010101
	FD1216S             0x04010101  
**************************************************************/

#ifndef __TF_MAC_SECRET_EXPO_H__
#define __TF_MAC_SECRET_EXPO_H__

/********************************************************************/
/* tfKeyCheck()       16bytes secret                               */
/* INPUT: device id, mac address, secret key                        */
/* OUTPUT: check OK return 1, otherwise return 0;                   */
/* TESTING:                                                         */
/*   device id:  0x01020304                                         */
/*   mac address:  e0:67:b3:01:02:03                                */
/*   secret key: c1 0a 70 27 ce 2b f3 df 82 99 72 4f 46 85 4c cf    */
/********************************************************************/
int tfKeyCheck(unsigned int devId, unsigned char mac[6], unsigned char secret[16]);


/********************************************************************/
/* tfShortKeyCheck()      4bytes secret                            */
/* INPUT: device id, mac address, short secret key                  */
/* OUTPUT: check OK return 1, otherwise return 0                    */
/* TESTING:                                                         */
/*   device id:  0x01020304                                         */
/*   mac address:  e0:67:b3:01:02:03                                */
/*   secret key: c1 0a 70 27 ce 2b f3 df                            */
/********************************************************************/
int tfShortKeyCheck(unsigned int devId, unsigned char mac[6], unsigned char secret[4]);

/********************************************************************/
/* tfMacLicenseCheck()       16bytes secret                        */
/* INPUT: NULL                                                      */
/* OUTPUT: license OK return 0, license Error return 2,             */
/*           otherwise return -1;                                   */
/* FUNCTION:                                                        */
/*      ��ȡNVRAM����д��MAC��License����У��,У��ͨ������0;        */
/*      У�鲻ͨ������2;�������󷵻�-1.                             */
/********************************************************************/
int tfMacLicenseCheck(void);


/********************************************************************/
/* bootFirmwareSuccessFlag()                                        */
/* INPUT: NULL                                                      */
/* OUTPUT: success return 0, otherwise return -1;                   */
/* FUNCTION:                                                        */
/*      ��������������                                            */
/*                                                                  */
/********************************************************************/
int bootFirmwareSuccessFlag(void);


/********************************************************************/
/* bootFirmwareFailedFlag()                                         */
/* INPUT: NULL                                                      */
/* OUTPUT: success return 0, otherwise return -1;                   */
/* FUNCTION:                                                        */
/*      ����쳣�������                                            */
/*                                                                  */
/********************************************************************/
int bootFirmwareFailedFlag(void);


/********************************************************************/
/* tfValidateManuUser()                                            */
/* INPUT: NULL                                                      */
/* OUTPUT: check ok return 1, otherwise return 0;                   */
/* FUNCTION:                                                        */
/*      ����ģʽ�û�������֤�ӿ�                                    */
/*                                                                  */
/********************************************************************/
int tfValidateManuUser(char *pPasswd);


#endif /* __TF_MAC_SECRET_EXPO_H__ */
