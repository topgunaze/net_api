/**************************************************************
 * �ļ�����:  tf_string_opera.c
 * ��           ��:  keith.gong
 * ��           ��:  2015.05.12
 * �ļ�����:  �ַ�������
 * ��           ��:  V1.00
 * �޸���ʷ:  
 *     <�޸���>   <ʱ��>    <�汾 >   <����>
**************************************************************/


#include <unistd.h>
#include <sys/msg.h>
#include <errno.h>

#include "tf_types.h"


/**************************************************************
 * ��������: tf_str_2_id_array
 * ��           ��: keith.gong
 * ��           ��: 2015��03��30��
 *
 * ��������: 
                            ���ַ���(1-7,2,9)ת���������У�
                            
 * �������: 
                            char  *str, 
                            unsigned int *idArry, 
                            unsigned int  idNum,
                            unsigned int  idMin,
                            unsigned int  idMax,
 * �������: 
                            unsigned int *idNumGet
 * ������ֵ: 
                            �ɹ�����0
 * ��           ��: 
                            ע�⣬�ú������޸�str
**************************************************************/
int 
tf_str_2_id_array(
                char  *str, 
                unsigned int *pIdArray, 
                const unsigned int idNum,
                const unsigned int idMin,
                const unsigned int idMax,
                unsigned int *pGetIdNum)
{
    char *p;
    unsigned int index, count = 0, pstart, pend;
    unsigned int end, start, buf[4095] = {0};

    if(pIdArray == NULL || str == NULL || idNum > sizeof(buf)/sizeof(buf[0]))
        return -1;  /* ��������*/

    p = strtok(str, ", ");
    while(p)
    {
        if((start = atoi(p)) < idMin || start > idMax)
            return -1; /* �������*/
        
        /* ��������*/
        for(index = 0; index < count; index++)
        {
            if(buf[index] > start)
            {
                if(count + 1 > idNum)
                    return -2;  /* ����*/

                memmove(buf + index + 1, buf + index, (count - index) * sizeof(buf[0]));
                buf[index] = start;
                count++;
                break;
            }
            else if(buf[index] == start)
                break;
        }

        /* ���ֵ����λ����*/
        if(index == count)
        {
            if(count + 1 > idNum)
                return -2;  /* ����*/
            
            buf[count++] = start;
        }

        while(*++p)
        {
            if(*p == '-')
            {          
                if((end = atoi(++p)) < idMin || end < start || end > idMax)
                    return -1;  /* �������*/

                pstart = pend = 0;
                for(index = 0; index < count; index++)
                {
                    if(buf[index] == start)
                    {
                        pstart = index;
                    }
                    
                    if(buf[index] > end)
                    {
                        pend = index;
                        break;
                    }
                }
                
                if(index != count)
                {
                    if(count + count - pend > idNum)
                        return -2;  /* ����*/
                    
                    memmove(buf + pstart + end - start + 1, buf + pend, (count - pend) * sizeof(buf[0]));
                }

                index = pstart;
                count += (end-start) - ((pend?pend:count) - pstart - 1);
                if(count > idNum)
                    return -2;  /* ����*/

                while(start < end) 
                    buf[++index] = ++start;
            }
            else if(*p > '9' || *p < '0')
                return -1;  /* �������*/

        }
        p = strtok(NULL, ", ");
    }

    memcpy(pIdArray, buf, count * sizeof(buf[0]));
    *pGetIdNum = count;

    return 0;
}


/**************************************************************
 * ��������: port_id_array_2_str
 * ��           ��: keith.gong
 * ��           ��: 2015��05��26��
 *
 * ��������: 
                            ������ת��Ϊ�ַ���(1,2,5-7)
                            
 * �������: 
                            const unsigned int *pIdArray,      ����ָ��
                            const unsigned int idNum,            ����Ԫ�ظ���
                            const unsigned char sortFlag,       �����ʶ
                            const unsigned int strLen,            ����ַ�������������
                            const unsigned char * prefix,       ����ַ���ǰ׺������ʹ�ÿ������β�"" ��
 * �������: 
                            unsigned char * const str,            ����ַ���������
 * ������ֵ: 
                            �ɹ�����0
 * ��           ��: 
                            ע�⣬�ú���δ�������Ԫ�غϷ���!!
**************************************************************/
int 
port_id_array_2_str(
                const unsigned int *pIdArray, 
                const unsigned int idNum,
                const unsigned char sortFlag,
                unsigned char * const str,
                const unsigned int strLen,
                const char * prefix)

