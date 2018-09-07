/********************************************************************************************
@author Bevis
@date 2018-09-02

/********************************************************************************************/
#ifndef __BV_COMMON_H__
#define __BV_COMMON_H__

//接口返回值定义
typedef enum
{
    BV_LOG_LEVEL_LIMIT = -9,        //日志级别限制
    BV_LOG_LENGTH_ERROR = -8,       //日志信息超过规定长度
    BV_NO_ITEM = -7,                //队列或者链表没有元素了
    BV_LIST_OVER = -6,              //链表溢出
    BV_CREATE_LIST_FAILED = -5,     //创建链表失败
    BV_CREATE_QUEUE_FAILED = -4,    //创建队列失败
    BV_PARAM_ERROR = -3,            //函数入参错误
    BV_MEM_LACK = -2,               //内存不足
    BV_FAILED = -1,                 //失败
    BV_SUCCESS,                     //成功
}BV_RETURN;

typedef enum 
{
    BV_FALSE = 0,
    BV_TRUE = 1,
}BV_BOOL;

typedef enum
{
    BV_NODE_EXIT,
    BV_NODE_NOEXIT,
}BV_NODE_FIND;
typedef BV_NODE_FIND (*pCompareFunc)(void*, void*);

typedef void (*pShowNodeFunc)(void*);

#define BV_NODE_MEMBER(type)type *prev;type *next

#define BV_SIZE_1M 1024 * 1024

#endif //__BV_COMMON_H__