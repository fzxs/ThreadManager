
#ifndef __BGD_PROXY_H_
#define __BGD_PROXY_H_

#include "threadmgr/bgd_scheduler.h"
#include "threadmgr/bgd_future.h"
#include "threadmgr/bgd_methodrequest.h"

/* 代理类 */
//
//template<typename T>
//class CProxy
//{
//public:
//	CProxy(CScheduler *scheduler) :m_scheduler(scheduler) {}
//public:
//
//	//任务注册
//	CFuture<T> * registerTask(AbsServant<T> *servant);
//
//	//无返回值任务注册
//	int registerTaskNoOut(AbsServant<T> *servant);
//
//private:
//	CScheduler *m_scheduler;
//};
//
///********************************************************
//   Func Name: registerTask
//Date Created: 2018-9-26
// Description: 任务注册
//       Input: servant：执行方法实例对象
//      Output: 
//      Return: 
//     Caution: 
//*********************************************************/
//template<typename T>
//CFuture<T> * CProxy<T>::registerTask(AbsServant<T> *servant)
//{
//	int result = 0;
//	CExecRequest<T> *request = NULL;
//	CFuture<T> * future = NULL;
//
//	if (NULL == servant)
//	{
//		return NULL;
//	}
//
//	future = new CFuture<T>();	
//	request = new CExecRequest<T>(servant, future);
//	result = m_scheduler->addRequest(request);
//	if (result)
//	{
//		delete future;
//		future = NULL;
//
//		delete request;
//		request = NULL;
//	}
//
//	return future;
//}
//
//
///********************************************************
//   Func Name: registerTask
//Date Created: 2018-9-26
// Description: 任务注册
//       Input: servant：执行方法实例对象
//      Output: 
//      Return: 
//     Caution: 
//*********************************************************/
//template<typename T>
//int CProxy<T>::registerTaskNoOut(AbsServant<T> *servant)
//{
//	int result = 0;
//	CExecRequest<T> *request = NULL;
//
//	if (NULL == servant)
//	{
//		return -1;
//	}
//
//	request = new CExecRequest<T>(servant, NULL);
//	result = m_scheduler->addRequest(request);
//	if (result)
//	{
//		delete request;
//		request = NULL;
//	}
//
//	return result;
//}


#endif




