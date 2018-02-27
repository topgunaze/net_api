/**************************************************************
 * �ļ�����:  tf_bit_map.c
 * ��           ��:  keith.gong
 * ��           ��:  2015.05.12
 * �ļ�����:  λͼ����
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/


#include "tf_bit_map.h"


static const UINT8 bitMask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

/*****************************************************************************
*
* bitListSet - Set a bit in the given byte array.
*/
inline void bitListSet    (    UINT8   * bitList,    UINT32     bit    )
{
    bitList[bit/8] |= bitMask[bit%8];
}

/*****************************************************************************
*
*bitListClr - Remove a bit in the given byte array.
*/
inline void bitListClr    (    UINT8   * bitList,    UINT32     bit    )
{    
    bitList[bit/8] &= ~bitMask[bit%8];
}

/*****************************************************************************
*
* bitListTst - Return True if the bit is set, else False.
*/
inline UINT8 bitListTst    (    UINT8   * bitList,    UINT32     bit    )
{    
    return (bitList[bit/8] & bitMask[bit%8])? 1 : 0;
}

/*****************************************************************************
*
* bitListTstAll - Return >0 if a bit is set in the list, else zero.
*/
UINT32 bitListTstAll    (    UINT8   * bitList,    UINT32     bitListLen    )
{    
    UINT32 i, found = 0, byteCount;
    
    /* Figure out the real number of bytes to scan */
    byteCount = ((bitListLen%8) == 0) ? (bitListLen/8) : (bitListLen/8) + 1;    
    
    /* Add up all of the bytes to determine if the bitList is empty. */    
    for( i = 0; i<byteCount; i++ )        
        found += bitList[i];
    
    return found;
}

