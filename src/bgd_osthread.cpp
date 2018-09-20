
#include "bgd_osthread.h"

#include <stdlib.h>
#include <pthread.h>

int BGD_OS::ThreadCreate(bgd_thread_t *pstThreadId, void *stack, size_t stacksize, CBaseThreadAdapter *pclsThreadAdapter)
{
	int iRet = 0;

	if (NULL == pstThreadId || NULL == pclsThreadAdapter)
	{
		return -1;
	}

	iRet = pthread_create(pstThreadId, NULL, pclsThreadAdapter->entryPoint(), pclsThreadAdapter);

	return iRet;
}

int BGD_OS::ThreadJoin(bgd_thread_t waiterId)
{
	int iRet = 0;

	iRet = pthread_join(waiterId, NULL);

	return iRet;
}

bgd_thread_t BGD_OS::ThreadSelf()
{
	return pthread_self();
}

int BGD_OS::ThreadCancel(bgd_thread_t waiterId)
{
	return pthread_cancel(waiterId);
}










