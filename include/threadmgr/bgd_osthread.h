
#ifndef __BGD_OSTHREAD_H_
#define __BGD_OSTHREAD_H_

#include <pthread.h>

#ifdef WIN
#define pthread_t int 
#endif

#define bgd_thread_t pthread_t

#include "threadmgr/bgd_adapter.h"

namespace BGD_OS
{
	//分配线程
	extern int ThreadSpawn(bgd_thread_t *pstThreadId, CBaseThreadAdapter *pclsThreadAdapter);

	//等待线程
	extern int ThreadJoin(bgd_thread_t waiterId);

	//获取线程ID
	extern bgd_thread_t ThreadSelf();

	//取消线程
	extern int ThreadCancel(bgd_thread_t threadId);

	//线程退出
	extern void ThreadExit();
}

#endif







