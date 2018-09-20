
#ifndef __GTL_QUEUE_H_
#define __GTL_QUEUE_H_

#include "doublelink.h"

typedef STDLinkNode STQueueNode;

typedef STDLinkHead STQueue;

#ifndef QUEUE_FOREACH
#define QUEUE_FOREACH(pstHead, pstEntry, member) \
for(pstEntry = LIST_ENTRY(queue_front(pstHead), typeof(*pstEntry), member); \
pstEntry != NULL; \
pstEntry = (pstEntry->member.pstNext)? LIST_ENTRY(pstEntry->member.pstNext, typeof(*pstEntry), member) : NULL)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	STQueue * queue_init();

	STQueueNode * queue_back(STQueue *pstQueue);

	STQueueNode * queue_front(STQueue *pstQueue);

	void queue_push(STQueue *pstQueue, STQueueNode *pstNode);

	STQueueNode * queue_pop(STQueue *pstQueue);

	void queue_release(STQueue **pstQueue);

	int queue_size(STQueue *pstQueue);


#ifdef __cplusplus
}
#endif


#endif




