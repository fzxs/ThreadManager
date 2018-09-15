

#ifndef __BGD_ADAPTER_H_
#define __BGD_ADAPTER_H_

#include "bgd_common.h"



/* 适配器父类 */
class CBaseThreadAdapter
{
public:
	CBaseThreadAdapter(FuncUser userFunc, FuncEntrance entryPoint) :m_userFunc(userFunc), m_entryPoint(entryPoint) {}
public:
	virtual THR_FUNC_RETURN invoke(void) = 0;
protected:
	FuncUser m_userFunc;                  //用户函数
	FuncEntrance m_entryPoint;            //入口函数
	void *arg;                            //任务实体对象
};

/* 适配器类 */
class CThreadAdapter:public CBaseThreadAdapter
{
public:

	virtual THR_FUNC_RETURN invoke(void);

private:

};



#endif


