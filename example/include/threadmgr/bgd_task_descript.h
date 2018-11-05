
#ifndef __BGD_TASK_DESCRIPT_H_
#define __BGD_TASK_DESCRIPT_H_

#include "threadmgr/bgd_descript.h"

#include <stdint.h>

#define DEFAULT_THREAD_MAX 50
#define DEFAULT_THREAD_MIN 1

typedef enum _THRPoolModel
{
	THRPool_VARIABLE = 0x0,      //�ɱ��̳߳�ģʽ--�����Զ������̸߳���
	THRPool_FIXED = 0x1,         //�̶��̳߳�ģʽ
}EN_THRPoolModel;


/* �������� */
struct STTaskDescriptor
{
	EN_THRPoolModel model;        //�̳߳�ģʽ
	EN_THR_FLAG flag;             //�߳�����
	uint32_t step;                //�̳߳�������λ
	uint32_t upperLimit;          //�����߳�����
	uint32_t lowerLimit;          //�����߳�����
	void *stack;                  //�߳�ջ��ַ
	uint32_t stack_size;          //�߳�ջ��С
	FuncUser user_func;           //���񷽷�
	void *task;                   //����ʵ��
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

