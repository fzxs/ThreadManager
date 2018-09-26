

#ifndef __BGD_METHODREQUEST_H_
#define __BGD_METHODREQUEST_H_

#include "bgd_future.h"
#include "bgd_servant.h"

/*命令方法*/

class AbsMethodRequest
{
public:
	virtual ~AbsMethodRequest() {}
public:
	//调用函数
	virtual int call(void) = 0;
};


template<typename T>
class CExecRequest:public AbsMethodRequest
{
public:
	/*
	future之所以为NULL，是因为有的函数不需要返回值
	*/
	CExecRequest(AbsServant<T> *servant, CFuture<T> *future = 0): m_servant(servant), m_future(future){}
	~CExecRequest();
public:
	//调用函数
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
		//用户不需要返回值
		m_servant->invoke();
	}

	return 0;
}

template<typename T>
CExecRequest<T>::~CExecRequest()
{
	//释放AbsServant对象实例
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

