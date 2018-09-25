
#include "bgd_scheduler.h"
#include <stdio.h>
#include <unistd.h>

CScheduler::CScheduler(size_t nSize)
{
	m_queue = new CMsgQueue<AbsMethodRequest *>(nSize);
}

CScheduler::~CScheduler()
{
	delete m_queue;
	m_queue = NULL;
}

THR_FUNC_RETURN CScheduler::srv(void)
{
	int result = 0;
	AbsMethodRequest * pclsRequest = NULL;
	while (true)
	{
		m_thrMgr->checkStatus();
		//while (true)
		//{
		//	//��ȡ�����е�Ԫ��
		//	result = m_queue->dequeue(pclsRequest,3);
		//	if (result)
		//	{
		//		//˵����������ʱ
		//		//��ʱ˵����Ҫ�Ż��̳߳أ��������߳�ȡ��
		//		//�˳���ǰ�߳�
		//		printf("request queue is empty , and i quit thread .\n");
		//		if (m_thrMgr->adjustReduce())
		//		{
		//			printf("i am alive .\n");
		//			sleep(10000);
		//			//��ʱ�̳߳��е��߳��Ѿ��������״̬���������˳��߳���
		//			continue;
		//		}
		//		return 0;
		//	}
		//	//�����ȡ�������˳�ѭ����ִ������
		//	break;
		//}

		//��ȡ�����е�Ԫ��
		result = m_queue->dequeue(pclsRequest, 3);
		if (result)
		{
			//˵����������ʱ
			//��ʱ˵����Ҫ�Ż��̳߳أ��������߳�ȡ��
			//�˳���ǰ�߳�
			printf("request queue is empty , and i quit thread .\n");
			if (m_thrMgr->adjustReduce())
			{
				printf("i am alive .\n");
				sleep(10000);
				//��ʱ�̳߳��е��߳��Ѿ��������״̬���������˳��߳���
				continue;
			}
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

int CScheduler::addRequest(AbsMethodRequest * request)
{
	int result = 0;
	//1.�������������еĳ��ȣ�����ﵽ�ٽ�ֵ����Ҫ�ٴ����������߳�
	if (m_queue->isFull())
	{
		printf("request queue is full , and i activate thread .\n");
		activate(INCREASE_INTERVAL);
	}
	result = m_queue->enqueue(request);

	return result;
}

void CScheduler::wait()
{
	if (NULL == m_thrMgr)
	{
		m_thrMgr = CThreadManager::instance();
	}
	m_thrMgr->wait();
}



