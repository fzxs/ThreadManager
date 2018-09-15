
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
 Description: �鷽�������߳�������
       Input: 
      Output: 
      Return: �����û�������ִ�н��
     Caution: 
*********************************************************/
THR_FUNC_RETURN CThreadAdapter::invoke(void)
{
	int iRet = 0;

	//�������
	if (NULL == m_userFunc || NULL == m_entryPoint || NULL == arg)
	{
		return -1;
	}
	//ִ���û�����
	iRet = m_userFunc(arg);

	return iRet;
}



