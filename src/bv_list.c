#include <bv_list.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct BV_LISTHEAD
{
    BV_LIST_MEMBER(struct BV_LISTHEAD);
    unsigned int uiListLength;
    unsigned long ulItemSize;
    int itemCount;
}stListHead, *pstListHead;

BV_LIST_HANDLE bv_list_create(unsigned long ulItemSize, unsigned int uiListLength)
{
    pstListHead pListHead = NULL;
    if (ulItemSize == 0)
    {
        printf("%s(%d):BV_PARAM_ERROR\n", __FUNCTION__, __LINE__);
        return BV_PARAM_ERROR;
    }
    pListHead = (pstListHead)malloc(sizeof(stListHead));
    pListHead->prev = pListHead;
    pListHead->next = pListHead;
    pListHead->uiListLength = uiListLength;
    pListHead->ulItemSize = ulItemSize;
    pListHead->itemCount = 0;

    //返回链表头指针
    printf("%s(%d):pListHead=%p, ulItemSize=%d\n", __FUNCTION__, __LINE__, pListHead, ulItemSize);
    return (BV_LIST_HANDLE)pListHead;
}


BV_RETURN bv_list_destroy(BV_LIST_HANDLE listHandle)
{
    pstListHead pListHead = (pstListHead)listHandle;
    pstListHead pListNode = pListHead->next;
    if (pListHead == NULL)
    {
        printf("%s(%d):BV_PARAM_ERROR\n", __FUNCTION__, __LINE__);
        return BV_PARAM_ERROR;
    }
    printf("%s(%d):itemCount=%d\n", __FUNCTION__, __LINE__, pListHead->itemCount);
    for (pListNode; pListNode != pListHead; pListNode = pListHead->next)
    {
        pListNode->prev->next = pListNode->next;
        pListNode->next->prev = pListNode->prev;
        free(pListNode);
        pListHead->itemCount--;
        printf("%s(%d):itemCount=%d\n", __FUNCTION__, __LINE__);
    }
    free(pListHead);
    return BV_SUCCESS;
}

BV_RETURN bv_list_insert_node_from_head(BV_LIST_HANDLE listHandle, void *pvItem)
{
    pstListHead pListHead = (pstListHead)listHandle;
    pstListHead pListNode = (pstListHead)pvItem;
    if (pListHead == NULL || pListNode == NULL)
    {
        printf("%s(%d):BV_PARAM_ERROR\n", __FUNCTION__, __LINE__);
        return BV_PARAM_ERROR;
    }
    if (pListHead->uiListLength != 0 && pListHead->itemCount == pListHead->uiListLength)
    {
        printf("%s(%d):BV_LIST_OVER %d %d\n", __FUNCTION__, __LINE__, pListHead->itemCount, pListHead->uiListLength);
        return BV_LIST_OVER;
    }
    pListHead->next->prev = pListNode;
    pListNode->next = pListHead->next;
    pListHead->prev = pListHead;
    pListHead->next = pListNode;
    pListHead->itemCount++;
    printf("%s(%d):pListNode=%p itemCount=%d\n", __FUNCTION__, __LINE__, pListNode, pListHead->itemCount);
    return BV_SUCCESS;
}

BV_RETURN bv_list_insert_node_from_tail(BV_LIST_HANDLE listHandle, void *pvItem)
{
    pstListHead pListHead = (pstListHead)listHandle;
    pstListHead pListNode = (pstListHead)pvItem;
    if (pListHead == NULL || pListNode == NULL)
    {
        printf("%s(%d):BV_PARAM_ERROR\n", __FUNCTION__, __LINE__);
        return BV_PARAM_ERROR;
    }
    if (pListHead->uiListLength != 0 && pListHead->itemCount == pListHead->uiListLength)
    {
        printf("%s(%d):BV_LIST_OVER %d %d\n", __FUNCTION__, __LINE__, pListHead->itemCount, pListHead->uiListLength);
        return BV_LIST_OVER;
    }
    pListHead->prev->next = pListNode;
    pListNode->prev = pListNode;
    pListNode->next = pListHead;
    pListHead->prev = pListNode;
    pListHead->itemCount++;
    printf("%s(%d):pListNode=%p itemCount=%d\n", __FUNCTION__, __LINE__, pListNode, pListHead->itemCount);
    return BV_SUCCESS;
}

BV_RETURN bv_list_get_node_from_head(BV_LIST_HANDLE listHandle, void **ppvItem)
{
    pstListHead pListHead = (pstListHead)listHandle;
    pstListHead pListNode = pListHead->next;
    if (pListHead == NULL)
    {
        printf("%s(%d):BV_PARAM_ERROR\n", __FUNCTION__, __LINE__);
        return BV_PARAM_ERROR;
    }
    if (pListNode == pListHead)
    {
        return BV_NO_ITEM;
    }

    pListNode->next->prev = pListHead;
    pListHead->next = pListNode->next;
    *(ppvItem) = (void*)pListNode;
    pListHead->itemCount--;
    printf("%s(%d):pListNode=%p itemCount=%d\n", __FUNCTION__, __LINE__, pListNode, pListHead->itemCount);
    return BV_SUCCESS;
}

BV_RETURN bv_list_get_node_from_tail(BV_LIST_HANDLE listHandle, void **ppvItem)
{
    pstListHead pListHead = (pstListHead)listHandle;
    pstListHead pListNode = pListHead->prev;
    if (pListHead == NULL)
    {
        printf("%s(%d):BV_PARAM_ERROR\n", __FUNCTION__, __LINE__);
        return BV_PARAM_ERROR;
    }
    if (pListNode == pListHead)
    {
        return BV_NO_ITEM;
    }
    pListNode->prev->next = pListHead;
    pListHead->prev = pListNode->prev;
    *(ppvItem) = (void *)pListNode;
    pListHead->itemCount--;
    printf("%s(%d):pListNode=%p itemCount=%d\n", __FUNCTION__, __LINE__, pListNode, pListHead->itemCount);
    return BV_SUCCESS;
}

BV_RETURN bv_list_delete_node(BV_LIST_HANDLE listHandle, void *pvItem)
{
    pstListHead pListHead = (pstListHead)listHandle;
    pstListHead pListNode = (pstListHead)pvItem;
    if (pListHead == NULL || pListNode == NULL)
    {
        printf("%s(%d):BV_PARAM_ERROR\n", __FUNCTION__, __LINE__);
        return BV_PARAM_ERROR;
    }
    if (pListNode == pListHead)
    {
        return BV_NO_ITEM;
    }
    pListNode->prev->next = pListNode->next;
    pListNode->next->prev = pListNode->prev;
    free(pListNode);
    pListHead->itemCount--;
    printf("%s(%d):pListNode=%p itemCount=%d\n", __FUNCTION__, __LINE__, pListNode, pListHead->itemCount);
    return BV_SUCCESS;
}


