

#ifndef __BGD_DESCRIPT_H_
#define __BGD_DESCRIPT_H_

#include "threadmgr/bgd_osthread.h"
#include "common/bithelper.h"

#include <string>

//�߳�״̬
enum THREAD_STATUS
{
	//Uninitialized.
	THR_IDLE = 0x00000000,

	//Created but not yet running.
	THR_SPAWNED = 0x00000001,

	//Thread is active
	THR_RUNNING = 0x00000002,

	//Thread is suspended.
	THR_SUSPENDED = 0x00000003,

	//Thread has been cancelled 
	THR_CANCELLED = 0x00000004,

	//Thread prepare exit
	THR_TERMINATING = 0x00000005,

	// Thread has shutdown, but the slot in the thread manager hasn't
	// been reclaimed yet.
	THR_TERMINATED = 0x0000006,
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
	//��ע:��windows��threadid��һ���ṹ�壬��ʼ����ʽ����
	STThreadDescriptor(int _flag, int _status) :threadId(0), status(_status) 
	{
		SET_BITS(flag, _flag);
	}
public:
	bgd_thread_t threadId;                      //�߳�ID
	int flag;                                   //�߳�����--�Ƿ���룬�Ƿ�join֮��
	int status;                                 //�̵߳�״̬(idel,runing)
};

/* �߳���Ϣͷ�� */  
//struct STThreadDescriptorHeader
//{
//public:
//	int priority;                                               //���ȼ�
//	int groupId;                                                //�߳���ID
//	//TemplateTaskQueue<STThreadDescriptorHeader *> threadGroup;  //�߳������
//};



#endif

