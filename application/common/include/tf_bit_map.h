/**************************************************************
 * �ļ�����:  tf_bit_map.h
 * ��           ��:  keith.gong
 * ��           ��:  2015.05.12
 * �ļ�����:  λͼ����
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/

#ifndef __TF_BIT_MAP_H__
#define __TF_BIT_MAP_H__

#include "tf_types.h"

/*****************************************************************************
*
* bitListSet - Set a bit in the given byte array.
*/
inline void bitListSet    (    UINT8   * bitList,    UINT32     bit    );

/*****************************************************************************
*
*bitListClr - Remove a bit in the given byte array.
*/
inline void bitListClr    (    UINT8   * bitList,    UINT32     bit    );

/*****************************************************************************
*
* bitListTst - Return True if the bit is set, else False.
*/
inline UINT8 bitListTst    (    UINT8   * bitList,    UINT32     bit    );

/*****************************************************************************
*
* bitListTstAll - Return >0 if a bit is set in the list, else zero.
*/
UINT32 bitListTstAll    (    UINT8   * bitList,    UINT32     bitListLen    );

#endif /* __TF_BIT_MAP_H__ */

