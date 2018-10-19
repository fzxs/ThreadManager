
#include "threadmgr/bgd_adapter.h"

#include <stdlib.h>

/********************************************************
������ں���
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
 Description: ������ں���ָ��
       Input: 
      Output: 
      Return: ������ں���ָ��
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
	//ִ���û�����
	iRet = m_userFunc(m_arg);

	//�ͷű���
	delete this;

	return iRet;
}



