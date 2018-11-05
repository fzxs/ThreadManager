

#ifndef __BGD_DESCRIPT_H_
#define __BGD_DESCRIPT_H_

#include "threadmgr/bgd_osthread.h"
#include "threadmgr/bithelper.h"

#include <stdint.h>
#include <list>

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
typedef enum _THR_FLAG
{
	THR_FLAG_JOIN = 0x0,         //�ɵȴ��߳�
	THR_FLAG_DETACH = 0x1,       //�����߳�
}EN_THR_FLAG;

/* �߳���Ϣ�� */
struct STThreadDescriptor
{
	bgd_thread_t threadId;                        //�߳�ID
	EN_THR_FLAG flag;                             //�߳�����--�Ƿ���룬�Ƿ�join֮��
	void *stack;                                  //�߳�ջ��ַ
	uint32_t stack_size;                          //�߳�ջ��С
	THREAD_STATUS status;                         //�̵߳�״̬(idel,runing)
public:
	STThreadDescriptor() :threadId(0), flag(THR_FLAG_JOIN), stack(0), stack_size(0), status(THR_IDLE) {}
};

/* �߳�����Ϣ */  
struct STThreadGroupDescriptor
{
	int priority;                                               //���ȼ�
	std::list<STThreadDescriptor> thread_list;                  //�߳���Ϣ
public:
	STThreadGroupDescriptor() :priority(0) {}
};



#endif

