
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
	//�����߳�
	extern int ThreadSpawn(bgd_thread_t *pstThreadId, CBaseThreadAdapter *pclsThreadAdapter);

	//�ȴ��߳�
	extern int ThreadJoin(bgd_thread_t waiterId);

	//��ȡ�߳�ID
	extern bgd_thread_t ThreadSelf();

	//ȡ���߳�
	extern int ThreadCancel(bgd_thread_t threadId);

	//�߳��˳�
	extern void ThreadExit();
}

#endif







