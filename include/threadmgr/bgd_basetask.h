

#ifndef __BGD_BASETASK_H_
#define __BGD_BASETASK_H_

#include "threadmgr/bgd_common.h"
#include "threadmgr/bgd_threadmgr.h"
#include "threadmgr/bgd_descript.h"

#include <stdint.h>

class AbsTaskBase
{
public:
	virtual ~AbsTaskBase(void) {};

public:
	//用户业务处理方法
	virtual THR_FUNC_RETURN srv(void);

	//开启线程池
	virtual int open(uint32_t upperLimit, uint32_t lowerLimit, uint32_t step);

	//申请线程
	virtual int activate();

public:
	//函数指针运行方法--非static无法当做函数指针传递
	static THR_FUNC_RETURN srvRun(void *);

	//获取线程管理器
	CThreadManager *getThreadManager(void);

protected:
	//禁止实例化
	AbsTaskBase(CThreadManager * = 0);

protected:
	CThreadManager *m_thrMgr;             //线程管理器指针
	int _task_no;                         //任务管理器序列号--序列号由线程管理器进行分配

};

#endif




