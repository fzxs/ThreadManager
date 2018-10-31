
#ifndef __BGD_THREADMGR_H_
#define __BGD_THREADMGR_H_

#include <map>
#include <vector>

#include "threadmgr/bgd_common.h"
#include "threadmgr/bgd_descript.h"
#include "threadmgr/lock.h"

#define MAX_THREAD 128
#define MIN_THREAD 5

/* �̹߳������� */

/* ����ģʽ */
class CThreadManager
{
public:
	
	//��ʱû��ʵ��
	~CThreadManager() {}

public:

	//��������߳���
	void setMaxThread(size_t n);

	//������С�߳���
	void setMinThread(size_t n);

	//��ȡʵ��
	static CThreadManager *instance();

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
	//��ʼ��--��ʱû�о�������
	int init();

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



