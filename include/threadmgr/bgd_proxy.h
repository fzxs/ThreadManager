
#ifndef __BGD_PROXY_H_
#define __BGD_PROXY_H_

#include "threadmgr/bgd_scheduler.h"
#include "threadmgr/bgd_future.h"
#include "threadmgr/bgd_methodrequest.h"

/* ������ */

template<typename T>
class CProxy
{
public:
	CProxy(CScheduler *scheduler) :m_scheduler(scheduler) {}
public:

	//ʵ��ע��
	CFuture<T> * servantRegister(AbsServant<T> *servant, AbsFutureObserver<T> *observer = NULL, bool needFuture = true);

private:
	CScheduler *m_scheduler;
};

/********************************************************
   Func Name: servantRegister
Date Created: 2018-9-26
 Description: ʵ��ע�᷽��
       Input: servant��ִ�з���ʵ������
	       needFuture���Ƿ�����Ҫ����ֵ
	         observer���Ƿ����ù۲���
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
CFuture<T> * CProxy<T>::servantRegister(AbsServant<T> *servant, AbsFutureObserver<T> *observer, bool needFuture)
{
	int result = 0;
	CExecRequest<T> *request = NULL;
	CFuture<T> * future = NULL;

	if (needFuture)
	{
		future = new CFuture<T>();
	}
	
	if (observer)
	{
		if (!needFuture)
		{
			delete future;
			future = NULL;
			return NULL;
		}
		future->attach(observer);
	}

	request = new CExecRequest<T>(servant, future);
	result = m_scheduler->addRequest(request);
	if (result)
	{
		delete future;
		future = NULL;

		delete request;
		request = NULL;
	}

	return future;
}


#endif




