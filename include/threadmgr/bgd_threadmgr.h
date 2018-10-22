
#ifndef __BGD_THREADMGR_H_
#define __BGD_THREADMGR_H_

#include <map>
#include <vector>

#include "threadmgr/bgd_common.h"
#include "threadmgr/bgd_descript.h"
#include "threadmgr/lock.h"

#define MAX_THREAD 50
#define MIN_THREAD 5

/* 线程管理器类 */

/* 单例模式 */
class CThreadManager
{
public:
	
	//暂时没有实现
	~CThreadManager() {}

public:

	//设置最大线程数
	void setMaxThread(size_t n);

	//设置最小线程数
	void setMinThread(size_t n);

	//获取实例
	static CThreadManager *instance();

	//创建n个子线程
	int spawn_n(int n_threads, int flag, void *stack[], size_t stack_size[], FuncUser userFunc, void *task);

	//取消子线程
	//int cancel(bgd_thread_t threadid);

	//移除线程
	int removeThr(bgd_thread_t threadId);

	//优化线程池
	int adjustReduce();

	//关闭线程池
	int close();

	//线程检测
	void checkStatus();

	//子线程退出函数
	int atExit();

	//等待子线程退出
	void wait();

private:
	//初始化--暂时没有具体需求
	int init();

	//创建一个子线程
	int spawn_i(int flag,void *stack, size_t stack_size, FuncUser userFunc, void *task);

	//获取当前线程信息
	STThreadDescriptor *getThreadDescriptor(bgd_thread_t threadId);

private:
	CThreadManager() {}

private:
	std::map<bgd_thread_t, STThreadDescriptor *> m_threadPool;      //线程池
	std::vector<STThreadDescriptor *> m_threadRemove;               //待删除线程列表

private:
	size_t m_threadMaxNum;                  //最大线程数量
	size_t m_threadMinNum;                  //最小线程数量

private:
	static CThreadManager *m_instance;
	static CMutexLock *m_mutex;
	static CCondLock *m_joinCond;

};

#endif



