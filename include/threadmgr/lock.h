
#ifndef __LOCK_H_
#define __LOCK_H_

/*锁模块*/

#include<pthread.h>

//互斥锁
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

//条件锁
class CCondLock
{
public:
	CCondLock(CMutexLock * pclsMutex);
	virtual ~CCondLock();

public:
	//条件等待
	int wait();

	//条件等待
	int timedwait(long sec, long nsec = 0);

	//发送信号
	int signal();

	//广播信号
	int broadcast();

private:
	pthread_cond_t m_cond;
	CMutexLock * m_clsMutex;
	bool m_signalWait;
};


//读写锁
class CReadWriteLock
{
public:
	CReadWriteLock();
	~CReadWriteLock();

public:
	//获取读锁
	void readLock();

	//获取写锁
	void writeLock();

	//释放锁
	void unLock();

private:
	pthread_rwlock_t  m_rwlock;
};

//范围读锁
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

//范围写锁
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























