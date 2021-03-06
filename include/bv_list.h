/********************************************************************************************
@author Bevis
@date 2018-09-02

********************************************************************************************/
#ifndef __BV_LIST_H__
#define __BV_LIST_H__

#include <bv_common.h>

#define BV_LIST_MEMBER(type) BV_NODE_MEMBER(type)

typedef unsigned long BV_LIST_HANDLE;


/********************************************************************************************
@函数名称:
bv_list_create

@函数功能:
创建链表

@函数参数:
ulItemSize:链表每个元素的大小
uiListLength:链表的最大长度。如果输入为0，则认为链表长度没有限制，否则输入参数为链表最大长度。

@函数返回值
创建成功，返回链表句柄(非0值)
创建失败，返回0
********************************************************************************************/
BV_LIST_HANDLE bv_list_create(unsigned long ulItemSize, unsigned int uiListLength);

/********************************************************************************************
@函数名称:
bv_queue_destroy

@函数功能:
销毁链表

@函数参数:
@IN
listHandle:链表句柄

@函数返回值
BV_SUCCESS:返回成功
其他值:失败
********************************************************************************************/
BV_RETURN bv_list_destroy(BV_LIST_HANDLE listHandle);

/********************************************************************************************
@函数名称:
bv_list_insert_node_from_head

@函数功能:
头插

@函数参数:
----IN
listHandle:链表句柄
pvItem:插入元素地址


@函数返回值
BV_SUCCESS:返回成功
其他值:失败
********************************************************************************************/
BV_RETURN bv_list_insert_node_from_head(BV_LIST_HANDLE listHandle, void *pvItem);

/********************************************************************************************
@函数名称:
bv_list_insert_node_from_tail

@函数功能:
尾插

@函数参数:
----IN
listHandle:链表句柄
pvItem:插入元素地址


@函数返回值
BV_SUCCESS:返回成功
其他值:失败
********************************************************************************************/
BV_RETURN bv_list_insert_node_from_tail(BV_LIST_HANDLE listHandle, void *pvItem);

/********************************************************************************************
@函数名称:
bv_list_get_node_from_head

@函数功能:
从链表头取出一个元素数据

@函数参数:
----IN
listHandle:链表句柄
----OUT
pvItem:取出元素


@函数返回值
BV_SUCCESS:返回成功
其他值:失败
********************************************************************************************/
BV_RETURN bv_list_get_node_from_head(BV_LIST_HANDLE listHandle, void **pvItem);

/********************************************************************************************
@函数名称:
bv_list_get_node_from_tail

@函数功能:
从链表尾部取出一个元素数据

@函数参数:
----IN
listHandle:链表句柄
----OUT
pvItem:取出元素


@函数返回值
BV_SUCCESS:返回成功
其他值:失败
********************************************************************************************/
BV_RETURN bv_list_get_node_from_tail(BV_LIST_HANDLE listHandle, void **pvItem);

/********************************************************************************************
@函数名称:
bv_list_delete_node

@函数功能:
从链表中删除一个节点

@函数参数:
----IN
listHandle:链表句柄
pvItem:删除节点


@函数返回值
BV_SUCCESS:返回成功
其他值:失败
********************************************************************************************/
BV_RETURN bv_list_delete_node(BV_LIST_HANDLE listHandle, void *pvItem);

/********************************************************************************************
@函数名称:
bv_list_find_node

@函数功能:
从链表中查找数据

@函数参数:
----IN
listHandle:链表句柄
pvItem:对比参数
----OUT
pComparefun:将节点指针通过此回调函数返回。

@函数返回值
其他值:节点地址
NULL:没找到,或参数错误
********************************************************************************************/
void* bv_list_find_node(BV_LIST_HANDLE listHandle, pCompareFunc pCompareFun, void *pvItem);

/********************************************************************************************
@函数名称:
bv_list_get_node_num

@函数功能:
获取链表中节点的数量

@函数参数:
----IN
listHandle:链表句柄
----OUT
piNodeNum:整形指针，用于取出节点数量

@函数返回值
BV_SUCCESS:返回成功
其他值:失败
********************************************************************************************/
BV_RETURN bv_list_get_node_num(BV_LIST_HANDLE listHandle, int* piNodeNum);

/********************************************************************************************
@函数名称:
bv_list_get_node_num

@函数功能:
输出链表的所有节点地址，方便输出节点信息。

@函数参数:
----IN
listHandle:链表句柄
----OUT
pShowNodeFun:取出节点地址

@函数返回值
BV_SUCCESS:返回成功
其他值:失败
********************************************************************************************/
BV_RETURN bv_list_show_all_node(BV_LIST_HANDLE listHandle, pShowNodeFunc pShowNodeFun);

#endif //__BV_LIST_H__



