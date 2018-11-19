
#include "threadmgr/bgd_threadmgr.h"
#include "threadmgr/bithelper.h"
#include "threadmgr/bgd_osthread.h"

#include <stdlib.h>
#include <string.h>

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
	//��ʼ�������б�
	memset(_taskManager, 0, sizeof(STThreadDescriptor *) * MAX_TASK_NUMBER);
	_taskCount = 0;

	return 0;
}

/********************************************************
   Func Name: open
Date Created: 2018-11-5
 Description: �������������
       Input: 
      Output: 
      Return: ����ID
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
	//�ж��̳߳�ģʽ
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
	
	//��������
	m_mutex->lock();
	if (_taskCount >= MAX_TASK_NUMBER)
	{
		m_mutex->unlock();
		return -1;
	}
	task_id = _taskCount++;
	m_mutex->unlock();

	//�����߳���
	result = spawn_n(lowerLimit, task_id, flag, stack, stack_size, userFunc, task);
	if (result)
	{
		return -1;
	}

	/*
	taskManager��������ȫ�ֱ��������ǲ��õ��Ķ��߳�����
	����ʹ���߳�ͬʱ���������飬����Ҳ�ǲ�������Ĳ�ͬ�ڴ�ռ䣬û������
	*/
	_taskManager[task_id] = pstTaskDescriptor;

	return (int)task_id;
}

/********************************************************
   Func Name: activate
Date Created: 2018-9-15
 Description: Ĭ�������߳�
       Input: 
      Output: 
      Return: 
     Caution: �ɱ��̳߳�ģʽ��ջ��ַ����ջ��С�޷��Զ��壬ֻ��ʹ��Ĭ��
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
 Description: Ĭ�������߳�
       Input: 
      Output: 
      Return: 
     Caution: �ɱ��̳߳�ģʽ��ջ��ַ����ջ��С�޷��Զ��壬ֻ��ʹ��Ĭ��
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
 Description: ��ȡһ��ʵ��
       Input: 
      Output: 
      Return: ʵ������
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
 Description: �����߳���
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
	uint32_t need_thread = 0;   //��Ҫ�������߳���
	uint32_t rest_thread = 0;   //ʣ���߳���

	//�������
	if (0 == n_threads || NULL == userFunc || NULL == task)
	{
		return -1;
	}

	//��ȡ��ǰ�߳���
	//��������
	m_mutex->lock();
	itFind = _thread_group.find(group_id);
	if (itFind != _thread_group.end())
	{
		//�̶��̳߳�ģʽ���������봴���߳�
		if (THRPool_FIXED == _taskManager[group_id]->model)
		{
			m_mutex->unlock();
			return -1;
		}
		pstGroupDescriptor = itFind->second;
		//У�鵱ǰ�����߳��Ƿ����
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
		//�����߳���
		pstGroupDescriptor = new STThreadGroupDescriptor();
		need_thread = n_threads;
	}
	m_mutex->unlock();

	//�����߳�
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

		//��������
		m_mutex->lock();
		if (itFind == _thread_group.end())
		{
			//�����߳����б�
			_thread_group.insert(std::make_pair(group_id, pstGroupDescriptor));
		}
		//�����̳߳�
		m_threadPool.insert(std::make_pair(threadId, pstDescriptor));
		m_mutex->unlock();
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
 Description: �Ż��̳߳�--�����̵߳�����
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

	//��ȡ��ǰ�߳�
	threadId = BGD_OS::ThreadSelf();
	
	//��������
	m_mutex->lock();
	//�жϵ�ǰ�̵߳������Ƿ��Ѿ��ﵽ��Сֵ
	if (_thread_group[group_id]->thread_list.size() <= _taskManager[group_id]->lowerLimit)
	{
		m_mutex->unlock();
		return -1;
	}
	//�ӵ�ǰ�߳�����ɾ�����߳�
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

	//�˳���ǰ�߳�
	result = atExit(threadId);

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
	bgd_thread_t threadId = BGD_OS::ThreadSelf();

	pstDescriptor = getThreadDescriptor(threadId);
	if (NULL == pstDescriptor)
	{
		//���߳�״̬������ֱ���˳���ǰ�߳�
		BGD_OS::ThreadExit();
		return;
	}
	if (THR_TERMINATED == pstDescriptor->status)
	{
		atExit(threadId);
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
int CThreadManager::atExit(long threadId)
{
	bgd_thread_t last_id = (bgd_thread_t)threadId;

	if (-1 == threadId)
	{
		//��ȡ��ǰ�߳�
		last_id = BGD_OS::ThreadSelf();
	}
	
	//����߳���Ϣ
	removeThr(last_id);

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
	//�����˳��߳��ź�
	m_joinCond->signal();

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

