
#ifndef __BGD_TASK_DESCRIPT_H_
#define __BGD_TASK_DESCRIPT_H_

#include "threadmgr/bgd_descript.h"

#include <stdint.h>

#define DEFAULT_THREAD_MAX 50
#define DEFAULT_THREAD_MIN 1

typedef enum _THRPoolModel
{
	THRPool_VARIABLE = 0x0,      //可变线程池模式--可以自动调节线程个数
	THRPool_FIXED = 0x1,         //固定线程池模式
}EN_THRPoolModel;


/* 任务属性 */
struct STTaskDescriptor
{
	EN_THRPoolModel model;        //线程池模式
	EN_THR_FLAG flag;             //线程属性
	uint32_t step;                //线程池增长单位
	uint32_t upperLimit;          //任务线程上限
	uint32_t lowerLimit;          //任务线程下限
	void *stack;                  //线程栈地址
	uint32_t stack_size;          //线程栈大小
	FuncUser user_func;           //任务方法
	void *task;                   //任务实例
public:
	STTaskDescriptor() :model(THRPool_VARIABLE)
		, flag(THR_FLAG_JOIN)
		, step(1)
		, upperLimit(DEFAULT_THREAD_MAX)
		, lowerLimit(DEFAULT_THREAD_MIN)
		, stack(0)
		, stack_size(0)
		, user_func(0)
		, task(0)
	{}
};

#endif

