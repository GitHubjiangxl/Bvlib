/********************************************************************************************
@author Bevis
@date 2018-09-04

********************************************************************************************/

#include <bv_log.h>
#include <bv_time.h>
#include <bv_thread_pool.h>
#include <bv_list.h>
#include <malloc.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define LOG_LEVEL_TAG 8

#define LOG_FILE_STORAGE_DEFAULT_SIZE_M 8   //LOG文件默认上限为8M
#define LOG_DEFAULT_DEAL_THREAD_NUM 1       //默认的处理线程数量

static char* log_level_tag[BV_LOG_LEVEL_ALL] = 
{
        "[FATAL]:",
        "[ERROR]:",
        "[ WARN]:",
        "[ INFO]:",
        "[DEBUG]:",
        "[TRACE]:",
};

typedef struct logHead
{
    FILE *FileId;                               //文件句柄
    unsigned int uiFileCount;                   //文件数量
    unsigned long ulFileSize;                   //存储文件大小
    unsigned int uiBlockSize;                 //文件存储上限
    unsigned int uiThreadCount;                 //创建线程池中线程的数量
    BV_THREADPOOL_HANDLE LogThreadPoolHandle;   //线程池句柄
    BV_LOG_LEVEL LogLevel;                      //日志级别
}stLogHead, *pstLogHead;

typedef struct logInfo
{
    BV_LIST_MEMBER(struct logInfo);
    pstLogHead logHead;
    char log[BV_TIME_INFO_LENGTH + LOG_LEVEL_TAG + BV_LOG_INFO_LENGTH + 1];//+1 '\n'
}stLogInfo, *pstLogInfo;


BV_LOG_HANDLE bv_log_create(const char *pcFile)
{
    pstLogHead pLogHead;

    if(pcFile == NULL)
    {
        return 0;
    }
    pLogHead = (pstLogHead)malloc(sizeof(stLogHead));
    pLogHead->FileId = fopen(pcFile, "a+");
    if (pLogHead->FileId == NULL)
    {
        free(pLogHead);
        return 0;
    }
    
    pLogHead->uiThreadCount = LOG_DEFAULT_DEAL_THREAD_NUM;
    pLogHead->LogThreadPoolHandle = bv_threadpool_create(pLogHead->uiThreadCount);
    if(pLogHead->LogThreadPoolHandle == 0)
    {
        free(pLogHead);
        return 0;
    }
    
    pLogHead->uiBlockSize = LOG_FILE_STORAGE_DEFAULT_SIZE_M * 1024 * 1024;
    pLogHead->ulFileSize = 0;
    pLogHead->LogLevel = BV_LOG_LEVEL_ALL;
    return (BV_LOG_HANDLE)pLogHead;
}

BV_RETURN bv_log_close(BV_LOG_HANDLE logHandle)
{
    BV_RETURN ret;
    pstLogHead pLogHead = (pstLogHead)logHandle;
    if(pLogHead == NULL)
    {
        return BV_FAILED;
    }
    ret = bv_threadpool_destroy(pLogHead->LogThreadPoolHandle);
    if (ret != BV_SUCCESS)
    {
        return ret;
    }
    fclose(pLogHead->FileId);
    free(pLogHead);
    pLogHead = NULL;
    return ret;
}


BV_RETURN bv_log_set_level(BV_LOG_HANDLE logHandle, BV_LOG_LEVEL logLevel)
{
    pstLogHead pLogHead = (pstLogHead)logHandle;
    if(pLogHead == NULL || logLevel > BV_LOG_LEVEL_ALL)
    {
        return BV_PARAM_ERROR;
    }
    pLogHead->LogLevel = logLevel;
    return BV_SUCCESS;
}

BV_RETURN bv_log_set_storage_block_size(BV_LOG_HANDLE logHandle, unsigned int uiBlockSize)
{
    pstLogHead pLogHead = (pstLogHead)logHandle;
    if(pLogHead == NULL || uiBlockSize == 0)
    {
        return BV_PARAM_ERROR;
    }
    pLogHead->uiBlockSize = uiBlockSize * 1024 * 1024;
    return BV_SUCCESS;
}


BV_RETURN bv_log_set_thread_num(BV_LOG_HANDLE logHandle, unsigned int uiThreadCount)
{
    BV_RETURN ret;
    pstLogHead pLogHead = (pstLogHead)logHandle;
    if(pLogHead == NULL || uiThreadCount == 0)
    {
        return BV_PARAM_ERROR;
    }
    pLogHead->uiThreadCount = uiThreadCount;
    ret = bv_threadpool_destroy(logHandle);
    if (ret == BV_SUCCESS)
    {
        pLogHead->LogThreadPoolHandle = bv_threadpool_create(pLogHead->uiThreadCount);
        if(pLogHead->LogThreadPoolHandle == 0)
        {
            free(pLogHead);
            return BV_FAILED;
        }
    }

    return BV_SUCCESS;
}


static void* bv_log_save(void *pvLog)
{
    pstLogInfo pLogInfo = (pstLogInfo)pvLog;
    if (pLogInfo != NULL)
    {
        if (pLogInfo->logHead->ulFileSize + strlen(pLogInfo->log) > pLogInfo->logHead->uiBlockSize)
        {
            //create a new file to storage[jiangxl]
            //1,fclose(pLogInfo->logHead->FileId)
            //2,pLogInfo->logHead->FileId = fopen
            //3,pLogInfo->logHead->ulFileSize = 0
        }
        pLogInfo->logHead->ulFileSize += strlen(pLogInfo->log);
        fwrite(pLogInfo->log, strlen(pLogInfo->log), 1, pLogInfo->logHead->FileId);
        //fllush(pLogInfo->logHead->FileId);
        free(pLogInfo);
        pLogInfo = NULL;
    }
}

BV_RETURN bv_log(BV_LOG_HANDLE logHandle, BV_LOG_LEVEL logLevel, char* pcLogInfo)
{
    BV_RETURN ret;
    pstLogHead pLogHead = (pstLogHead)logHandle;
    pstLogInfo pLogInfo;
    if (pLogHead == NULL || logLevel > BV_LOG_LEVEL_ALL || pcLogInfo == NULL)
    {
        return BV_PARAM_ERROR;
    }
    if (strlen(pcLogInfo) >= BV_LOG_INFO_LENGTH)
    {
        return BV_LOG_LENGTH_ERROR;
    }

    //日志级别判断
    if (logLevel > pLogHead->LogLevel)
    {
        return BV_LOG_LEVEL_LIMIT;
    }
    
    pLogInfo = (pstLogInfo)malloc(sizeof(stLogInfo));
    if (pLogInfo == NULL)
    {
        return BV_FAILED;
    }
    pLogInfo->logHead = pLogHead;
    memset(pLogInfo->log, 0x00, sizeof(pLogInfo->log));
    sprintf(pLogInfo->log, "%s%s%s%s", bv_time_get(), log_level_tag[logLevel], pcLogInfo, "\n\0");
    ret = bv_threadpool_add_request_service(pLogHead->LogThreadPoolHandle, bv_log_save, (void*)pLogInfo);
    if (ret != BV_SUCCESS)
    {
        free(pLogInfo);
        return BV_FAILED;
    }
    return BV_SUCCESS;
}


