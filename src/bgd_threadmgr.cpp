
#include "bgd_threadmgr.h"
#include "common/include/bithelper.h"
#include "bgd_osthread.h"

#include <stdlib.h>
//��̬��������
CMutexLock * CThreadManager::m_mutex = new CMutexLock;
CThreadManager *CThreadManager::m_instance = NULL;

/********************************************************
CThreadManager
*********************************************************/

/********************************************************
   Func Name: init
Date Created: 2018-9-15
 Description: ��ʼ��
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
 Description: ��ȡһ��ʵ��
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
 Description: �ȴ��������߳��˳�
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
		//��ʱ������Detached�߳�
		return;
	}

	for (it = m_threadPool.begin(); it != m_threadPool.end();)
	{
		pstDescriptor = it->second;
		while (BIT_ENABLED(pstDescriptor->flag, THR_JOINING))
		{
			//�ȴ����̽���
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
			//�ȴ����̽���
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
 Description: ����n�����߳�
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::spawn_n(int n, FuncUser userFunc, void *taskArg)
{
	int i = 0;
	int result = 0;
	
	//1.�жϵ�ǰ�̳߳���Ŀ�Ƿ��Ѿ���������
	if (m_threadNum + n >= m_threadMaxNum)
	{
		return -1;
	}
	//2.�����߳�

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
 Description: �������߳�
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
	//��ʱ�Ĳ��ԣ����ڽ���������ֵ
	SET_BITS(pstDescriptor->flag, THR_JOINING);
	pclsAdapter = new CThreadAdapter(userFunc, taskEntrance, taskArg);
	result = BGD_OS::ThreadCreate(&pstDescriptor->threadId, pstDescriptor->stack, pstDescriptor->stackSize, pclsAdapter);
	//���߳���Ϣ�����̹߳�����
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
	//2.�رո��߳�
	BGD_OS::ThreadCancel(pclsDescriptor->threadId);
	//3.�ͷ��̶߳���
	delete pclsDescriptor;
	pclsDescriptor = NULL;

	return 0;
}

/********************************************************
   Func Name: cancel
Date Created: 2018-9-19
 Description: ȡ�����߳�
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::cancel(bgd_thread_t threadId)
{
	//1.���̹߳������в��Ҹ��߳�
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator itFind;
	STThreadDescriptor * pclsDescriptor = NULL;

	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		return -1;
	}

	//2.�رո��߳�.�ͷ��߳���Դ
	clearThreadInfo(pclsDescriptor);
	//3.���̹߳�������ɾ���ö���
	m_threadPool.erase(itFind);
	m_threadNum--;

	return 0;
}

void CThreadManager::setThreadStatus(int status)
{
	//1.��ȡ��ǰ�߳�id
	bgd_thread_t threadId;
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator itFind;
	STThreadDescriptor * pclsDescriptor = NULL;

	threadId = BGD_OS::ThreadSelf();
	//2.���̹߳������в��Ҹ��߳�
	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		//��ʱ��������--�п�����Ҫ���ݴ�
		return ;
	}
	pclsDescriptor = itFind->second;
	pclsDescriptor->status = status;
}

/********************************************************
   Func Name: adjustReduce
Date Created: 2018-9-19
 Description: �Ż��̳߳�
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
	
	//1.�жϵ�ǰ�̵߳������Ƿ��Ѿ��ﵽ��Сֵ
	if (m_threadNum <= m_threadMinNum)
	{
		return -1;
	}

	//��ȡ��ǰ�߳�id
	threadId = BGD_OS::ThreadSelf();
	//2.���̹߳������в��Ҹ��߳�
	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		//��ʱ��������--�п�����Ҫ���ݴ�
		return -1;
	}
	pclsDescriptor = itFind->second;
	//3�رո��߳�.�ͷ��߳���Դ
	clearThreadInfo(pclsDescriptor);
	//4.���̹߳�������ɾ���ö���---����ɾ������Ϊ�̹߳���������ʹ��
	//m_threadPool.erase(itFind);
	m_threadNum--;

	return 0;
}








