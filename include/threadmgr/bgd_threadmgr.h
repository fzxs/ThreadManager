
#ifndef __BGD_THREADMGR_H_
#define __BGD_THREADMGR_H_

#include <map>
#include <vector>

#include "threadmgr/bgd_common.h"
#include "threadmgr/bgd_descript.h"
#include "threadmgr/lock.h"
#include "threadmgr/bgd_task_descript.h"


#define MAX_THREAD 256        //֧�ֵ�����߳���
#define MAX_TASK_NUMBER 64    //֧�����������

/* �̹߳������� */

/* ����ģʽ */
class CThreadManager
{
public:
	//��ʱû��ʵ��
	~CThreadManager() {}

public:
	//�������������
	int open(uint32_t upperLimit
		, uint32_t lowerLimit
		, uint32_t step
		, EN_THR_FLAG flag
		, void *stack[]
		, uint32_t stack_size[]
		, FuncUser userFunc
		, void *task);

	//����Ĭ���߳�
	int activate(uint32_t group_id);

	//�����Զ����߳�
	int activate(uint32_t n_threads
		, uint32_t group_id
		, EN_THR_FLAG flag
		, void *stack[]
		, uint32_t stack_size[]
		, FuncUser userFunc
		, void *task);

	//��ȡʵ��
	static CThreadManager *instance();

	//�����߳���
	int spawn_n(uint32_t n_threads
		, uint32_t group_id
		, EN_THR_FLAG flag
		, void *stack[]
		, uint32_t stack_size[]
		, FuncUser userFunc
		, void *task);

	//�Ƴ��߳�
	int removeThr(bgd_thread_t threadId);

	//�Ż��̳߳�
	int adjustReduce(uint32_t group_id);

	//�ر��̳߳�
	int close();

	//�̼߳��
	void checkStatus();

	//���߳��˳�����
	int atExit(long threadId = 0);

	//�ȴ����߳��˳�
	void wait();

private:
	//��ʼ��--��ʱû�о�������
	int init();

	//����һ�����߳�
	long spawn_i(EN_THR_FLAG flag, void *stack, uint32_t stack_size, FuncUser userFunc, void *task);

	//��ȡ��ǰ�߳���Ϣ
	STThreadDescriptor *getThreadDescriptor(bgd_thread_t threadId);

private:
	CThreadManager() {}

private:
	//bgd_thread_t �߳�ID  STThreadDescriptor�߳�������Ϣ
	std::map<bgd_thread_t, STThreadDescriptor *> m_threadPool;      //�̳߳�
	std::vector<STThreadDescriptor *> m_threadRemove;               //��ɾ���߳��б�

private:
	uint32_t _taskCount;                                          //��ǰ�������
	STTaskDescriptor *_taskManager[MAX_TASK_NUMBER];              //�����б�
	//uint32_t �߳�����ţ�����������ţ� STThreadGroupDescriptor �߳�����Ϣ
	std::map<uint32_t, STThreadGroupDescriptor *> _thread_group;   //�߳�����Ϣ

private:
	static CThreadManager *m_instance;
	static CMutexLock *m_mutex;
	static CCondLock *m_joinCond;

};

#endif



