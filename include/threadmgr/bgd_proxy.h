
#ifndef __BGD_PROXY_H_
#define __BGD_PROXY_H_

#include "threadmgr/bgd_scheduler.h"
#include "threadmgr/bgd_future.h"
#include "threadmgr/bgd_methodrequest.h"

/* ������ */
//
//template<typename T>
//class CProxy
//{
//public:
//	CProxy(CScheduler *scheduler) :m_scheduler(scheduler) {}
//public:
//
//	//����ע��
//	CFuture<T> * registerTask(AbsServant<T> *servant);
//
//	//�޷���ֵ����ע��
//	int registerTaskNoOut(AbsServant<T> *servant);
//
//private:
//	CScheduler *m_scheduler;
//};
//
///********************************************************
//   Func Name: registerTask
//Date Created: 2018-9-26
// Description: ����ע��
//       Input: servant��ִ�з���ʵ������
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
// Description: ����ע��
//       Input: servant��ִ�з���ʵ������
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