{
    unsigned int index;
    unsigned char buf[4096];
    unsigned char offset;
    unsigned int end, start;
    int diff;

    if(pIdArray == NULL || idNum < 1 || str == NULL || strLen > sizeof(buf))
        return -1;  /* ��������*/

    if(sortFlag)
    {
        /* kth.debug  ����������*/
        return -2;
    }

    start = pIdArray[0];
    offset = snprintf(buf, sizeof(buf), "%s%d", prefix, start);
    for(index = 1; index < idNum; index++)
    {
        if(pIdArray[index-1] == pIdArray[index] - 1)
            continue;

        end = pIdArray[index-1];
        diff = end - start;
        if(diff == 1)
            offset += snprintf(&buf[0]+offset, sizeof(buf)-offset, ",%s%d,%s%d", prefix, end, prefix, pIdArray[index]);
        else if(diff > 1)
            offset += snprintf(&buf[0]+offset, sizeof(buf)-offset, "-%s%d,%s%d", prefix, end, prefix, pIdArray[index]);  
        else if(diff == 0)
            offset += snprintf(&buf[0]+offset, sizeof(buf)-offset, ",%s%d", prefix, pIdArray[index]);  
        start = pIdArray[index];
    }
    end = pIdArray[index-1];
    if(start != end)
    {
        diff = end - start;
        if(diff == 1)
            offset += snprintf(&buf[0]+offset, sizeof(buf)-offset, ",%s%d", prefix, end);
        else if(diff > 1)
            offset += snprintf(&buf[0]+offset, sizeof(buf)-offset, "-%s%d", prefix, end);  
        else if(diff == 0)
            offset += snprintf(&buf[0]+offset, sizeof(buf)-offset, ",%s%d", prefix, end);  
        start = pIdArray[index];	
    }
    
    snprintf(str, strLen, "%s", buf);

    return 0;
}

/*************************************************************
 * ltrim - ɾ����ߵĿո�
 * 
 * str[in]: ��Ҫȥ���յĵ��ַ���
 *
 * Returns: ����ָ��ԭ�ַ�����������ָ��,�����������,����NULL;
 *
 ************************************************************/
char *ltrim(char *str)
{
    if (str == NULL)
        return NULL;

    while(*str != '\0' && *str == ' ')
        str++;

    return str;
}

/*************************************************************
 * rtrim - ɾ���ұߵĿո�
 * ע��:���޸�str����
 * 
 * str[in]: ��Ҫȥ���յĵ�Դ�ַ���
 *
 * Returns: ����ָ��ԭ�ַ�����������ָ��,�����������,����NULL;
 *
 ************************************************************/
char *rtrim(char *str)
{
    char *end = NULL;
    
    if (str == NULL)
        return NULL;

    end = str + strlen(str) - 1;
    while(str != end && *end == ' ')
    {
        *end = '\0';
        end--;
    }

    return str;
}

/*************************************************************
 * lstripNewLine - ɾ����ߵĻ��з�
 * 
 * str[in]: ��Ҫȥ�����з����ַ���
 *
 * Returns: ����ָ��ԭ�ַ�����������ָ��,�����������,����NULL;
 *
 ************************************************************/
char *lstripNewLine(char *str)
{
    if (str == NULL)
        return NULL;

    while((*str != '\0') && (*str == '\r' || *str == '\n'))
    {
        str++;
    }

    return str;
}

/*************************************************************
 * rstripNewLine - ɾ���ұߵĻ��з�
 * ע��:���޸�str����
 * 
 * str[in]: ��Ҫȥ�����з����ַ���
 *
 * Returns: ����ָ��ԭ�ַ�����������ָ��,�����������,����NULL;
 *
 ************************************************************/
char *rstripNewLine(char *str)
{
    char *end = NULL;
    
    if (str == NULL)
        return NULL;

    end = str + strlen(str) - 1;
    while((end != str) && (*end == '\r' || *end == '\n'))
    {
        *end = '\0';
        end--;
    }

    return str;
}


