

#ifndef __BGD_BASETASK_H_
#define __BGD_BASETASK_H_

#include "bgd_common.h"
#include "bgd_threadmgr.h"
#include "bgd_descript.h"


class AbsTaskBase
{
public:
	AbsTaskBase(CThreadManager * = 0);
	virtual ~AbsTaskBase(void) {};
public:
	//用户业务处理方法
	virtual THR_FUNC_RETURN srv(void);

	//激活任务
	virtual int activate(int nThreads = 1, int flag = THR_JOINING, void *stack[] = 0, size_t stack_size[] = 0);

public:

	//函数指针运行方法--非static无法当做函数指针传递
	static THR_FUNC_RETURN srvRun(void *);

	//获取线程管理器
	CThreadManager *getThreadManager(void);

protected:
	CThreadManager *m_thrMgr;             //线程管理器指针

};

#endif




