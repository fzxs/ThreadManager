
#include "threadmgr/bgd_threadmgr.h"
#include "threadmgr/bithelper.h"
#include "threadmgr/bgd_osthread.h"

#include <stdlib.h>
#include <string.h>

//静态变量定义
CMutexLock * CThreadManager::m_mutex = new CMutexLock;
CCondLock * CThreadManager::m_joinCond = new CCondLock(m_mutex);
CThreadManager *CThreadManager::m_instance = NULL;

/********************************************************
CThreadManager
*********************************************************/

/********************************************************
   Func Name: init
Date Created: 2018-9-15
 Description: 初始化
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::init()
{
	//初始化任务列表
	memset(_taskManager, 0, sizeof(STThreadDescriptor *) * MAX_TASK_NUMBER);
	_taskCount = 0;

	return 0;
}

/********************************************************
   Func Name: open
Date Created: 2018-11-5
 Description: 开启任务管理器
       Input: 
      Output: 
      Return: 任务ID
     Caution: 
*********************************************************/
int CThreadManager::open(uint32_t upperLimit
	, uint32_t lowerLimit
	, uint32_t step
	, EN_THR_FLAG flag
	, void *stack[]
	, uint32_t stack_size[]
	, FuncUser userFunc
	, void *task)
{
	int result = 0;
	uint32_t task_id = 0;
	STTaskDescriptor *pstTaskDescriptor = NULL;

	if (0 == upperLimit || 0 == lowerLimit || upperLimit < lowerLimit || NULL == userFunc || NULL == task)
	{
		return -1;
	}

	pstTaskDescriptor = new STTaskDescriptor();
	//判断线程池模式
	if (upperLimit == lowerLimit)
	{
		pstTaskDescriptor->model = THRPool_FIXED;
	}
	else
	{
		pstTaskDescriptor->flag = flag;
		pstTaskDescriptor->upperLimit = upperLimit;
		pstTaskDescriptor->lowerLimit = lowerLimit;
		pstTaskDescriptor->step = step;
		pstTaskDescriptor->stack = stack[0];
		pstTaskDescriptor->stack_size = stack_size[0];
		pstTaskDescriptor->user_func = userFunc;
		pstTaskDescriptor->task = task;
	}
	
	//加锁保护
	m_mutex->lock();
	if (_taskCount >= MAX_TASK_NUMBER)
	{
		m_mutex->unlock();
		return -1;
	}
	task_id = _taskCount++;
	m_mutex->unlock();

	//创建线程组
	result = spawn_n(lowerLimit, task_id, flag, stack, stack_size, userFunc, task);
	if (result)
	{
		return -1;
	}

	/*
	taskManager本身属于全局变量，但是不用担心多线程问题
	，即使多线程同时操作该数组，但是也是操作数组的不同内存空间，没有问题
	*/
	_taskManager[task_id] = pstTaskDescriptor;

	return (int)task_id;
}

/********************************************************
   Func Name: activate
Date Created: 2018-9-15
 Description: 默认申请线程
       Input: 
      Output: 
      Return: 
     Caution: 可变线程池模式中栈地址，和栈大小无法自定义，只能使用默认
*********************************************************/
int CThreadManager::activate(uint32_t group_id)
{
	int result = 0;
	STTaskDescriptor *pstTaskDescriptor = NULL;
	void *stack[1] = { 0 };
	stack[0] = 0;
	uint32_t stack_size[1] = { 0 };

	pstTaskDescriptor = _taskManager[group_id];
	result = spawn_n(pstTaskDescriptor->step
		, group_id
		, pstTaskDescriptor->flag
		, stack
		, stack_size
		, pstTaskDescriptor->user_func
		, pstTaskDescriptor->task);

	return result;

}

/********************************************************
   Func Name: activate
Date Created: 2018-9-15
 Description: 默认申请线程
       Input: 
      Output: 
      Return: 
     Caution: 可变线程池模式中栈地址，和栈大小无法自定义，只能使用默认
*********************************************************/
int CThreadManager::activate(uint32_t n_threads
	, uint32_t group_id
	, EN_THR_FLAG flag
	, void *stack[]
	, uint32_t stack_size[]
	, FuncUser userFunc
	, void *task)
{
	int result = 0;

	result = spawn_n(n_threads
		, group_id
		, flag
		, stack
		, stack_size
		, userFunc
		, task);

	return result;

}

