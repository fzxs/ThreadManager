
#include "bgd_basetask.h"

#include <stdlib.h>

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
AbsTaskBase::AbsTaskBase(CThreadManager * threadManager):m_thrMgr(threadManager)
{
	//暂时没有功能
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
	CThreadManager *pclsThrMgr = NULL;

	if (NULL == arg)
	{
		return -1;
	}
	base = (AbsTaskBase *)arg;
	status = base->srv();
	//退出当前线程
	pclsThrMgr = base->getThreadManager();
	pclsThrMgr->atExit(NULL, NULL, NULL);

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
CThreadManager *AbsTaskBase::getThreadManager(void)
{
	return m_thrMgr;
}





