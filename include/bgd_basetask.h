

#ifndef __BGD_BASETASK_H_
#define __BGD_BASETASK_H_

#include "bgd_common.h"
#include "bgd_threadmgr.h"


class AbsTaskBase
{
public:
	AbsTaskBase(CThreadManager * = 0);
	virtual ~AbsTaskBase(void) {};
public:
	//�û�ҵ������
	virtual THR_FUNC_RETURN srv(void);

public:

	//����ָ�����з���--��static�޷���������ָ�봫��
	static THR_FUNC_RETURN srvRun(void *);

	//��ȡ�̹߳�����
	CThreadManager *getThreadManager(void);


protected:
	CThreadManager *m_thrMgr;             //�̹߳�����ָ��

};

#endif




