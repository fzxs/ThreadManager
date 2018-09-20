
#include "bgd_scheduler.h"
#include <stdio.h>
#include <unistd.h>

CScheduler::CScheduler(size_t nSize)
{
	m_queue = new CMsgQueue<AbsMethodRequest *>(nSize);
}

CScheduler::~CScheduler()
{
}

THR_FUNC_RETURN CScheduler::srv(void)
{
	int result = 0;
	AbsMethodRequest * pclsRequest = NULL;
	while (true)
	{
		m_thrMgr->setThreadStatus(THR_RUNNING);
		while (true)
		{
			//获取队列中的元素
			result = m_queue->dequeue(pclsRequest,3);
			if (result)
			{
				//说明请求任务超时
				//此时说明需要优化线程池，将空闲线程取消
				//退出当前线程
				printf("request queue is empty , and i quit thread .\n");
				if (m_thrMgr->adjustReduce())
				{
					printf("i am alive .\n");
					sleep(10000);
					//此时线程池中的线程已经处于最低状态，不能再退出线程了
					continue;
				}
				return 0;
			}
			//如果获取到任务，退出循环，执行任务
			break;
		}
		
		//执行任务请求中的方法
		pclsRequest->call();
		//释放任务请求
		delete pclsRequest;
		pclsRequest = NULL;
		m_thrMgr->setThreadStatus(THR_SPAWNED);
	}

	return 0;
}

int CScheduler::addRequest(AbsMethodRequest * request)
{
	int result = 0;
	//1.检测任务请求队列的长度，如果达到临界值，需要再次申请更多的线程
	if (m_queue->isFull()||)
	{
		printf("request queue is full , and i activate thread .\n");
		activate(1, INCREASE_INTERVAL);
	}
	result = m_queue->enqueue(request);

	return result;
}

void CScheduler::wait()
{
	if (NULL == m_thrMgr)
	{
		m_thrMgr = CThreadManager::instance();
	}
	m_thrMgr->wait(NULL, false);
}



