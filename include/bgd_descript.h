

#ifndef __BGD_DESCRIPT_H_
#define __BGD_DESCRIPT_H_

#include "bgd_queue.h"

#ifdef WIN
#define pthread_t int 
#endif 

typedef pthread_t bgd_thread_t;

/* �߳���Ϣ�� */
struct STThreadDescriptorHeader
{
public:
	bgd_thread_t threadId;                      //�߳�ID
	int flag;                                   //�߳�����--�Ƿ���룬�Ƿ�join֮��
	int priority;                               //���ȼ�
	int groupId;                                //�߳���ID
	char *threadName;                           //�߳�����
	void *stack;                                //�߳�ջ��ַ
	unsigned int stackSize;                     //�߳�ջ��С
	int status;                                 //�̵߳�״̬(idel,runing)
};

/* �߳���Ϣͷ�� */
struct STThreadDescriptor
{
public:
	int priority;                                               //���ȼ�
	int groupId;                                                //�߳���ID
	TemplateTaskQueue<STThreadDescriptorHeader *> threadGroup;  //�߳������
};



#endif