/********************************************************
   Func Name: instance
Date Created: 2018-9-18
 Description: 获取一个实例
       Input: 
      Output: 
      Return: 实例对象
     Caution: 
*********************************************************/
CThreadManager * CThreadManager::instance()
{
	CThreadManager * pclsInstance = NULL;
	//double check
	if (NULL == m_instance)
	{
		m_mutex->lock();
		if (NULL == m_instance)
		{
			pclsInstance = new CThreadManager;
			pclsInstance->init();
			m_instance = pclsInstance;
		}
		m_mutex->unlock();
	}

	return m_instance;
}

/********************************************************
   Func Name: spawn_n
Date Created: 2018-9-17
 Description: 创建线程组
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::spawn_n(uint32_t n_threads 
	, uint32_t group_id 
	, EN_THR_FLAG flag 
	, void *stack[] 
	, uint32_t stack_size[] 
	, FuncUser userFunc 
	, void *task)
{
	int result = 0;
	uint32_t i = 0;
	bgd_thread_t threadId;
	STThreadDescriptor *pstDescriptor = NULL;
	STThreadGroupDescriptor *pstGroupDescriptor = NULL;
	std::map<uint32_t, STThreadGroupDescriptor *>::iterator itFind;
	uint32_t need_thread = 0;   //需要创建的线程数
	uint32_t rest_thread = 0;   //剩余线程数

	//参数检查
	if (0 == n_threads || NULL == userFunc || NULL == task)
	{
		return -1;
	}

	//获取当前线程组
	//加锁保护
	m_mutex->lock();
	itFind = _thread_group.find(group_id);
	if (itFind != _thread_group.end())
	{
		//固定线程池模式不可再申请创建线程
		if (THRPool_FIXED == _taskManager[group_id]->model)
		{
			m_mutex->unlock();
			return -1;
		}
		pstGroupDescriptor = itFind->second;
		//校验当前任务线程是否溢出
		rest_thread = _taskManager[group_id]->upperLimit - pstGroupDescriptor->thread_list.size();
		if (rest_thread <= 0)
		{
			m_mutex->unlock();
			return -1;
		}
		need_thread = n_threads > rest_thread ? rest_thread : n_threads;
	}
	else
	{
		//创建线程组
		pstGroupDescriptor = new STThreadGroupDescriptor();
		need_thread = n_threads;
	}
	m_mutex->unlock();

	//创建线程
	for (i = 0; i < need_thread; i++)
	{
		threadId = spawn_i(flag, stack[i], stack_size[i], userFunc, task);
		if (threadId < 0)
		{
			result = -1;
			break;
		}
		pstDescriptor = new STThreadDescriptor();
		pstDescriptor->threadId = threadId;
		pstDescriptor->flag = flag;
		pstDescriptor->stack = stack[i];
		pstDescriptor->stack_size = stack_size[i];
		pstDescriptor->status = THR_RUNNING;

		pstGroupDescriptor->thread_list.push_back(*pstDescriptor);

		//加锁保护
		m_mutex->lock();
		if (itFind == _thread_group.end())
		{
			//加入线程组列表
			_thread_group.insert(std::make_pair(group_id, pstGroupDescriptor));
		}
		//加入线程池
		m_threadPool.insert(std::make_pair(threadId, pstDescriptor));
		m_mutex->unlock();
	}

	return result;
}

/********************************************************
   Func Name: spawn_i
Date Created: 2018-9-17
 Description: 创建子线程
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
long CThreadManager::spawn_i(EN_THR_FLAG flag
	, void *stack
	, uint32_t stack_size
	, FuncUser userFunc
	, void *task)
{
	int result = 0;
	
	CThreadAdapter *pclsAdapter = NULL;
	bgd_thread_t threadId;

	pclsAdapter = new CThreadAdapter(flag, stack, stack_size, userFunc, taskEntrance, task);
	if (NULL == pclsAdapter)
	{
		return -1;
	}

	result = BGD_OS::ThreadSpawn(&threadId, pclsAdapter);
	if (result)
	{
		return result;
	}

	return (long)threadId;
}

/********************************************************
   Func Name: adjustReduce
Date Created: 2018-9-19
 Description: 优化线程池--减少线程的数量
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::adjustReduce(uint32_t group_id)
{
	int result = 0;
	std::list<STThreadDescriptor>::iterator it;
	bgd_thread_t threadId;

	//获取当前线程
	threadId = BGD_OS::ThreadSelf();
	
	//加锁保护
	m_mutex->lock();
	//判断当前线程的数量是否已经达到最小值
	if (_thread_group[group_id]->thread_list.size() <= _taskManager[group_id]->lowerLimit)
	{
		m_mutex->unlock();
		return -1;
	}
	//从当前线程组中删除该线程
	for (it = _thread_group[group_id]->thread_list.begin()
		; it != _thread_group[group_id]->thread_list.end(); ++it)
	{
		if (it->threadId == threadId)
		{
			_thread_group[group_id]->thread_list.erase(it);
			break;
		}
	}
	m_mutex->unlock();

	//退出当前线程
	result = atExit(threadId);

	return result;
}

/********************************************************
   Func Name: checkStatus
Date Created: 2018-9-21
 Description: 检查当前线程状态
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
void CThreadManager::checkStatus()
{
	STThreadDescriptor * pstDescriptor = NULL;
	bgd_thread_t threadId = BGD_OS::ThreadSelf();

	pstDescriptor = getThreadDescriptor(threadId);
	if (NULL == pstDescriptor)
	{
		//该线程状态不明，直接退出当前线程
		BGD_OS::ThreadExit();
		return;
	}
	if (THR_TERMINATED == pstDescriptor->status)
	{
		atExit(threadId);
		return;
	}
	//其余情况，暂时不实现--比如等待
}

/********************************************************
   Func Name: getThreadDescriptor
Date Created: 2018-9-21
 Description: 获取线程信息
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
STThreadDescriptor *CThreadManager::getThreadDescriptor(bgd_thread_t threadId)
{
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator itFind;
	STThreadDescriptor * pstDescriptor = NULL;

	//加锁保护
	m_mutex->lock();

	//在线程管理器中查找该线程
	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		//zhang_debug   找不到可能需要做容错
		m_mutex->unlock();
		return NULL;
	}
	pstDescriptor = itFind->second;
	m_mutex->unlock();

	return pstDescriptor;
}

/********************************************************
   Func Name: atExit
Date Created: 2018-9-21
 Description: 子线程退出
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::atExit(long threadId)
{
	bgd_thread_t last_id = (bgd_thread_t)threadId;

	if (-1 == threadId)
	{
		//获取当前线程
		last_id = BGD_OS::ThreadSelf();
	}
	
	//清除线程信息
	removeThr(last_id);

	//退出当前线程
	BGD_OS::ThreadExit();

	return 0;
}

/********************************************************
   Func Name: removeThr
Date Created: 2018-9-21
 Description: 移除线程
       Input: 
      Output: 
      Return: 
     Caution: 线程退出前，调用此函数，清除线程信息
*********************************************************/
int CThreadManager::removeThr(bgd_thread_t threadId)
{
	int result = 0;
	STThreadDescriptor * pstDescriptor = NULL;
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator itFind;

	//加锁保护
	m_mutex->lock();
	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		m_mutex->unlock();
		return 0;
	}
	pstDescriptor = itFind->second;

	//修改线程状态
	pstDescriptor->status = THR_TERMINATED;

	//移除当前线程队列
	m_threadPool.erase(itFind);

	//添加到删除队列中
	m_threadRemove.push_back(pstDescriptor);

	//发送退出线程信号--该方法不一定成功，如果还没有进入wait就会发送失败，注意
	result = m_joinCond->signal();

	m_mutex->unlock();

	return result;
}

