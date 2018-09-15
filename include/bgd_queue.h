
#ifndef __BGD_QUEUE_H_
#define __BGD_QUEUE_H_

/* 任务队列类 */

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

/* 线程队列类 */
class CThreadQueue
{
public:
};

#endif




