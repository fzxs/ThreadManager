
#include "bgd_adapter.h"

#include <stdlib.h>

/********************************************************
任务入口函数
*********************************************************/
void * taskEntrance(void *arg)
{
	if (NULL == arg)
	{
		return NULL;
	}
	CBaseThreadAdapter *pclsAdapter = (CBaseThreadAdapter *)arg;
	pclsAdapter->invoke();

	return NULL;
}

/********************************************************
CBaseThreadAdapter
*********************************************************/

/********************************************************
   Func Name: entryPoint
Date Created: 2018-9-15
 Description: 返回入口函数指针
       Input: 
      Output: 
      Return: 返回入口函数指针
     Caution: 
*********************************************************/
FuncEntrance CBaseThreadAdapter::entryPoint()
{
	return m_entryPoint;
}


/********************************************************
CThreadAdapter
*********************************************************/

/********************************************************
   Func Name: invoke
Date Created: 2018-9-15
 Description: 虚方法被子线程所调用
       Input: 
      Output: 
      Return: 返回用户函数的执行结果
     Caution: 
*********************************************************/
THR_FUNC_RETURN CThreadAdapter::invoke(void)
{
	int iRet = 0;

	//参数检查
	//执行用户方法
	iRet = m_userFunc(m_arg);

	//释放本身
	delete this;

	return iRet;
}



