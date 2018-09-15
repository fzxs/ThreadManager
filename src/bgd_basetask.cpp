
#include "bgd_basetask.h"

#include <stdlib.h>

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
AbsTaskBase::AbsTaskBase(CThreadManager * threadManager):m_thrMgr(threadManager)
{
	//��ʱû�й���
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
	CThreadManager *pclsThrMgr = NULL;

	if (NULL == arg)
	{
		return -1;
	}
	base = (AbsTaskBase *)arg;
	status = base->srv();
	//�˳���ǰ�߳�
	pclsThrMgr = base->getThreadManager();
	pclsThrMgr->atExit(NULL, NULL, NULL);

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
CThreadManager *AbsTaskBase::getThreadManager(void)
{
	return m_thrMgr;
}





