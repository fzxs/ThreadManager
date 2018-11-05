
#ifndef __BGD_THREADMGR_H_
#define __BGD_THREADMGR_H_

#include <map>
#include <vector>

#include "threadmgr/bgd_common.h"
#include "threadmgr/bgd_descript.h"
#include "threadmgr/lock.h"
#include "threadmgr/bgd_task_descript.h"


#define MAX_THREAD 256        //支持的最大线程数
#define MAX_TASK_NUMBER 64    //支持最大任务数

/* 线程管理器类 */

/* 单例模式 */
class CThreadManager
{
public:
	//暂时没有实现
	~CThreadManager() {}

public:
	//开启任务管理器
	int open(uint32_t upperLimit
		, uint32_t lowerLimit
		, uint32_t step
		, EN_THR_FLAG flag
		, void *stack[]
		, uint32_t stack_size[]
		, FuncUser userFunc
		, void *task);

	//申请默认线程
	int activate(uint32_t group_id);

	//申请自定义线程
	int activate(uint32_t n_threads
		, uint32_t group_id
		, EN_THR_FLAG flag
		, void *stack[]
		, uint32_t stack_size[]
		, FuncUser userFunc
		, void *task);

	//获取实例
	static CThreadManager *instance();

	//创建线程组
	int spawn_n(uint32_t n_threads
		, uint32_t group_id
		, EN_THR_FLAG flag
		, void *stack[]
		, uint32_t stack_size[]
		, FuncUser userFunc
		, void *task);

	//移除线程
	int removeThr(bgd_thread_t threadId);

	//优化线程池
	int adjustReduce(uint32_t group_id);

	//关闭线程池
	int close();

	//线程检测
	void checkStatus();

	//子线程退出函数
	int atExit(long threadId = 0);

	//等待子线程退出
	void wait();

private:
	//初始化--暂时没有具体需求
	int init();

	//创建一个子线程
	long spawn_i(EN_THR_FLAG flag, void *stack, uint32_t stack_size, FuncUser userFunc, void *task);

	//获取当前线程信息
	STThreadDescriptor *getThreadDescriptor(bgd_thread_t threadId);

private:
	CThreadManager() {}

private:
	//bgd_thread_t 线程ID  STThreadDescriptor线程描述信息
	std::map<bgd_thread_t, STThreadDescriptor *> m_threadPool;      //线程池
	std::vector<STThreadDescriptor *> m_threadRemove;               //待删除线程列表

private:
	uint32_t _taskCount;                                          //当前任务个数
	STTaskDescriptor *_taskManager[MAX_TASK_NUMBER];              //任务列表
	//uint32_t 线程组序号（就是任务序号） STThreadGroupDescriptor 线程组信息
	std::map<uint32_t, STThreadGroupDescriptor *> _thread_group;   //线程组信息

private:
	static CThreadManager *m_instance;
	static CMutexLock *m_mutex;
	static CCondLock *m_joinCond;

};

#endif



