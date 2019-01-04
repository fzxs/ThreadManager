

#ifndef __BGD_BASETASK_H_
#define __BGD_BASETASK_H_

#include "threadmgr/bgd_common.h"
#include "threadmgr/bgd_threadmgr.h"
#include "threadmgr/bgd_descript.h"

#include <stdint.h>

class AbsTaskBase
{
public:
	virtual ~AbsTaskBase(void) {};

public:
	//�û�ҵ������
	virtual THR_FUNC_RETURN srv(void);

	//�����̳߳�
	virtual int open(uint32_t upperLimit, uint32_t lowerLimit, uint32_t step);

	//�����߳�
	virtual int activate();

public:
	//����ָ�����з���--��static�޷���������ָ�봫��
	static THR_FUNC_RETURN srvRun(void *);

	//��ȡ�̹߳�����
	CThreadManager *getThreadManager(void);

protected:
	//��ֹʵ����
	AbsTaskBase(CThreadManager * = 0);

protected:
	CThreadManager *m_thrMgr;             //�̹߳�����ָ��
	int _task_no;                         //������������к�--���к����̹߳��������з���

};

#endif




