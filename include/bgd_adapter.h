

#ifndef __BGD_ADAPTER_H_
#define __BGD_ADAPTER_H_

#include "bgd_common.h"



/* ���������� */
class CBaseThreadAdapter
{
public:
	CBaseThreadAdapter(FuncUser userFunc, FuncEntrance entryPoint) :m_userFunc(userFunc), m_entryPoint(entryPoint) {}
public:
	virtual THR_FUNC_RETURN invoke(void) = 0;
protected:
	FuncUser m_userFunc;                  //�û�����
	FuncEntrance m_entryPoint;            //��ں���
	void *arg;                            //����ʵ�����
};

/* �������� */
class CThreadAdapter:public CBaseThreadAdapter
{
public:

	virtual THR_FUNC_RETURN invoke(void);

private:

};



#endif