/********************************************************
   Func Name: close
Date Created: 2018-9-21
 Description: 关闭线程池
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::close()
{
	//关闭线程池
	//1.遍历当前线程池，将所有线程属性置为THR_TERMINATING
	//2.等待子线程执行状态检查时，主动关闭

	STThreadDescriptor * pstDescriptor = NULL;
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator it;

	//加锁保护
	m_mutex->lock();
	for (it = m_threadPool.begin(); it != m_threadPool.end(); ++it)
	{
		pstDescriptor = it->second;
		//将所有线程状态置为THR_TERMINATING
		pstDescriptor->status = THR_TERMINATING;
	}
	//发送退出线程信号
	m_joinCond->signal();

	m_mutex->unlock();

	return 0;
}

/********************************************************
   Func Name: wait
Date Created: 2018-9-21
 Description: 等待子线程退出
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
void CThreadManager::wait()
{
	STThreadDescriptor * pstDescriptor = NULL;
	std::vector<STThreadDescriptor *>::iterator itRemove;

	/*
	wait()函数必定是在主线程中调用，不存在开启多个线程池的设定
	*/

	while (true)
	{
		//加锁保护
		m_mutex->lock();

		//等待信号
		m_joinCond->wait();

		//回收线程
		for (itRemove = m_threadRemove.begin(); itRemove != m_threadRemove.end(); ++itRemove)
		{
			pstDescriptor = *itRemove;
			BGD_OS::ThreadJoin(pstDescriptor->threadId);
			//释放线程信息
			delete pstDescriptor;
			pstDescriptor = NULL;
		}
		//清空容器
		std::vector<STThreadDescriptor *>().swap(m_threadRemove);
		m_mutex->unlock();
	}
}

