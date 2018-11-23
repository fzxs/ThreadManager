
#include "threadmgr/bgd_scheduler.h"
#include <stdio.h>
#include <unistd.h>

/********************************************************
BaseScheduler
*********************************************************/

/********************************************************
   Func Name: BaseScheduler
Date Created: 2018-9-15
 Description: ���캯��
       Input:  @max_thread������߳���
			   @n_queue����Ϣ������
			   @strategy���ȴ�����
      Output: 
      Return: 
     Caution: 
*********************************************************/
BaseScheduler::BaseScheduler(uint32_t max_thread, uint32_t n_queue)
{
	uint32_t min_thread = 0;
	/*
	--ע������ȴ�����Ϣ�����ٴ����̣߳���Ϊ�̻߳�����Ϣ�����Ƿ���ֵ
	������Ϣ���в��������ͻ�����ڴ����
	*/
	//������Ϣ����
	m_queue = new CMsgQueue<AbsMethodRequest *>(n_queue);
	//���̳߳�
	min_thread = max_thread / 3 == 0 ? 1 : (max_thread / 3);
	open(max_thread, min_thread, INCREASE_INTERVAL);
}

/********************************************************
   Func Name: ~CScheduler
Date Created: 2018-9-15
 Description: ��������
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
BaseScheduler::~BaseScheduler()
{
	delete m_queue;
	m_queue = NULL;
}

/********************************************************
   Func Name: srv
Date Created: 2018-9-15
 Description: ��������
       Input: 
      Output: 
      Return: 
     Caution: �ú����������߳�������
*********************************************************/
THR_FUNC_RETURN BaseScheduler::srv(void)
{
	int result = 0;
	AbsMethodRequest * pclsRequest = NULL;
	while (true)
	{
		//�߳�״̬���--���������߳��˳�
		m_thrMgr->checkStatus();

		//��ȡ�����е�Ԫ��
		result = m_queue->dequeue(pclsRequest, THREAD_QUIT_TIME);
		if (result)
		{
			//˵����������ʱ
			//��ʱ˵����Ҫ�Ż��̳߳أ��������߳�ȡ��
			////--debug
			//printf("request queue is empty , and i quit thread .\n");
			if (m_thrMgr->adjustReduce(_task_no))
			{
				////--debug
				//printf("i am alive .\n");
				//��ʱ�̳߳��е��߳��Ѿ��������״̬���������˳��߳���
				continue;
			}
			//�˳���ǰ�߳�
			break;
		}

		//ִ�����������еķ���
		pclsRequest->call();
		//�ͷ���������
		delete pclsRequest;
		pclsRequest = NULL;
	}

	return 0;
}

/********************************************************
   Func Name: addRequest
Date Created: 2018-9-15
 Description: �������
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int BaseScheduler::addRequest(AbsMethodRequest * request, long timeout)
{
	int result = 0;
	//1.�������������еĳ��ȣ�����ﵽ�ٽ�ֵ����Ҫ�ٴ����������߳�
	if (m_queue->full())
	{
		printf("request queue is full , and i activate thread .\n");
		activate();
	}
	//��ǰ���������������ô��ʱ�ȴ�ʱ��
	result = m_queue->enqueue(request, timeout);

	return result;
}



