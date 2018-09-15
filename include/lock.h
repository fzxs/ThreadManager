
#ifndef __LOCK_H_
#define __LOCK_H_

/*��ģ��*/

#include<pthread.h>

//������
class CMutexLock
{
public:
	friend class CCondLock;
public:
	CMutexLock();
	virtual ~CMutexLock();
	
public:
	void lock();
	void unlock();

protected:
	pthread_mutex_t m_mutex;
};

//������
class CCondLock
{
public:
	CCondLock(CMutexLock * pclsMutex);
	virtual ~CCondLock();

public:
	//�����ȴ�
	void wait();

	//�����ź�
	void signal();

private:
	pthread_cond_t m_cond;
	CMutexLock * m_clsMutex;
};


//��д��
class CReadWriteLock
{
public:
	CReadWriteLock();
	~CReadWriteLock();

public:
	//��ȡ����
	void readLock();

	//��ȡд��
	void writeLock();

	//�ͷ���
	void unLock();

private:
	pthread_rwlock_t  m_rwlock;
};

//��Χ����
class CScopedReadLock
{
public:
	CScopedReadLock(CReadWriteLock *mutex)
	{
		m_mutex = mutex;
		m_mutex->readLock();
	}
	~CScopedReadLock()
	{
		m_mutex->unLock();
	}
private:
	CReadWriteLock *m_mutex;
};

//��Χд��
class CScopedWriteLock
{
public:
	CScopedWriteLock(CReadWriteLock *mutex)
	{
		m_mutex = mutex;
		m_mutex->writeLock();
	}
	~CScopedWriteLock()
	{
		m_mutex->unLock();
	}
private:
	CReadWriteLock *m_mutex;
};













#endif























