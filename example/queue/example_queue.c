#include <bv.h>
#include <stdio.h>
#include <malloc.h>

typedef struct test
{
    BV_NODE_MEMBER(struct test);
    int data;
}data, *pData;

BV_LOG_HANDLE logHandle;

void showNode(void*pvNode)
{
    bv_log_info(logHandle,"data=%d", ((pData)pvNode)->data);
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
        printf("bv_log_create failed\n");
        return -1;
    }
    //设置log文件最大存储8M
    bv_log_set_storage_block_size(logHandle, 8);
    bv_log_set_level(logHandle, BV_LOG_LEVEL_INFO);
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
    bv_log_info(logHandle, "show node start");
    ret = bv_queue_show_all_node(queueHandle, showNode);
    bv_log_info(logHandle, "show node end ret=%d,%d", ret, bv_time_ms_end(timeHandle));
    
    timeHandle = bv_time_ms_start();
    bv_log_info(logHandle, "bv_queue_destroy start");
    ret = bv_queue_destroy(queueHandle);
    bv_log_info(logHandle, "bv_queue_destroy end ret=%d,%d", ret, bv_time_ms_end(timeHandle));

    printf("input any key to exit\n");
    scanf("%c",&c);//为了阻塞
    
    bv_log_close(logHandle);
    return 0;
}
