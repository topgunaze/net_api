/**************************************************************
 * �ļ�����:  sys_debug.h
 * ��           ��:  keith.gong
 * ��           ��:  2015.02.26
 * �ļ�����:  ���ƹ��̵�����Ϣ��ӡ
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/

#ifndef __SYS_DEBUG_H__
#define __SYS_DEBUG_H__

enum
{
    CLI_DEBUG_MOD0 = 0,
    CLI_DEBUG_MOD_MAX
};

#ifndef CLI_DEBUG
#define CLI_DEBUG(fmt, arg...) tflog_debug(CLI_DEBUG_MOD0, fmt, ##arg)
#endif

#endif /* __SYS_DEBUG_H__ */


