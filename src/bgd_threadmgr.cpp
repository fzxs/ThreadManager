
#include "bgd_threadmgr.h"
#include "common/include/bithelper.h"
#include "bgd_osthread.h"

#include <stdlib.h>
//静态变量定义
CMutexLock * CThreadManager::m_mutex = new CMutexLock;
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
   Func Name: wait
Date Created: 2018-9-17
 Description: 等待所有子线程退出
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
void CThreadManager::wait(struct timespec *timeout,bool abandonDetached)
{
	STThreadDescriptor * pstDescriptor = NULL;
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator it;

	if (abandonDetached)
	{
		//暂时不处理Detached线程
		return;
	}

	for (it = m_threadPool.begin(); it != m_threadPool.end();)
	{
		pstDescriptor = it->second;
		while (BIT_ENABLED(pstDescriptor->flag, THR_JOINING))
		{
			//等待进程结束
			if (BGD_OS::ThreadJoin(pstDescriptor->threadId))
			{
				break;
			}
			it = m_threadPool.erase(it);
			delete pstDescriptor;
			pstDescriptor = NULL;
			continue;
		}
		if (BIT_ENABLED(pstDescriptor->flag, THR_JOINING))
		{
			//等待进程结束
			if (BGD_OS::ThreadJoin(pstDescriptor->threadId))
			{
				break;
			}
			it = m_threadPool.erase(it);
			delete pstDescriptor;
			pstDescriptor = NULL;
			continue;
		}
		it++;
	}
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
int CThreadManager::spawn_n(int n, FuncUser userFunc, void *taskArg)
{
	int i = 0;
	int result = 0;
	
	//1.判断当前线程池数目是否已经到达上限
	if (m_threadNum + n >= m_threadMaxNum)
	{
		return -1;
	}
	//2.创建线程

	for (i = 0; i < n; i++)
	{
		result = spawn_i(userFunc, taskArg);
		if (result)
		{
			break;
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
int CThreadManager::spawn_i(FuncUser userFunc, void *taskArg)
{
	int result = 0;
	STThreadDescriptor *pstDescriptor = NULL;
	CThreadAdapter *pclsAdapter = NULL;

	pstDescriptor = new STThreadDescriptor;
	//暂时的策略，后期将会由任务传值
	SET_BITS(pstDescriptor->flag, THR_JOINING);
	pclsAdapter = new CThreadAdapter(userFunc, taskEntrance, taskArg);
	result = BGD_OS::ThreadCreate(&pstDescriptor->threadId, pstDescriptor->stack, pstDescriptor->stackSize, pclsAdapter);
	//将线程信息加入线程管理器
	m_threadPool.insert(std::make_pair(pstDescriptor->threadId, pstDescriptor));
	m_threadNum++;

	return result;
}

int CThreadManager::clearThreadInfo(STThreadDescriptor *pclsDescriptor)
{
	if (NULL == pclsDescriptor)
	{
		return -1;
	}
	//2.关闭该线程
	BGD_OS::ThreadCancel(pclsDescriptor->threadId);
	//3.释放线程对象
	delete pclsDescriptor;
	pclsDescriptor = NULL;

	return 0;
}

/********************************************************
   Func Name: cancel
Date Created: 2018-9-19
 Description: 取消子线程
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::cancel(bgd_thread_t threadId)
{
	//1.在线程管理器中查找该线程
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator itFind;
	STThreadDescriptor * pclsDescriptor = NULL;

	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		return -1;
	}

	//2.关闭该线程.释放线程资源
	clearThreadInfo(pclsDescriptor);
	//3.从线程管理器中删除该对象
	m_threadPool.erase(itFind);
	m_threadNum--;

	return 0;
}

void CThreadManager::setThreadStatus(int status)
{
	//1.获取当前线程id
	bgd_thread_t threadId;
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator itFind;
	STThreadDescriptor * pclsDescriptor = NULL;

	threadId = BGD_OS::ThreadSelf();
	//2.在线程管理器中查找该线程
	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		//暂时不作处理--有可能需要做容错
		return ;
	}
	pclsDescriptor = itFind->second;
	pclsDescriptor->status = status;
}

/********************************************************
   Func Name: adjustReduce
Date Created: 2018-9-19
 Description: 优化线程池
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::adjustReduce()
{
	bgd_thread_t threadId;
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator itFind;
	STThreadDescriptor * pclsDescriptor = NULL;
	
	//1.判断当前线程的数量是否已经达到最小值
	if (m_threadNum <= m_threadMinNum)
	{
		return -1;
	}

	//获取当前线程id
	threadId = BGD_OS::ThreadSelf();
	//2.在线程管理器中查找该线程
	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		//暂时不作处理--有可能需要做容错
		return -1;
	}
	pclsDescriptor = itFind->second;
	//3关闭该线程.释放线程资源
	clearThreadInfo(pclsDescriptor);
	//4.从线程管理器中删除该对象---不能删除，因为线程管理器还在使用
	//m_threadPool.erase(itFind);
	m_threadNum--;

	return 0;
}








