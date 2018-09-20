

#ifndef __BGD_ADAPTER_H_
#define __BGD_ADAPTER_H_

#include "bgd_common.h"

extern "C"
{
	extern void * taskEntrance(void *);
}

/* 适配器父类 */
class CBaseThreadAdapter
{
public:
	CBaseThreadAdapter(FuncUser userFunc, FuncEntrance entryPoint,void *arg) :m_userFunc(userFunc), m_entryPoint(entryPoint),m_arg(arg) {}
	virtual ~CBaseThreadAdapter() {}

public:

	//执行用户方法
	virtual THR_FUNC_RETURN invoke(void) = 0;

	FuncEntrance entryPoint();

protected:
	FuncUser m_userFunc;                  //用户函数
	FuncEntrance m_entryPoint;            //入口函数
	void *m_arg;                            //任务实体对象
};

/* 适配器类 */
class CThreadAdapter:public CBaseThreadAdapter
{
public:

	CThreadAdapter(FuncUser userFunc, FuncEntrance entryPoint, void *arg) :CBaseThreadAdapter(userFunc,entryPoint,arg) {}
	virtual ~CThreadAdapter() {}
public:

	virtual THR_FUNC_RETURN invoke(void);

private:

};



#endif


