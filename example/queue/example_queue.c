#include <bv.h>
#include <stdio.h>
#include <malloc.h>

typedef struct test
{
    BV_LIST_MEMBER(struct test);
    int data;
}data, *pData;

BV_LOG_HANDLE logHandle;

void showNode(void*pvNode)
{
    bv_log_debug(logHandle,"data=%d", ((pData)pvNode)->data);
}

int main()
{
    int iLoop;
    pData pNode = NULL;
    BV_RETURN ret;
    BV_QUEUE_HANDLE queueHandle;
    BV_TIME_HANDLE timeHandle;
    char c;
    logHandle = bv_log_create("./queue.log");
    if (logHandle == 0)
    {
        return -1;
    }
    queueHandle = bv_queue_create(sizeof(data), 0);
    for (iLoop = 1; iLoop <= 1000000; iLoop++)
    {
        pNode = (pData)malloc(sizeof(data));
        if (pNode != NULL)
        {
            pNode->data = iLoop;
            bv_queue_push(queueHandle, (void*)pNode);
        }
    }
    timeHandle = bv_time_ms_start();
    bv_log_debug(logHandle, "show node start");
    ret = bv_queue_show_all_node(queueHandle, showNode);
    bv_log_debug(logHandle, "show node end ret=%d,%d", ret, bv_time_ms_end(timeHandle));
    
    timeHandle = bv_time_ms_start();
    bv_log_debug(logHandle, "bv_queue_destroy start");
    ret = bv_queue_destroy(queueHandle);
    bv_log_debug(logHandle, "bv_queue_destroy end ret=%d,%d", ret, bv_time_ms_end(timeHandle));

    scanf("%c",&c);//为了阻塞
    
    bv_log_close(logHandle);
    return 0;
}
