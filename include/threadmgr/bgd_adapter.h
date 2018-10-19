

#ifndef __BGD_ADAPTER_H_
#define __BGD_ADAPTER_H_

#include "threadmgr/bgd_common.h"
#include <stdlib.h>

extern "C"
{
	extern void * taskEntrance(void *);
}

/* 适配器父类 */
class CBaseThreadAdapter
{
public:
	CBaseThreadAdapter(int flag
		, void *stack
		, size_t stack_size
		, FuncUser userFunc
		, FuncEntrance entryPoint
		, void *arg) 
		:m_flag(flag)
		, m_stack(stack)
		, m_stackSize(stack_size)
		, m_userFunc(userFunc)
		, m_entryPoint(entryPoint)
		, m_arg(arg) {}
	virtual ~CBaseThreadAdapter() {}

public:

	//执行用户方法
	virtual THR_FUNC_RETURN invoke(void) = 0;

	FuncEntrance entryPoint();

protected:
	int m_flag;                             //线程属性参数
	void *m_stack;                          //线程栈设置
	size_t m_stackSize;                     //线程栈大小设置
	FuncUser m_userFunc;                    //用户函数
	FuncEntrance m_entryPoint;              //入口函数
	void *m_arg;                            //任务实体对象
	
};

/* 适配器类 */
class CThreadAdapter:public CBaseThreadAdapter
{
public:

	CThreadAdapter(int flag
		, void *stack
		, size_t stack_size
		, FuncUser userFunc
		, FuncEntrance entryPoint
		, void *arg
	)
		:CBaseThreadAdapter
		(
			flag
			, stack
			, stack_size
			, userFunc
			, entryPoint
			, arg) {}
	virtual ~CThreadAdapter() {}
public:

	virtual THR_FUNC_RETURN invoke(void);

};



#endif


