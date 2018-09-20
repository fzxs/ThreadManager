

#ifndef __BGD_DESCRIPT_H_
#define __BGD_DESCRIPT_H_

#include "bgd_osthread.h"

//�߳�״̬
enum THREAD_STATUS
{
	//Uninitialized.
	THR_IDLE = 0x00000000,

	//Created but not yet running.
	THR_SPAWNED = 0x00000001,

	//Thread is active
	THR_RUNNING = 0x00000002,

	//hread is suspended.
	THR_SUSPENDED = 0x00000003,

	//Thread has been cancelled 
	THR_CANCELLED = 0x00000004,

	// Thread has shutdown, but the slot in the thread manager hasn't
	// been reclaimed yet.
	THR_TERMINATED = 0x0000005,
};

//�߳�����
enum THREAD_FLAG
{
	//join
	THR_JOINING = 0x00000001,

	//detach
	THR_DETACH = 0x00000002,

	////Thread is active
	//THR_RUNNING = 0x00000002,

	////hread is suspended.
	//THR_SUSPENDED = 0x00000004,

	////Thread has been cancelled 
	//THR_CANCELLED = 0x00000008,

	//// Thread has shutdown, but the slot in the thread manager hasn't
	//// been reclaimed yet.
	//THR_TERMINATED = 0x00000010,

	////Join operation has been invoked on the thread by thread manager.
	//THR_JOINING = 0x00000020
};


/* �߳���Ϣ�� */
struct STThreadDescriptor
{
public:
	bgd_thread_t threadId;                      //�߳�ID
	int flag;                                   //�߳�����--�Ƿ���룬�Ƿ�join֮��
	int priority;                               //���ȼ�
	int groupId;                                //�߳���ID
	char *threadName;                           //�߳�����
	void *stack;                                //�߳�ջ��ַ
	size_t stackSize;                           //�߳�ջ��С
	int status;                                 //�̵߳�״̬(idel,runing)
};

/* �߳���Ϣͷ�� */  
struct STThreadDescriptorHeader
{
public:
	int priority;                                               //���ȼ�
	int groupId;                                                //�߳���ID
	//TemplateTaskQueue<STThreadDescriptorHeader *> threadGroup;  //�߳������
};



#endif

