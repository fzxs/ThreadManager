

#ifndef __BGD_METHODREQUEST_H_
#define __BGD_METHODREQUEST_H_

#include "bgd_future.h"
#include "bgd_servant.h"

/*�����*/

class AbsMethodRequest
{
public:
	virtual ~AbsMethodRequest() {}
public:
	//���ú���
	virtual int call(void) = 0;
};


template<typename T>
class CExecRequest:public AbsMethodRequest
{
public:
	/*
	future֮����ΪNULL������Ϊ�еĺ�������Ҫ����ֵ
	*/
	CExecRequest(AbsServant<T> *servant, CFuture<T> *future = 0): m_servant(servant), m_future(future){}
	~CExecRequest();
public:
	//���ú���
	virtual int call(void);
protected:
	AbsServant<T> *m_servant;
	CFuture<T> *m_future;
};

template<typename T>
int CExecRequest<T>::call()
{
	if (m_future)
	{
		m_future->set(m_servant->invoke());
	}
	else
	{
		//�û�����Ҫ����ֵ
		m_servant->invoke();
	}

	return 0;
}

template<typename T>
CExecRequest<T>::~CExecRequest()
{
	//�ͷ�AbsServant����ʵ��
	if (m_servant)
	{
		if (0 == m_servant->release())
		{
			delete m_servant;
		}
		m_servant = NULL;
	}
}

#endif

