
#include "bgd_adapter.h"

#include <stdlib.h>

/********************************************************
CBaseThreadAdapter
*********************************************************/

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
	if (NULL == m_userFunc || NULL == m_entryPoint || NULL == arg)
	{
		return -1;
	}
	//执行用户方法
	iRet = m_userFunc(arg);

	return iRet;
}



