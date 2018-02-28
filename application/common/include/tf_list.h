/******************************************************************************
 * FILE   : list.h
 * Description : 
 * Author : Gerhard Lao
 * Date   : 2015/12/30
 * Copyright (c) 2015-2025 by C-Data Tech. Co., Ltd. All Rights Reserved.
 ******************************************************************************/
#ifndef _LIST_H_
#define _LIST_H_

#ifdef __cplusplus
extern "C"
{
#endif 

#include <types.h>

typedef struct node_t
{
    struct node_t * previous;
    struct node_t * next;
}node;

typedef struct list_t
{
    node node;
    UINT32 count;
    INT32 (*compare)(void * , UINT32);
}list;

#define tail node.previous
#define head node.next

extern void lst_init(list * pLst , INT32 (*compare)(void * , UINT32));
extern void lst_insert(list * pLst, node * pPrevious, node * pNode);
extern void lst_add(list * pLst, node * pNode);
node *lst_remove(list * pLst , UINT32 key);
extern void lst_delete(list * pLst, node * pNode);
extern node * lst_first(list * pLst);
extern node * lst_last(list * pLst);
extern node * lst_get(list * pLst);
extern node * lst_nth(list * pLst, int nodeNum);
node *lst_find(list *pLst , UINT32 key);
extern node * lst_prev(node * pNode);
extern node * lst_next(node * pNode);
extern UINT32 lst_count(list * pLst);
extern list *lst_concat(list *pDst , list *pSrc);

#define lst_scan(pList , pNode , type) for(pNode=(type)lst_first(pList);pNode;pNode=(type)lst_next((node *)pNode))



#ifdef __cplusplus
}
#endif 


#endif /*_LIST_H_*/


