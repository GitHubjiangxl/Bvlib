/********************************************************************************************
@author Bevis
@date 2018-09-04

********************************************************************************************/
#ifndef __BV_LOG_H__
#define __BV_LOG_H__

#include <bv_common.h>

typedef unsigned long BV_LOG_HANDLE;
#define BV_LOG_INFO_LENGTH 1024

typedef enum 
{
    BV_LOG_LEVEL_FATAL,     //指定非常严重的错误事件，这可能导致应用程序中止
    BV_LOG_LEVEL_ERROR,     //错误事件可能仍然允许应用程序继续运行
    BV_LOG_LEVEL_WARN,      //指定具有潜在危害的情况
    BV_LOG_LEVEL_INFO,      //指定能够突出在粗粒度级别的应用程序运行情况的信息的消息
    BV_LOG_LEVEL_DEBUG,     //指定细粒度信息事件是最有用的应用程序调试
    BV_LOG_LEVEL_TRACE,     //指定细粒度比DEBUG更低的信息事件
    BV_LOG_LEVEL_ALL,       //输出所有日志
}BV_LOG_LEVEL;

/********************************************************************************************
@函数名称:
bv_log_create

@函数功能:
创建日志文件(目前默认单线程，多线程可能会存在日志错乱)

@函数参数:
----IN
pcFile:日志文件

@函数返回值
创建成功，返回日志句柄(非0值)
创建失败，返回0
********************************************************************************************/
BV_LOG_HANDLE bv_log_create(const char *pcFile);

/********************************************************************************************
@函数名称:
bv_log_close

@函数功能:
关闭log(关闭文件)

@函数参数:
----IN
pcFile:日志文件
uiThreadCount:用几个线程来处理log消息。

@函数返回值
成功：BV_SUCCESS
失败：其他值
********************************************************************************************/
BV_RETURN bv_log_close(BV_LOG_HANDLE logHandle);


/********************************************************************************************
@函数名称:
bv_log_set_level

@函数功能:
设置日志级别

@函数参数:
----IN
logHandle:日志句柄
logLevel:日志级别。

@函数返回值
成功：BV_SUCCESS
失败：其他值

@注意
如果不设置log级别，则默认级别为BV_LOG_LEVEL_ALL，既所有级别日志均保存。
********************************************************************************************/
BV_RETURN bv_log_set_level(BV_LOG_HANDLE logHandle, BV_LOG_LEVEL logLevel);

/********************************************************************************************
@函数名称:
bv_log_set_thread_num

@函数功能:
同时有几个线程在写log (我认为这个函数是个多余，多个线程可能会存在文件操作上的问题)

@函数参数:
----IN
logHandle:日志句柄
uiThreadCount:线程数量

@函数返回值
成功：BV_SUCCESS
失败：其他值

********************************************************************************************/
BV_RETURN bv_log_set_thread_num(BV_LOG_HANDLE logHandle, unsigned int uiThreadCount);

/********************************************************************************************
@函数名称:
bv_log_set_storage_block_size

@函数功能:
设置文件存储上限为uiBlockSize(M)

@函数参数:
----IN
logHandle:日志句柄
uiBlockSize:存储上限(单位：M)

@函数返回值
成功：BV_SUCCESS
失败：其他值

********************************************************************************************/
BV_RETURN bv_log_set_storage_block_size(BV_LOG_HANDLE logHandle, unsigned int uiBlockSize);

/********************************************************************************************
@函数名称:
bv_log

@函数功能:
写log

@函数参数:
----IN
logHandle:日志句柄
logLevel:日志级别。
pcLogInfo:当前支持log最大长度为1024字节(包含\0)。


@函数返回值
成功：BV_SUCCESS
失败：其他值

@注意
如果不设置log级别，则默认级别为BV_LOG_LEVEL_ALL，既所有级别日志均保存。
********************************************************************************************/
BV_RETURN bv_log(BV_LOG_HANDLE logHandle, BV_LOG_LEVEL logLevel, char* pcLogInfo);


/********************************************************************************************
@函数名称:
1,bv_log_fatal
2,bv_log_error
3,bv_log_warn
4,bv_log_info
5,bv_log_debug
6,bv_log_trace

@函数功能:
输出对应LOG级别的函数。

@函数参数:
----IN
logHandle:日志句柄
pcLogInfo:
...:可变参数

@函数返回值
成功：BV_SUCCESS
失败：其他值
********************************************************************************************/
BV_RETURN bv_log_fatal(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...);
BV_RETURN bv_log_error(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...);
BV_RETURN bv_log_warn(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...);
BV_RETURN bv_log_info(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...);
BV_RETURN bv_log_debug(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...);
BV_RETURN bv_log_trace(BV_LOG_HANDLE logHandle, char* pcLogInfo, ...);

#endif //__BV_LOG_H__



