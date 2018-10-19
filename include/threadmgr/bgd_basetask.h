

#ifndef __BGD_BASETASK_H_
#define __BGD_BASETASK_H_

#include "threadmgr/bgd_common.h"
#include "threadmgr/bgd_threadmgr.h"
#include "threadmgr/bgd_descript.h"

class AbsTaskBase
{
public:
	AbsTaskBase(CThreadManager * = 0);
	virtual ~AbsTaskBase(void) {};
public:
	//�û�ҵ������
	virtual THR_FUNC_RETURN srv(void);

	//��������
	virtual int activate(int nThreads = 1, int flag = THR_JOINING, void *stack[] = 0, size_t stack_size[] = 0);

public:

	//����ָ�����з���--��static�޷���������ָ�봫��
	static THR_FUNC_RETURN srvRun(void *);

	//��ȡ�̹߳�����
	CThreadManager *getThreadManager(void);

protected:
	CThreadManager *m_thrMgr;             //�̹߳�����ָ��

};

#endif




