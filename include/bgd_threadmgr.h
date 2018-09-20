
#ifndef __BGD_THREADMGR_H_
#define __BGD_THREADMGR_H_

#include <map>

#include "bgd_common.h"
#include "bgd_descript.h"
#include "lock.h"

/* 线程管理器类 */

/* 单例模式 */
class CThreadManager
{
public:
	
	//暂时没有实现
	~CThreadManager() {}

public:

	//初始化
	int init();

	//设置最大线程数
	void setMaxThread(size_t n);

	//设置最小线程数
	void setMinThread(size_t n);

	//获取实例
	static CThreadManager *instance();

	//等待所有子线程退出
	void wait(struct timespec *timeout,bool abandonDetached);

	//创建n个子线程
	int spawn_n(int n,FuncUser userFunc, void *taskArg);

	//取消子线程
	int cancel(bgd_thread_t threadid);

	//设置线程状态
	void setThreadStatus(int status);

	//优化线程池
	int adjustReduce();

	//取消所有子线程
	int cancelAll();

	//子线程退出函数
	int atExit(void *object, FUNC_THR_CLEANUP cleanup_hook, void *param) { return 0; };

private:

	//清理线程
	int clearThreadInfo(STThreadDescriptor *pclsDescriptor);

	//创建一个子线程
	int spawn_i(FuncUser userFunc, void *taskArg);

private:
	CThreadManager() {}
private:
	std::map<bgd_thread_t, STThreadDescriptor *> m_threadPool;
	size_t m_threadMaxNum;                  //最大线程数量
	size_t m_threadMinNum;                  //最小线程数量
	size_t m_threadNum;                     //当前实际线程数量

private:
	static CThreadManager *m_instance;
	static CMutexLock *m_mutex;

};

#endif



