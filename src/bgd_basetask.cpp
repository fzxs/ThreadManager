
#include "threadmgr/bgd_basetask.h"

#include <stdlib.h>
#include <limits.h>

/********************************************************
AbsTaskBase
*********************************************************/

/********************************************************
   Func Name: AbsTaskBase
Date Created: 2018-9-15
 Description: 构造函数
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
AbsTaskBase::AbsTaskBase(CThreadManager * threadManager):m_thrMgr(threadManager),_task_no(INT_MAX)
{
	//暂时没有功能
}

/********************************************************
   Func Name: open
Date Created: 2018-11-5
 Description: 开启线程池
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
	//打开线程池
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
 Description: 用户方法
       Input: 
      Output: 
      Return: 
     Caution: 由用户继承，实现自己的业务逻辑
*********************************************************/
THR_FUNC_RETURN AbsTaskBase::srv(void)
{
	return 0;
}

/********************************************************
   Func Name: srvRun
Date Created: 2018-9-15
 Description: 执行用户方法
       Input: 
      Output: 
      Return: 
     Caution: 由AbsTaskBase实现，用户可以继承，但是最好不要修改，
	 因为里面有关于线程退出清理的一些方法
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
 Description: 执行用户方法
       Input: 
      Output: 
      Return: 
     Caution: 由AbsTaskBase实现，用户可以继承，但是最好不要修改，
	 因为里面有关于线程退出清理的一些方法
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
 Description: 申请默认线程
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
	//申请默认线程
	result = m_thrMgr->activate(_task_no);

	return result;
}





