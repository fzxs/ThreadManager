

#ifndef __BGD_ADAPTER_H_
#define __BGD_ADAPTER_H_

#include "bgd_common.h"

extern "C"
{
	extern void * taskEntrance(void *);
}

/* ���������� */
class CBaseThreadAdapter
{
public:
	CBaseThreadAdapter(FuncUser userFunc, FuncEntrance entryPoint,void *arg) :m_userFunc(userFunc), m_entryPoint(entryPoint),m_arg(arg) {}
	virtual ~CBaseThreadAdapter() {}

public:

	//ִ���û�����
	virtual THR_FUNC_RETURN invoke(void) = 0;

	FuncEntrance entryPoint();

protected:
	FuncUser m_userFunc;                  //�û�����
	FuncEntrance m_entryPoint;            //��ں���
	void *m_arg;                            //����ʵ�����
};

/* �������� */
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


