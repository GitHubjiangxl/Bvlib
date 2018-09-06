#include <bv.h>
#include <stdio.h>

typedef struct param
{
    int num_1;
    int num_2;
}para, *pPara;

void * add(void*arg)
{
    pPara pFunPara = (pPara)arg;
    //printf("%s(%d):%d\n", __FUNCTION__, __LINE__, pFunPara->num_1 + pFunPara->num_2);
}

void * sub(void*arg)
{
    pPara pFunPara = (pPara)arg;
    //printf("%s(%d):%d\n", __FUNCTION__, __LINE__, pFunPara->num_1 - pFunPara->num_2);
}

void * mul(void*arg)
{
    pPara pFunPara = (pPara)arg;
    //printf("%s(%d):%d\n", __FUNCTION__, __LINE__, pFunPara->num_1 * pFunPara->num_2);
}

void * div(void*arg)
{
    pPara pFunPara = (pPara)arg;
    //printf("%s(%d):%d\n", __FUNCTION__, __LINE__, pFunPara->num_1 / pFunPara->num_2);
}



int main()
{
    para funPara;
    BV_THREADPOOL_HANDLE threadHandle;
    int iLoop = 0;
    funPara.num_1 = 6;
    funPara.num_2 = 3;
    threadHandle = bv_threadpool_create(1);
    for (iLoop = 0; iLoop < 10000; iLoop ++)
    {
        bv_threadpool_add_request_service(threadHandle, add, (void*)&funPara);
        bv_threadpool_add_request_service(threadHandle, sub, (void*)&funPara);
        bv_threadpool_add_request_service(threadHandle, mul, (void*)&funPara);
        bv_threadpool_add_request_service(threadHandle, div, (void*)&funPara);
    }

    while(1)
    {
        pause();
    }
    bv_threadpool_destroy(threadHandle);

    return 0;
}
