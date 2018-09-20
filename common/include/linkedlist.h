
#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

#include "gtl.h"

typedef struct tagStListNode
{
	struct tagStListNode *pstNext;
}StListNode;

typedef struct tagStListHead
{
	StListNode *pstFirstNode;
}StListHead;

#ifndef LINK_FOREACH
#define LINK_FOREACH(pstHead, pstEntry, member) \
for(pstEntry = LIST_ENTRY(Link_First(pstHead), typeof(*pstEntry), member); \
pstEntry != NULL; \
pstEntry = (pstEntry->member.pstNext)? LIST_ENTRY(pstEntry->member.pstNext, typeof(*pstEntry), member) : NULL)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	//��ȡͷ���
	StListHead * Link_Head_Init();

	//�ͷ�����
	void Link_Release(StListHead **pstHead);

	StListNode * Link_First(StListHead *pstHead);

	void Link_PushBack(StListHead *pstHead, StListNode *pstNode);

	StListNode * Link_PopFront(StListHead *pstHead);
	

#ifdef __cplusplus
}
#endif

#endif


