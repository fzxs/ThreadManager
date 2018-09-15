

#ifndef __BGD_DESCRIPT_H_
#define __BGD_DESCRIPT_H_

#include "bgd_queue.h"

#ifdef WIN
#define pthread_t int 
#endif 

typedef pthread_t bgd_thread_t;

/* 线程信息类 */
struct STThreadDescriptorHeader
{
public:
	bgd_thread_t threadId;                      //线程ID
	int flag;                                   //线程属性--是否分离，是否join之类
	int priority;                               //优先级
	int groupId;                                //线程组ID
	char *threadName;                           //线程名字
	void *stack;                                //线程栈地址
	unsigned int stackSize;                     //线程栈大小
	int status;                                 //线程的状态(idel,runing)
};

/* 线程信息头类 */
struct STThreadDescriptor
{
public:
	int priority;                                               //优先级
	int groupId;                                                //线程组ID
	TemplateTaskQueue<STThreadDescriptorHeader *> threadGroup;  //线程组队列
};



#endif

