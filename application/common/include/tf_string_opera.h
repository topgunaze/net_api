/**************************************************************
 * �ļ�����:  tf_string_opera.h
 * ��           ��:  keith.gong
 * ��           ��:  2015.05.12
 * �ļ�����:  �ַ�������
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>     <ʱ��>      <�汾 >   <����>
        keith.gong          2015.05.12      V1.00           Create
**************************************************************/

#ifndef __TF_STRING_OPERA_H__
#define __TF_STRING_OPERA_H__

int 
tf_str_2_id_array(
                char  *str, 
                unsigned int *pIdArray, 
                const unsigned int idNum,
                const unsigned int idMin,
                const unsigned int idMax,
                unsigned int *pGetIdNum);

int 
port_id_array_2_str(
                const unsigned int *pIdArray, 
                const unsigned int idNum,
                const unsigned char sortFlag,
                unsigned char * const str,
                const unsigned int strLen,
                const char * prefix);


char *ltrim(char *str);
char *rtrim(char *str);
char *lstripNewLine(char *str);
char *rstripNewLine(char *str);

#endif


