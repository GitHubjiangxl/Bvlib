#include <bv_queue.h>
#include <bv_list.h>


BV_QUEUE_HANDLE bv_queue_create(unsigned long ulItemSize, unsigned int uiQueueLength)
{
    return (BV_QUEUE_HANDLE)bv_list_create(ulItemSize, uiQueueLength);
}

BV_RETURN bv_queue_destroy(BV_QUEUE_HANDLE queueHandle)
{
    return bv_list_destroy((BV_LIST_HANDLE)queueHandle);
}

BV_RETURN bv_queue_push(BV_QUEUE_HANDLE queueHandle, void* pvItem)
{
    return bv_list_insert_node_from_tail((BV_LIST_HANDLE)queueHandle, pvItem);
}

BV_RETURN bv_queue_pop(BV_QUEUE_HANDLE queueHandle, void** ppvItem)
{
    return bv_list_get_node_from_head((BV_LIST_HANDLE)queueHandle, ppvItem);
}

void* bv_queue_find_node(BV_QUEUE_HANDLE queueHandle, pCompareFunc pCompareFun, void *pvItem)
{
    return bv_list_find_node((BV_LIST_HANDLE)queueHandle, pCompareFun, pvItem);
}

BV_RETURN bv_queue_get_node_num(BV_QUEUE_HANDLE queueHandle, int* piNodeNum)
{
    return bv_list_get_node_num((BV_LIST_HANDLE)queueHandle, piNodeNum);
}

BV_RETURN bv_queue_show_all_node(BV_QUEUE_HANDLE queueHandle, pShowNodeFunc pShowNodeFun)
{
    return bv_list_show_all_node((BV_LIST_HANDLE)queueHandle, pShowNodeFun);
}

