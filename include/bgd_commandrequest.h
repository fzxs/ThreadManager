

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
	CExecRequest(CFuture<T> *future, AbsServant *servant, void *arg) :m_future(future)
		, m_servant(servant), m_arg(arg) {}
	~CExecRequest() {}
public:
	//调用函数
	virtual int call(void);
protected:
	CFuture<T> *m_future;
	AbsServant *m_servant;
	void *m_arg;
};

template<typename T>
int CExecRequest<T>::call()
{
	m_future->set(m_servant->invoke(m_arg));
	return 0;
}

#endif

