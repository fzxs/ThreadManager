
#include "bgd_threadmgr.h"
#include "common/include/bithelper.h"
#include "bgd_osthread.h"

#include <stdlib.h>
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
	m_threadMaxNum = 50;
	m_threadMinNum = 5;
	return 0;
}

void CThreadManager::setMaxThread(size_t n)
{
	m_threadMaxNum = n;
}

void CThreadManager::setMinThread(size_t n)
{
	m_threadMinNum = n;
}

/********************************************************
   Func Name: instance
Date Created: 2018-9-18
 Description: 获取一个实例
       Input: 
      Output: 
      Return: 
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
 Description: 创建n个子线程
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::spawn_n(int n_threads, int flag, void *stack[], size_t stack_size[], FuncUser userFunc, void *task)
{
	int i = 0;
	int result = 0;

	//参数检查
	if (0 == n_threads || NULL == userFunc || NULL == task)
	{
		return -1;
	}
	
	//判断当前线程池数目是否已经到达上限

	//加锁保护
	m_mutex->lock();
	if (m_threadPool.size() + n_threads >= m_threadMaxNum)
	{
		return -1;
	}
	m_mutex->unlock();

	//2.创建线程

	for (i = 0; i < n_threads; i++)
	{
		if (NULL == stack)
		{
			result = spawn_i(flag, NULL, 0, userFunc, task);
			if (result)
			{
				break;
			}
		}
		else
		{
			result = spawn_i(flag, stack[i], stack_size[i], userFunc, task);
			if (result)
			{
				break;
			}
		}
		
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
int CThreadManager::spawn_i(int flag,void *stack, size_t stack_size, FuncUser userFunc, void *task)
{
	int result = 0;
	STThreadDescriptor *pstDescriptor = NULL;
	CThreadAdapter *pclsAdapter = NULL;

	pstDescriptor = new STThreadDescriptor(flag, THR_RUNNING);
	if (NULL == pstDescriptor)
	{
		return -1;
	}
	pclsAdapter = new CThreadAdapter(flag, stack, stack_size, userFunc, taskEntrance, task);
	if (NULL == pclsAdapter)
	{
		return -1;
	}

	result = BGD_OS::ThreadSpawn(&pstDescriptor->threadId, pclsAdapter);
	if (result)
	{
		return result;
	}
	//将线程信息加入线程管理器

	//加锁保护
	m_mutex->lock();
	m_threadPool.insert(std::make_pair(pstDescriptor->threadId, pstDescriptor));
	m_mutex->unlock();

	return 0;
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
int CThreadManager::adjustReduce()
{
	int result = 0;
	
	//判断当前线程的数量是否已经达到最小值

	//加锁保护
	m_mutex->lock();
	if (m_threadPool.size() <= m_threadMinNum)
	{
		m_mutex->unlock();
		return 1;
	}
	m_mutex->unlock();

	//退出当前线程
	result = atExit();

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

	pstDescriptor = getThreadDescriptor(BGD_OS::ThreadSelf());
	if (NULL == pstDescriptor)
	{
		//该线程状态不明，立刻退出
		atExit();
		return;
	}
	if (THR_TERMINATED == pstDescriptor->status)
	{
		atExit();
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
int CThreadManager::atExit()
{
	bgd_thread_t threadId;

	//获取当前线程
	threadId = BGD_OS::ThreadSelf();
	//清除线程信息
	removeThr(threadId);
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

	//发送检测心跳
	m_joinCond->signal();

	m_mutex->unlock();

	return 0;
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
	wait()函数必定是在主线程中调用，遇到开启多个线程池的场景，每个线程池都必须wait
	*/

	while (true)
	{
		//加锁保护
		m_mutex->lock();

		//判断当前线程数量如果为0，退出线程池
		if (0 == m_threadPool.size())
		{
			break;
		}

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










