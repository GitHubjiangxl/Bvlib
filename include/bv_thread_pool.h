/********************************************************************************************
@author Bevis
@date 2018-09-02

********************************************************************************************/
#ifndef __BV_THREAD_POOL_H__
#define __BV_THREAD_POOL_H__

#include <bv_common.h>

typedef unsigned long BV_THREADPOOL_HANDLE;
typedef void* (*pServiceFun)(void*);

/********************************************************************************************
@函数名称:
bv_threadpool_create

@函数功能:
创建一个线程池

@函数参数:
----IN
uiThreadNum:初始化线程池中线程的数量

@函数返回值
创建成功，返回线程池句柄(非0值)
创建失败，返回0
/********************************************************************************************/
BV_THREADPOOL_HANDLE bv_threadpool_create(unsigned int uiThreadNum);

/********************************************************************************************
@函数名称:
bv_threadpool_destroy

@函数功能:
销毁一个线程池

@函数参数:
----IN
threadPoolHandle:线程池句柄

@函数返回值
BV_SUCCESS:成功。
其他值:失败。
/********************************************************************************************/
BV_RETURN bv_threadpool_destroy(BV_THREADPOOL_HANDLE threadPoolHandle);

/********************************************************************************************
@函数名称:
bv_threadpool_add_request_service

@函数功能:
向队列里添加请求服务。

@函数参数:
----IN
threadPoolHandle:线程池句柄
serviceFun:服务函数(回调函数)
arg:服务函数参数(回调函数参数)


@函数返回值
BV_SUCCESS:成功。
其他值:失败。
/********************************************************************************************/
BV_RETURN bv_threadpool_add_request_service(BV_THREADPOOL_HANDLE threadPoolHandle, pServiceFun serviceFun, void *arg);


#endif //__BV_THREAD_POOL_H__



