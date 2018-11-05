
#include "threadmgr/bgd_basetask.h"

#include <stdlib.h>
#include <limits.h>

/********************************************************
AbsTaskBase
*********************************************************/

/********************************************************
   Func Name: AbsTaskBase
Date Created: 2018-9-15
 Description: ���캯��
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
AbsTaskBase::AbsTaskBase(CThreadManager * threadManager):m_thrMgr(threadManager),_task_no(INT_MAX)
{
	//��ʱû�й���
}

/********************************************************
   Func Name: open
Date Created: 2018-11-5
 Description: �����̳߳�
       Input: 
      Output: 
      Return: error code
     Caution: 
*********************************************************/
int AbsTaskBase::open(uint32_t upperLimit, uint32_t lowerLimit, uint32_t step)
{
	int result = 0;
	void *stack[1] = { 0 };
	uint32_t stack_size[1] = { 0 };

	if (NULL == m_thrMgr)
	{
		m_thrMgr = CThreadManager::instance();
	}
	//���̳߳�
	stack[0] = NULL;
	stack_size[0] = 0;

	result = m_thrMgr->open(upperLimit
		, lowerLimit
		, step
		, THR_FLAG_JOIN
		, stack
		, stack_size
		, AbsTaskBase::srvRun
		, (void *)this);

	if (result >= 0)
	{
		_task_no = result;
		result = 0;
	}

	return result;
}

/********************************************************
   Func Name: srv
Date Created: 2018-9-15
 Description: �û�����
       Input: 
      Output: 
      Return: 
     Caution: ���û��̳У�ʵ���Լ���ҵ���߼�
*********************************************************/
THR_FUNC_RETURN AbsTaskBase::srv(void)
{
	return 0;
}

/********************************************************
   Func Name: srvRun
Date Created: 2018-9-15
 Description: ִ���û�����
       Input: 
      Output: 
      Return: 
     Caution: ��AbsTaskBaseʵ�֣��û����Լ̳У�������ò�Ҫ�޸ģ�
	 ��Ϊ�����й����߳��˳������һЩ����
*********************************************************/
THR_FUNC_RETURN AbsTaskBase::srvRun(void *arg)
{
	AbsTaskBase *base = NULL;
	THR_FUNC_RETURN status = 0;

	if (NULL == arg)
	{
		return -1;
	}
	base = (AbsTaskBase *)arg;
	status = base->srv();

	return status;
}

/********************************************************
   Func Name: getThreadManager
Date Created: 2018-9-15
 Description: ִ���û�����
       Input: 
      Output: 
      Return: 
     Caution: ��AbsTaskBaseʵ�֣��û����Լ̳У�������ò�Ҫ�޸ģ�
	 ��Ϊ�����й����߳��˳������һЩ����
*********************************************************/
CThreadManager *AbsTaskBase::getThreadManager(void)
{
	if (NULL == m_thrMgr)
	{
		m_thrMgr = CThreadManager::instance();
	}
	return m_thrMgr;
}

/********************************************************
   Func Name: activate
Date Created: 2018-9-18
 Description: ����Ĭ���߳�
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int AbsTaskBase::activate()
{
	int result = 0;
	if (NULL == m_thrMgr)
	{
		m_thrMgr = CThreadManager::instance();
	}
	if (INT_MAX == _task_no)
	{
		return -1;
	}
	//����Ĭ���߳�
	result = m_thrMgr->activate(_task_no);

	return result;
}





