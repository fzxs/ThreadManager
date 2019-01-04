
#ifndef __BGD_SCHEDULER_H_
#define __BGD_SCHEDULER_H_

#include "threadmgr/bgd_basetask.h"
#include "threadmgr/bgd_methodrequest.h"
#include "threadmgr/bgd_msgqueue.h"

#include <stdint.h>


#define INCREASE_INTERVAL 5           //Ĭ���̵߳�������
#define DEFAULT_QUEUE 50              //Ĭ����Ϣ���г���
#define INITED_THREAD_COUNT 30        //Ĭ�ϳ�ʼ���̵߳ĸ���       
/*
����ȴ�ʱ����п�������Ϊ����̳߳�ʱ�ȴ�������˳��̣߳�����˳���ʱ���������߳�wait֮��ִ��
*/
#define THREAD_QUIT_TIME 10            //�ȴ�������ж��е�ʱ��
#define ENQUEUE_TIMEOUT 60*5           //��������������ȴ�ʱ��Ϊ5���ӣ�����5���ӣ�����ȫ������


/* ������ */

class BaseScheduler:public AbsTaskBase
{
public:
	virtual ~BaseScheduler();

public:
	//��ʼ��
	int open(uint32_t max_thread = INITED_THREAD_COUNT, uint32_t n_queue = DEFAULT_QUEUE);

	//ִ���������
	virtual THR_FUNC_RETURN srv(void);

	//�������
	int addRequest(AbsMethodRequest * request, long timeout = -1);

protected:
	//������ʵ����
	BaseScheduler();

private:
	//��ֹ�������캯��
	BaseScheduler(BaseScheduler &r) {}

private:
	CMsgQueue<AbsMethodRequest *> *m_queue;

};

#endif




