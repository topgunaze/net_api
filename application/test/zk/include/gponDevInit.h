/**************************************************************
 * 文件名称:  
 * 作           者:  
 * 日           期:  
 * 文件描述:  
 * 版           本:  
 * 修改历史:  
 *     <修改者>   <时间>    <版本 >   <描述>
**************************************************************/


#ifndef __GDEV_INIT_H__
#define __GDEV_INIT_H__

#include "sys_debug.h"


UINT32 
process_gcmd_task(
                char  *pMsgOut,
                UINT32 ulMsgLen,
                UINT8  ucMsgType,
                UINT8  ucSrcMo);


#endif /* __GDEV_INIT_H__ */

