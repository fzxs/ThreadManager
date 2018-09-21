
#ifndef __BGD_THREADMGR_H_
#define __BGD_THREADMGR_H_

#include <map>
#include <vector>

#include "bgd_common.h"
#include "bgd_descript.h"
#include "lock.h"

/* �̹߳������� */

/* ����ģʽ */
class CThreadManager
{
public:
	
	//��ʱû��ʵ��
	~CThreadManager() {}

public:

	//��ʼ��
	int init();

	//��������߳���
	void setMaxThread(size_t n);

	//������С�߳���
	void setMinThread(size_t n);

	//��ȡʵ��
	static CThreadManager *instance();

	//�ȴ��������߳��˳�
	//void wait(struct timespec *timeout,bool abandonDetached);

	//����n�����߳�
	int spawn_n(int n_threads, int flag, void *stack[], size_t stack_size[], FuncUser userFunc, void *task);

	//ȡ�����߳�
	//int cancel(bgd_thread_t threadid);

	//�Ƴ��߳�
	int removeThr(bgd_thread_t threadId);

	//�Ż��̳߳�
	int adjustReduce();

	//�ر��̳߳�
	int close();

	//�̼߳��
	void checkStatus();

	//���߳��˳�����
	int atExit();

	//�ȴ����߳��˳�
	void wait();

private:
	//����һ�����߳�
	int spawn_i(int flag,void *stack, size_t stack_size, FuncUser userFunc, void *task);

	//��ȡ��ǰ�߳���Ϣ
	STThreadDescriptor *getThreadDescriptor(bgd_thread_t threadId);

private:
	CThreadManager() {}

private:
	std::map<bgd_thread_t, STThreadDescriptor *> m_threadPool;      //�̳߳�
	std::vector<STThreadDescriptor *> m_threadRemove;               //��ɾ���߳��б�

private:
	size_t m_threadMaxNum;                  //����߳�����
	size_t m_threadMinNum;                  //��С�߳�����

private:
	static CThreadManager *m_instance;
	static CMutexLock *m_mutex;
	static CCondLock *m_joinCond;

};

#endif



