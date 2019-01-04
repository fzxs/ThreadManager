
#include "threadmgr/bgd_scheduler.h"
#include <stdio.h>
#include <unistd.h>

/********************************************************
BaseScheduler
*********************************************************/

/********************************************************
   Func Name: BaseScheduler
Date Created: 2018-9-15
 Description: 构造函数
       Input:  @max_thread：最大线程数
			   @n_queue：消息队列数
			   @strategy：等待策略
      Output: 
      Return: 
     Caution: 
*********************************************************/
BaseScheduler::BaseScheduler()
{
}

/********************************************************
   Func Name: ~CScheduler
Date Created: 2018-9-15
 Description: 析构函数
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
BaseScheduler::~BaseScheduler()
{
	delete m_queue;
	m_queue = NULL;
}


/********************************************************
   Func Name: open
Date Created: 2019-1-4
 Description: 初始化
       Input:  @max_thread：最大线程数
			   @n_queue：消息队列数
			   @strategy：等待策略
      Output: 
      Return: 
     Caution: 
*********************************************************/
int BaseScheduler::open(uint32_t max_thread, uint32_t n_queue)
{
	int result = 0;
	uint32_t min_thread = 0;
	/*
	--注意必须先创建消息队列再创建线程，因为线程会检查消息队列是否有值
	假如消息队列不创建，就会出现内存错误
	*/
	//创建消息队列
	m_queue = new CMsgQueue<AbsMethodRequest *>(n_queue);
	//打开线程池
	min_thread = max_thread / 3 == 0 ? 1 : (max_thread / 3);
	result = AbsTaskBase::open(max_thread, min_thread, INCREASE_INTERVAL);

	return result;
}


/********************************************************
   Func Name: srv
Date Created: 2018-9-15
 Description: 请求处理函数
       Input: 
      Output: 
      Return: 
     Caution: 该函数是在子线程中运行
*********************************************************/
THR_FUNC_RETURN BaseScheduler::srv(void)
{
	int result = 0;
	AbsMethodRequest * pclsRequest = NULL;
	while (true)
	{
		//线程状态监测--用来控制线程退出
		m_thrMgr->checkStatus();

		//获取队列中的元素
		result = m_queue->dequeue(pclsRequest, THREAD_QUIT_TIME);
		if (result)
		{
			//说明请求任务超时
			//此时说明需要优化线程池，将空闲线程取消
			////--debug
			//printf("request queue is empty , and i quit thread .\n");
			if (m_thrMgr->adjustReduce(_task_no))
			{
				////--debug
				//printf("i am alive .\n");
				//此时线程池中的线程已经处于最低状态，不能再退出线程了
				continue;
			}
			//退出当前线程
			break;
		}

		//执行任务请求中的方法
		pclsRequest->call();
		//释放任务请求
		delete pclsRequest;
		pclsRequest = NULL;
	}

	return 0;
}

/********************************************************
   Func Name: addRequest
Date Created: 2018-9-15
 Description: 添加请求
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int BaseScheduler::addRequest(AbsMethodRequest * request, long timeout)
{
	int result = 0;
	//1.检测任务请求队列的长度，如果达到临界值，需要再次申请更多的线程
	if (m_queue->full())
	{
		printf("request queue is full , and i activate thread .\n");
		activate();
	}
	//当前队列如果已满，那么超时等待时间
	result = m_queue->enqueue(request, timeout);

	return result;
}



