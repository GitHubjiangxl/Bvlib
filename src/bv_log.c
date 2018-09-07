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
#include <stdarg.h>



#define LOG_LEVEL_TAG 8

#define LOG_FILE_STORAGE_DEFAULT_SIZE_M 8   //LOG文件默认上限为8M
#define LOG_DEFAULT_DEAL_THREAD_NUM 1       //默认的处理线程数量
#define LOG_FILE_PATH_LENGTH 128            //log文件路径

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
    char cFileName[LOG_FILE_PATH_LENGTH];       //文件名称
    unsigned long ulFileSize;                   //存储文件大小
    unsigned int uiBlockSize;                   //文件存储上限
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
    if(pcFile == NULL || strlen(pcFile) >= LOG_FILE_PATH_LENGTH)
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
    fseek(pLogHead->FileId, 0, SEEK_END);
    pLogHead->ulFileSize = ftell(pLogHead->FileId);
    fseek(pLogHead->FileId, 0, SEEK_SET);
    pLogHead->uiThreadCount = LOG_DEFAULT_DEAL_THREAD_NUM;
    pLogHead->LogThreadPoolHandle = bv_threadpool_create(pLogHead->uiThreadCount);
    if(pLogHead->LogThreadPoolHandle == 0)
    {
        free(pLogHead);
        return 0;
    }
    memset(pLogHead->cFileName, 0x00, LOG_FILE_PATH_LENGTH);
    strcpy(pLogHead->cFileName, pcFile);
    pLogHead->uiBlockSize = LOG_FILE_STORAGE_DEFAULT_SIZE_M * 1024 * 1024;
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
    if (pLogHead->FileId != NULL)
    {
        fclose(pLogHead->FileId);
    }
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
    pLogHead->uiBlockSize = uiBlockSize * BV_SIZE_1M;
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

static void* _bv_log_save(void *pvLog)
{
    pstLogInfo pLogInfo = (pstLogInfo)pvLog;
    if (pLogInfo != NULL)
    {
        if (pLogInfo->logHead->FileId != NULL)//再次创建新log文件时,由于系统原因可能会存在失败。
        {
            if (pLogInfo->logHead->ulFileSize + strlen(pLogInfo->log) >= pLogInfo->logHead->uiBlockSize)
            {
                //create a new file to storage
                char buff[LOG_FILE_PATH_LENGTH + BV_TIME_NUM_LENGTH + 1];// +1 '_'
                memset(buff, 0x00, sizeof(buff));
                sprintf(buff, "%s_%s", pLogInfo->logHead->cFileName, bv_time_get_num());
                fclose(pLogInfo->logHead->FileId);
                rename(pLogInfo->logHead->cFileName, buff);
                pLogInfo->logHead->FileId = fopen(pLogInfo->logHead->cFileName, "a+");
                if (pLogInfo->logHead->FileId == NULL)
                {
                    printf("fopen %s error!!! \n", pLogInfo->logHead->cFileName);
                    goto ERROR;
                }
                pLogInfo->logHead->ulFileSize = 0;
            }
            pLogInfo->logHead->ulFileSize += strlen(pLogInfo->log);
            fwrite(pLogInfo->log, strlen(pLogInfo->log), 1, pLogInfo->logHead->FileId);
            fflush(pLogInfo->logHead->FileId);
        }
ERROR:
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
        pcLogInfo[BV_LOG_INFO_LENGTH - 1] = '\0';
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
    ret = bv_threadpool_add_request_service(pLogHead->LogThreadPoolHandle, _bv_log_save, (void*)pLogInfo);
    if (ret != BV_SUCCESS)
    {
        free(pLogInfo);
        return BV_FAILED;
    }
    return BV_SUCCESS;
}

BV_RETURN bv_log_fatal(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...)
{
    va_list arg;
    char buffer[BV_LOG_INFO_LENGTH + 1] = {0};
    
    va_start(arg,pcLogInfo);
    vsnprintf(buffer, BV_LOG_INFO_LENGTH, pcLogInfo, arg);
    va_end(arg);

    return bv_log(logHandle, BV_LOG_LEVEL_FATAL, buffer);
}

BV_RETURN bv_log_error(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...)
{
    va_list arg;
    char buffer[BV_LOG_INFO_LENGTH + 1] = {0};
    
    va_start(arg,pcLogInfo);
    vsnprintf(buffer, BV_LOG_INFO_LENGTH, pcLogInfo, arg);
    va_end(arg);

    return bv_log(logHandle, BV_LOG_LEVEL_ERROR, buffer);
}

BV_RETURN bv_log_warn(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...)
{
    va_list arg;
    char buffer[BV_LOG_INFO_LENGTH + 1] = {0};
    
    va_start(arg,pcLogInfo);
    vsnprintf(buffer, BV_LOG_INFO_LENGTH, pcLogInfo, arg);
    va_end(arg);

    return bv_log(logHandle, BV_LOG_LEVEL_WARN, buffer);
}

BV_RETURN bv_log_info(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...)
{
    va_list arg;
    char buffer[BV_LOG_INFO_LENGTH + 1] = {0};
    
    va_start(arg,pcLogInfo);
    vsnprintf(buffer, BV_LOG_INFO_LENGTH, pcLogInfo, arg);
    va_end(arg);

    return bv_log(logHandle, BV_LOG_LEVEL_INFO, buffer);
}

BV_RETURN bv_log_debug(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...)
{
    va_list arg;
    char buffer[BV_LOG_INFO_LENGTH + 1] = {0};
    
    va_start(arg,pcLogInfo);
    vsnprintf(buffer, BV_LOG_INFO_LENGTH, pcLogInfo, arg);
    va_end(arg);

    return bv_log(logHandle, BV_LOG_LEVEL_DEBUG, buffer);
}

BV_RETURN bv_log_trace(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...)
{
    va_list arg;
    char buffer[BV_LOG_INFO_LENGTH + 1] = {0};
    
    va_start(arg,pcLogInfo);
    vsnprintf(buffer, BV_LOG_INFO_LENGTH, pcLogInfo, arg);
    va_end(arg);

    return bv_log(logHandle, BV_LOG_LEVEL_TRACE, buffer);
}


