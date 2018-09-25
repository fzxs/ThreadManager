
#ifndef __BGD_SCHEDULER_H_
#define __BGD_SCHEDULER_H_

#include "bgd_basetask.h"
#include "bgd_commandrequest.h"
#include "bgd_msgqueue.h"

#define INCREASE_INTERVAL 10   //�̵߳�������
#define DEFAULT_QUEUE 50        //Ĭ����Ϣ���г���

/* ������ */

class CScheduler:public AbsTaskBase
{
public:
	CScheduler(size_t nSize = DEFAULT_QUEUE);
	~CScheduler();
public:
	//ִ���������
	virtual THR_FUNC_RETURN srv(void);

	//�������
	int addRequest(AbsMethodRequest * request);

	//�ȴ��߳�ִ�н���
	void wait();

private:
	//��ֹ�������캯��
	CScheduler(CScheduler &r) {}

private:
	CMsgQueue<AbsMethodRequest *> *m_queue;

};

#endif




