
#ifndef __DOUBLELINK_H_
#define __DOUBLELINK_H_

#include "gtl.h"

#ifndef DLINK_FOREACH
#define DLINK_FOREACH(pstHead, pstEntry, member) \
for(pstEntry = LIST_ENTRY(DLink_First(pstHead), typeof(*pstEntry), member); \
pstEntry != NULL; \
pstEntry = (pstEntry->member.pstNext)? LIST_ENTRY(pstEntry->member.pstNext, typeof(*pstEntry), member) : NULL)
#endif

/* ˫������ */

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

	//��ȡͷ���
	STDLinkHead * DLink_Head_Init();

	//�ͷ�˫������
	void DLink_Release(STDLinkHead **pstHead);

	//��ȡ��Ԫ��
	STDLinkNode * DLink_First(STDLinkHead *pstHead);

	//��ȡβԪ��
	STDLinkNode * DLink_Back(STDLinkHead *pstHead);

	//β�����
	void DLink_PushBack(STDLinkHead *pstHead, STDLinkNode *pstNode);

	//ͷ�����
	void DLink_PushFront(STDLinkHead *pstHead, STDLinkNode *pstNode);

	//��Ԫ�ص���
	STDLinkNode * DLink_PopFront(STDLinkHead *pstHead);

	//ĩβԪ�ص���
	STDLinkNode * DLink_PopEnd(STDLinkHead *pstHead);

	//�Ƴ�Ԫ��
	void DLink_Remove(STDLinkHead *pstHead, STDLinkNode *pstNode);

	//��ȡǰһ��Ԫ��
	STDLinkNode * DLink_Prev(STDLinkNode *pstNode);

	//��ȡ��һ��Ԫ��
	STDLinkNode * DLink_Next(STDLinkNode *pstNode);

	//�Ƿ�Ϊ�գ��շ���0���ǿշ���1��
	int DLink__Empty(STDLinkHead *pstHead);

	//��ȡ��ǰԪ�ظ���
	int DLink__Size(STDLinkHead *pstHead);

	

#ifdef __cplusplus
}
#endif


#endif



