
#ifndef __BGD_QUEUE_H_
#define __BGD_QUEUE_H_

/* ��������� */

#include "lock.h"

template<typename T>
class TemplateTaskQueue
{
public:
	TemplateTaskQueue();
public:
	void set(T obj);
	T get();

private:
	CMutexLock m_mutex;
	CCondLock m_cond;
};

/* �̶߳����� */
class CThreadQueue
{
public:
};

#endif




