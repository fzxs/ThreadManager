
#ifndef __BGD_SCHEDULER_H_
#define __BGD_SCHEDULER_H_

#include "bgd_basetask.h"
#include "bgd_commandrequest.h"
#include "bgd_msgqueue.h"

#define INCREASE_INTERVAL 10   //线程递增步长
#define DEFAULT_QUEUE 50        //默认消息队列长度

/* 调度器 */

class CScheduler:public AbsTaskBase
{
public:
	CScheduler(size_t nSize = DEFAULT_QUEUE);
	~CScheduler();
public:
	//执行任务队列
	virtual THR_FUNC_RETURN srv(void);

	//添加任务
	int addRequest(AbsMethodRequest * request);

	//等待线程执行结束
	void wait();

private:
	//禁止拷贝构造函数
	CScheduler(CScheduler &r) {}

private:
	CMsgQueue<AbsMethodRequest *> *m_queue;

};

#endif




