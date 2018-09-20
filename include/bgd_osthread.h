
#ifndef __BGD_OSTHREAD_H_
#define __BGD_OSTHREAD_H_

#include <pthread.h>

#ifdef WIN
#define pthread_t int 
#endif

#define bgd_thread_t pthread_t

#include "bgd_adapter.h"

namespace BGD_OS
{
	extern int ThreadCreate(bgd_thread_t *pstThreadId, void *stack, size_t stacksize
		, CBaseThreadAdapter *pclsThreadAdapter);
	extern int ThreadJoin(bgd_thread_t waiterId);
	extern bgd_thread_t ThreadSelf();
	extern int ThreadCancel(bgd_thread_t waiterId);
}

#endif







