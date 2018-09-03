/********************************************************************************************
@author Bevis
@date 2018-09-02

/********************************************************************************************/
#include <bv_thread_pool.h>
#include <bv_queue.h>
#include <bv_list.h>
#include <pthread.h>
#include <malloc.h>


#define THREADPOOL_RUNNING  0
#define THREADPOOL_STOPPED  1

//服务队列
typedef struct queue_ser {
    BV_LIST_MEMBER(struct queue_ser);
    pServiceFun pFun;                       //服务函数
    void *arg;                              //服务函数的参数
}ServiceQueue, *pServiceQueue;


/* 线程池的结构 */
typedef struct {
    int state;                              //线程池的运行状态
    int thread_num;                         //线程池中线程的个数
    pthread_t *tid_arr;                     //线程id的数组
    BV_QUEUE_HANDLE queue_handle;           //线程池的服务队列
    pthread_mutex_t mutex;                  //互斥锁
    pthread_cond_t cond;                    //条件变量
}ThreadPool;


static void * thread_func(void *arg)
{
    ThreadPool *pThreadPool;
    pServiceQueue pservice;
    pThreadPool = (ThreadPool *)arg;
    while (1) 
    {
        pthread_mutex_lock(&pThreadPool->mutex);
        while (pThreadPool->state == THREADPOOL_RUNNING && 
                bv_queue_pop(pThreadPool->queue_handle, (void**)&pservice) == BV_NO_ITEM)
        {
            pthread_cond_wait(&pThreadPool->cond, &pThreadPool->mutex);
        }
        if (pThreadPool->state == THREADPOOL_STOPPED)
        {
            pthread_mutex_unlock(&pThreadPool->mutex);
            break;
        }
        pthread_mutex_unlock(&pThreadPool->mutex);
        pservice->pFun(pservice->arg);
        free(pservice);
    }
    pthread_exit(NULL);
}


BV_THREADPOOL_HANDLE bv_threadpool_create(unsigned int uiThreadNum)
{
    ThreadPool *pThreadPool;
    int iLoop, iRet;
    
    if (uiThreadNum <= 0)
    {
        printf("%s(%d):uiThreadNum=%d Error!!!\n", __FUNCTION__, __LINE__, uiThreadNum);
        return 0;
    }
    printf("%s(%d):uiThreadNum=%d\n", __FUNCTION__, __LINE__, uiThreadNum);
    pThreadPool = (ThreadPool*)malloc(sizeof(ThreadPool));
    pThreadPool->state = THREADPOOL_RUNNING;
    pThreadPool->thread_num = uiThreadNum;
    pThreadPool->tid_arr = (pthread_t*)malloc(uiThreadNum * sizeof(pthread_t));
    pThreadPool->queue_handle = bv_queue_create(sizeof(ServiceQueue), 0);
    pthread_mutex_init(&pThreadPool->mutex, NULL);
    pthread_cond_init(&pThreadPool->cond, NULL);
    for (iLoop = 0; iLoop < uiThreadNum; iLoop++)
    {
        iRet = pthread_create(pThreadPool->tid_arr+iLoop, NULL, thread_func, pThreadPool);
        if ( iRet != 0 )
        {
            fprintf(stderr, "pthread_create():%s\n", strerror(iRet));
            return 0;
        }
    }

    return (BV_THREADPOOL_HANDLE)pThreadPool;
}

BV_RETURN bv_threadpool_destroy(BV_THREADPOOL_HANDLE threadPoolHandle)
{
    ThreadPool *pThreadPool;
    ServiceQueue service;
    int iLoop;
    
    pThreadPool = (ThreadPool *)threadPoolHandle;
    pThreadPool->state = THREADPOOL_STOPPED;
    pthread_cond_broadcast(&pThreadPool->cond);

    //线程退出
    for (iLoop = 0; iLoop < pThreadPool->thread_num; iLoop++) 
    {
        pthread_cancel(pThreadPool->tid_arr[iLoop]);
    }
    free(pThreadPool->tid_arr);

    //删除队列
    bv_queue_destroy(pThreadPool->queue_handle);

    pthread_mutex_destroy(&pThreadPool->mutex);
    pthread_cond_destroy(&pThreadPool->cond);

    free(pThreadPool);
    pThreadPool = NULL;
}


BV_RETURN bv_threadpool_add_request_service(BV_THREADPOOL_HANDLE threadPoolHandle, pServiceFun serviceFun, void *arg)
{
    ThreadPool *pThreadPool;
    pServiceQueue pser;
    pThreadPool = (ThreadPool *)threadPoolHandle;
    
    if (pThreadPool == NULL || serviceFun == NULL)
    {
        printf("%s(%d):BV_PARAM_ERROR\n", __FUNCTION__, __LINE__);
        return BV_PARAM_ERROR;
    }
    
    pser = (pServiceQueue)malloc(sizeof(ServiceQueue));
    pser->pFun = serviceFun;
    pser->arg = arg;

    pthread_mutex_lock(&pThreadPool->mutex);
    bv_queue_push(pThreadPool->queue_handle, pser);
    pthread_mutex_unlock(&pThreadPool->mutex);
    pthread_cond_signal(&pThreadPool->cond);
    return BV_SUCCESS;
}


