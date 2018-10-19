
#include "threadmgr/bgd_threadmgr.h"
#include "threadmgr/bithelper.h"
#include "threadmgr/bgd_osthread.h"

#include <stdlib.h>
//��̬��������
CMutexLock * CThreadManager::m_mutex = new CMutexLock;
CCondLock * CThreadManager::m_joinCond = new CCondLock(m_mutex);
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
	//��ʱռ��λ�ӣ��������init()����Ҫ��ʲô
	m_threadMaxNum = MAX_THREAD;
	m_threadMinNum = MIN_THREAD;
	return 0;
}

/********************************************************
   Func Name: setMaxThread
Date Created: 2018-9-15
 Description: ��������߳���
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
void CThreadManager::setMaxThread(size_t n)
{
	m_threadMaxNum = n;
}

/********************************************************
   Func Name: setMinThread
Date Created: 2018-9-15
 Description: ������С�߳���
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
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
   Func Name: spawn_n
Date Created: 2018-9-17
 Description: ����n�����߳�
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::spawn_n(int n_threads, int flag, void *stack[], size_t stack_size[], FuncUser userFunc, void *task)
{
	int i = 0;
	int result = 0;

	//�������
	if (0 == n_threads || NULL == userFunc || NULL == task)
	{
		return -1;
	}
	
	//�жϵ�ǰ�̳߳���Ŀ�Ƿ��Ѿ���������

	//��������
	m_mutex->lock();
	if (m_threadPool.size() + n_threads >= m_threadMaxNum)
	{
		m_mutex->unlock();
		return -1;
	}
	m_mutex->unlock();

	//2.�����߳�

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
 Description: �������߳�
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
	//���߳���Ϣ�����̹߳�����

	//��������
	m_mutex->lock();
	m_threadPool.insert(std::make_pair(pstDescriptor->threadId, pstDescriptor));
	m_mutex->unlock();

	return 0;
}

/********************************************************
   Func Name: adjustReduce
Date Created: 2018-9-19
 Description: �Ż��̳߳�--�����̵߳�����
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::adjustReduce()
{
	int result = 0;
	
	//�жϵ�ǰ�̵߳������Ƿ��Ѿ��ﵽ��Сֵ

	//��������
	m_mutex->lock();
	if (m_threadPool.size() <= m_threadMinNum)
	{
		m_mutex->unlock();
		return 1;
	}
	m_mutex->unlock();

	//�˳���ǰ�߳�
	result = atExit();

	return result;
}

/********************************************************
   Func Name: checkStatus
Date Created: 2018-9-21
 Description: ��鵱ǰ�߳�״̬
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
		//���߳�״̬�����������˳�
		atExit();
		return;
	}
	if (THR_TERMINATED == pstDescriptor->status)
	{
		atExit();
		return;
	}
	//�����������ʱ��ʵ��--����ȴ�
}

/********************************************************
   Func Name: getThreadDescriptor
Date Created: 2018-9-21
 Description: ��ȡ�߳���Ϣ
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
STThreadDescriptor *CThreadManager::getThreadDescriptor(bgd_thread_t threadId)
{
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator itFind;
	STThreadDescriptor * pstDescriptor = NULL;

	//��������
	m_mutex->lock();

	//���̹߳������в��Ҹ��߳�
	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		//zhang_debug   �Ҳ���������Ҫ���ݴ�
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
 Description: ���߳��˳�
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::atExit()
{
	bgd_thread_t threadId;

	//��ȡ��ǰ�߳�
	threadId = BGD_OS::ThreadSelf();
	//����߳���Ϣ
	removeThr(threadId);
	//�˳���ǰ�߳�
	BGD_OS::ThreadExit();

	return 0;
}

/********************************************************
   Func Name: removeThr
Date Created: 2018-9-21
 Description: �Ƴ��߳�
       Input: 
      Output: 
      Return: 
     Caution: �߳��˳�ǰ�����ô˺���������߳���Ϣ
*********************************************************/
int CThreadManager::removeThr(bgd_thread_t threadId)
{
	int result = 0;
	STThreadDescriptor * pstDescriptor = NULL;
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator itFind;

	//��������
	m_mutex->lock();
	itFind = m_threadPool.find(threadId);
	if (itFind == m_threadPool.end())
	{
		m_mutex->unlock();
		return 0;
	}
	pstDescriptor = itFind->second;

	//�޸��߳�״̬
	pstDescriptor->status = THR_TERMINATED;

	//�Ƴ���ǰ�̶߳���
	m_threadPool.erase(itFind);

	//��ӵ�ɾ��������
	m_threadRemove.push_back(pstDescriptor);

	//�����˳��߳��ź�--�÷�����һ���ɹ��������û�н���wait�ͻᷢ��ʧ�ܣ�ע��
	result = m_joinCond->signal();

	m_mutex->unlock();

	return result;
}

/********************************************************
   Func Name: close
Date Created: 2018-9-21
 Description: �ر��̳߳�
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CThreadManager::close()
{
	//�ر��̳߳�
	//1.������ǰ�̳߳أ��������߳�������ΪTHR_TERMINATING
	//2.�ȴ����߳�ִ��״̬���ʱ�������ر�

	STThreadDescriptor * pstDescriptor = NULL;
	std::map<bgd_thread_t, STThreadDescriptor *>::iterator it;

	//��������
	m_mutex->lock();
	for (it = m_threadPool.begin(); it != m_threadPool.end(); ++it)
	{
		pstDescriptor = it->second;
		//�������߳�״̬��ΪTHR_TERMINATING
		pstDescriptor->status = THR_TERMINATING;
	}
	m_mutex->unlock();

	return 0;
}

/********************************************************
   Func Name: wait
Date Created: 2018-9-21
 Description: �ȴ����߳��˳�
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
	wait()�����ض��������߳��е��ã������ڿ�������̳߳ص��趨
	*/

	while (true)
	{
		//��������
		m_mutex->lock();

		//�ȴ��ź�
		m_joinCond->wait();

		//�����߳�
		for (itRemove = m_threadRemove.begin(); itRemove != m_threadRemove.end(); ++itRemove)
		{
			pstDescriptor = *itRemove;
			BGD_OS::ThreadJoin(pstDescriptor->threadId);
			//�ͷ��߳���Ϣ
			delete pstDescriptor;
			pstDescriptor = NULL;
		}
		//�������
		std::vector<STThreadDescriptor *>().swap(m_threadRemove);
		m_mutex->unlock();
	}
}










