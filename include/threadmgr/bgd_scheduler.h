
#ifndef __BGD_SCHEDULER_H_
#define __BGD_SCHEDULER_H_

#include "threadmgr/bgd_basetask.h"
#include "threadmgr/bgd_methodrequest.h"
#include "threadmgr/bgd_msgqueue.h"

#define INCREASE_INTERVAL 5           //�̵߳�������
#define DEFAULT_QUEUE 50              //Ĭ����Ϣ���г���
/*
����ȴ�ʱ����п�������Ϊ����̳߳�ʱ�ȴ�������˳��̣߳�����˳���ʱ���������߳�wait֮��ִ��
*/
#define THREAD_QUIT_TIME 10            //�ȴ�������ж��е�ʱ��
#define ENQUEUE_TIMEOUT 60*5           //��������������ȴ�ʱ��Ϊ5���ӣ�����5���ӣ�����ȫ������

/* ������ */

class BaseScheduler:public AbsTaskBase
{
public:
	BaseScheduler(size_t nSize = DEFAULT_QUEUE);
	virtual ~BaseScheduler();
public:
	//ִ���������
	virtual THR_FUNC_RETURN srv(void);

	//�������
	int addRequest(AbsMethodRequest * request);

private:
	//��ֹ�������캯��
	BaseScheduler(BaseScheduler &r) {}

private:
	CMsgQueue<AbsMethodRequest *> *m_queue;

};

#endif




