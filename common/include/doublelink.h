
#ifndef __DOUBLELINK_H_
#define __DOUBLELINK_H_

#include "gtl.h"

#ifndef DLINK_FOREACH
#define DLINK_FOREACH(pstHead, pstEntry, member) \
for(pstEntry = LIST_ENTRY(DLink_First(pstHead), typeof(*pstEntry), member); \
pstEntry != NULL; \
pstEntry = (pstEntry->member.pstNext)? LIST_ENTRY(pstEntry->member.pstNext, typeof(*pstEntry), member) : NULL)
#endif

/* 双向链表 */

typedef struct tagSTDLinkNode
{
	struct tagSTDLinkNode *pstPrev;
	struct tagSTDLinkNode *pstNext;
}STDLinkNode;

typedef struct tagSTDLinkHead
{
	STDLinkNode *pstFirstNode;
	STDLinkNode *pstLastNode;
	int size;
}STDLinkHead;

#ifdef __cplusplus
extern "C"
{
#endif

	//获取头结点
	STDLinkHead * DLink_Head_Init();

	//释放双向链表
	void DLink_Release(STDLinkHead **pstHead);

	//获取首元素
	STDLinkNode * DLink_First(STDLinkHead *pstHead);

	//获取尾元素
	STDLinkNode * DLink_Back(STDLinkHead *pstHead);

	//尾部添加
	void DLink_PushBack(STDLinkHead *pstHead, STDLinkNode *pstNode);

	//头部添加
	void DLink_PushFront(STDLinkHead *pstHead, STDLinkNode *pstNode);

	//首元素弹出
	STDLinkNode * DLink_PopFront(STDLinkHead *pstHead);

	//末尾元素弹出
	STDLinkNode * DLink_PopEnd(STDLinkHead *pstHead);

	//移除元素
	void DLink_Remove(STDLinkHead *pstHead, STDLinkNode *pstNode);

	//获取前一个元素
	STDLinkNode * DLink_Prev(STDLinkNode *pstNode);

	//获取后一个元素
	STDLinkNode * DLink_Next(STDLinkNode *pstNode);

	//是否为空（空返回0，非空返回1）
	int DLink__Empty(STDLinkHead *pstHead);

	//获取当前元素个数
	int DLink__Size(STDLinkHead *pstHead);

	

#ifdef __cplusplus
}
#endif


#endif



