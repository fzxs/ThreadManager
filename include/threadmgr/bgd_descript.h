

#ifndef __BGD_DESCRIPT_H_
#define __BGD_DESCRIPT_H_

#include "threadmgr/bgd_osthread.h"
#include "threadmgr/bithelper.h"

#include <stdint.h>
#include <list>

//线程状态
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

//线程属性
typedef enum _THR_FLAG
{
	THR_FLAG_JOIN = 0x0,         //可等待线程
	THR_FLAG_DETACH = 0x1,       //分离线程
}EN_THR_FLAG;

/* 线程信息类 */
struct STThreadDescriptor
{
	bgd_thread_t threadId;                        //线程ID
	EN_THR_FLAG flag;                             //线程属性--是否分离，是否join之类
	void *stack;                                  //线程栈地址
	uint32_t stack_size;                          //线程栈大小
	THREAD_STATUS status;                         //线程的状态(idel,runing)
public:
	STThreadDescriptor() :threadId(0), flag(THR_FLAG_JOIN), stack(0), stack_size(0), status(THR_IDLE) {}
};

/* 线程组信息 */  
struct STThreadGroupDescriptor
{
	int priority;                                               //优先级
	std::list<STThreadDescriptor> thread_list;                  //线程信息
public:
	STThreadGroupDescriptor() :priority(0) {}
};



#endif

