

#ifndef __BGD_DESCRIPT_H_
#define __BGD_DESCRIPT_H_

#include "bgd_osthread.h"

//线程状态
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

//线程属性
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


/* 线程信息类 */
struct STThreadDescriptor
{
public:
	bgd_thread_t threadId;                      //线程ID
	int flag;                                   //线程属性--是否分离，是否join之类
	int priority;                               //优先级
	int groupId;                                //线程组ID
	char *threadName;                           //线程名字
	void *stack;                                //线程栈地址
	size_t stackSize;                           //线程栈大小
	int status;                                 //线程的状态(idel,runing)
};

/* 线程信息头类 */  
struct STThreadDescriptorHeader
{
public:
	int priority;                                               //优先级
	int groupId;                                                //线程组ID
	//TemplateTaskQueue<STThreadDescriptorHeader *> threadGroup;  //线程组队列
};



#endif

