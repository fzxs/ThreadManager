
#ifndef __BGD_THREADMGR_H_
#define __BGD_THREADMGR_H_

#include <map>

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
	void wait(struct timespec *timeout,bool abandonDetached);

	//����n�����߳�
	int spawn_n(int n,FuncUser userFunc, void *taskArg);

	//ȡ�����߳�
	int cancel(bgd_thread_t threadid);

	//�����߳�״̬
	void setThreadStatus(int status);

	//�Ż��̳߳�
	int adjustReduce();

	//ȡ���������߳�
	int cancelAll();

	//���߳��˳�����
	int atExit(void *object, FUNC_THR_CLEANUP cleanup_hook, void *param) { return 0; };

private:

	//�����߳�
	int clearThreadInfo(STThreadDescriptor *pclsDescriptor);

	//����һ�����߳�
	int spawn_i(FuncUser userFunc, void *taskArg);

private:
	CThreadManager() {}
private:
	std::map<bgd_thread_t, STThreadDescriptor *> m_threadPool;
	size_t m_threadMaxNum;                  //����߳�����
	size_t m_threadMinNum;                  //��С�߳�����
	size_t m_threadNum;                     //��ǰʵ���߳�����

private:
	static CThreadManager *m_instance;
	static CMutexLock *m_mutex;

};

#endif



