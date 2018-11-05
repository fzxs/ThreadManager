

#ifndef __BGD_ADAPTER_H_
#define __BGD_ADAPTER_H_

#include "threadmgr/bgd_common.h"
#include <stdlib.h>

extern "C"
{
	extern void * taskEntrance(void *);
}

/* ���������� */
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

	//ִ���û�����
	virtual THR_FUNC_RETURN invoke(void) = 0;

	FuncEntrance entryPoint();

protected:
	int m_flag;                             //�߳����Բ���
	void *m_stack;                          //�߳�ջ����
	size_t m_stackSize;                     //�߳�ջ��С����
	FuncUser m_userFunc;                    //�û�����
	FuncEntrance m_entryPoint;              //��ں���
	void *m_arg;                            //����ʵ�����
	
};

/* �������� */
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


