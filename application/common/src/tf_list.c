/******************************************************************************
 * FILE   : list.c
 * Description : 
 * Author : Gerhard Lao
 * Date   : 2015/12/30
 * Copyright (c) 2015-2025 by C-Data Tech. Co., Ltd. All Rights Reserved.
 ******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif 

#include "list.h"

void lst_init(list * pLst , INT32 (*compare)(void * , UINT32))
{
    pLst->head = NULL;
    pLst->tail = NULL;
    pLst->count = 0;
    pLst->compare = compare;
}

/**********************************************************
Description:
   insert a new node 'pNode' after  the specific node  'pPrevious'
**********************************************************/
void lst_insert(list * pLst, node * pPrevious, node * pNode)
{
    node * pNext = NULL;

    if (NULL == pPrevious) {
        pNext = pLst->head;
        pLst->head = pNode;
    } else {
        pNext = pPrevious->next;
        pPrevious->next = pNode;
    }

    if (NULL == pNext) {
        pLst->tail = pNode;
    } else {
        pNext->previous = pNode;
    }

    pNode->previous = pPrevious;
    pNode->next = pNext;

    pLst->count++;
}

/**********************************************************
Description:
   add a new node to the tail of 'pLst'
**********************************************************/
void lst_add(list * pLst, node * pNode)
{
    lst_insert(pLst, pLst->tail, pNode);
}

/**********************************************************
Description:
   Find the actual node according to the 'key' and remove it from 'pLst',
   The operatoin doesn't mean to free the node , it is only cut off from 'pLst'
**********************************************************/
node *lst_remove(list * pLst , UINT32 key)
{
    node *remove_node = (node *)NULL;

    if (pLst->compare == NULL) {
        return (node *)remove_node;
    }

    lst_scan(pLst , remove_node , node *) {
        if (pLst->compare((void *)remove_node , key) == 0) {
            lst_delete(pLst , remove_node);
            return (node *)remove_node;
        }
    }

    return (node *)NULL;
}

/**********************************************************
Description:
   The operatoin doesn't mean to free the node , it is only cut off from 'pLst'
**********************************************************/
void lst_delete(list * pLst, node * pNode)
{
    if (pNode->previous == NULL) {
        pLst->head = pNode->next;
    } else {
        pNode->previous->next = pNode->next;
    }

    if (pNode->next == NULL) {
        pLst->tail = pNode->previous;
    } else {
        pNode->next->previous = pNode->previous;
    }

    pLst->count--;
}

node * lst_first(list * pLst)
{
    return pLst->head;
}

node * lst_last(list * pLst)
{
    return pLst->tail;
}

/**********************************************************
Description:
   get the pointer of the first node in the list , and the node will be cutted off
   from the header of the list
**********************************************************/
node * lst_get(list * pLst)
{
    node * pNode = NULL;

    pNode = pLst->head;

    if (pNode != NULL) {
        pLst->head = pNode->next;

        if (pNode->next == NULL) {
            pLst->tail = NULL;
        } else {
            pNode->next->previous = NULL;
        }

        pLst->count--;
    }

    return pNode;
}


node * lst_nth(list * pLst, int nodeNum)
{
    node * pNode = NULL;

    if (nodeNum < 1 || nodeNum > pLst->count) {
        return NULL;
    }

    if (nodeNum < ((pLst->count + 1) >> 1)) {
        pNode = pLst->head;

        while (--nodeNum > 0) {
            pNode = pNode->next;
        }
    } else {
        nodeNum -= pLst->count;
        pNode = pLst->tail;

        while (nodeNum++ < 0) {
            pNode = pNode->previous;
        }
    }

    return pNode;
}

/**********************************************************
Description:
   Get the pointer of the actual node according to the 'key'
**********************************************************/
node *lst_find(list *pLst , UINT32 key)
{
    node *find_node = NULL;

    if (pLst->compare == NULL) {
        return (node *)find_node;
    }

    lst_scan(pLst , find_node , node *) {
        if (pLst->compare((void *)find_node , key) == 0)
            return (node *)find_node;
    }

    return (node *)NULL;
}

node * lst_prev(node * pNode)
{
    return pNode->previous;
}

node * lst_next(node * pNode)
{
    return pNode->next;
}

UINT32 lst_count(list * pLst)
{
    return pLst->count;
}

list *lst_concat(list *pDst , list *pSrc)
{
    if(pDst == NULL || pSrc == NULL){
        return NULL;
    }

    if(pDst->compare != pSrc->compare) {
        return NULL;
    }

    if(pDst->count == 0) {
        memcpy(pDst , pSrc , sizeof(list));
        return (list *)pDst;
    }

    if(pSrc->count == 0) {
        return (list *)pDst;
    }

    pDst->tail->next = pSrc->head;
    pSrc->head->previous = pDst->tail;    
    pDst->tail = pSrc->tail;
    
    pDst->count += pSrc->count;

    lst_init(pSrc , pSrc->compare);

    return (list *)pDst;
}




#ifdef __cplusplus
}
#endif 


