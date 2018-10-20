
#ifndef __BGD_SCHEDULER_H_
#define __BGD_SCHEDULER_H_

#include "threadmgr/bgd_basetask.h"
#include "threadmgr/bgd_methodrequest.h"
#include "threadmgr/bgd_msgqueue.h"

#define INCREASE_INTERVAL 5           //线程递增步长
#define DEFAULT_QUEUE 50              //默认消息队列长度
/*
这个等待时间很有考究，因为如果线程超时等待，便会退出线程，这个退出的时机必须在线程wait之后执行
*/
#define THREAD_QUIT_TIME 10            //等待请求队列队列的时间
#define ENQUEUE_TIMEOUT 60*5           //请求队列已满，等待时间为5分钟，超过5分钟，任务全部抛弃

/* 调度器 */

class BaseScheduler:public AbsTaskBase
{
public:
	BaseScheduler(size_t nSize = DEFAULT_QUEUE);
	virtual ~BaseScheduler();
public:
	//执行任务队列
	virtual THR_FUNC_RETURN srv(void);

	//添加请求
	int addRequest(AbsMethodRequest * request);

private:
	//禁止拷贝构造函数
	BaseScheduler(BaseScheduler &r) {}

private:
	CMsgQueue<AbsMethodRequest *> *m_queue;

};

#endif




