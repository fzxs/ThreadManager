
#include "lock.h"

#include <time.h>

/********************************************************
   Func Name: CMutexLock
Date Created: 2018-7-5
 Description: 构造函数
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
CMutexLock::CMutexLock()
{
	//暂时不需要设计mutexattr，以后有需要会让外部传参设置mutexattr
	pthread_mutexattr_t *mutexattr = NULL;
	//初始化锁
	pthread_mutex_init(&m_mutex, mutexattr);
}

/********************************************************
   Func Name: ~CMutexLock
Date Created: 2018-7-5
 Description: 析构函数
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
CMutexLock::~CMutexLock()
{
	pthread_mutex_destroy(&m_mutex);
}

/********************************************************
   Func Name: lock
Date Created: 2018-7-5
 Description: 加锁
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
void CMutexLock::lock()
{
	pthread_mutex_lock(&m_mutex);
}

/********************************************************
   Func Name: unlock
Date Created: 2018-7-5
 Description: 解锁
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
void CMutexLock::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}

/********************************************************

CCondLock类

*********************************************************/

/********************************************************
   Func Name: CCondLock
Date Created: 2018-7-5
 Description: 构造函数
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
CCondLock::CCondLock(CMutexLock * pclsMutex):m_clsMutex(pclsMutex)
{
	pthread_condattr_t *cond_attr = NULL;
	pthread_cond_init(&m_cond, cond_attr);
	m_signalWait = false;
}

/********************************************************
   Func Name: ~CCondLock
Date Created: 2018-7-5
 Description: 析构函数
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
CCondLock::~CCondLock()
{
	pthread_cond_destroy(&m_cond);
}

/********************************************************
   Func Name: wait
Date Created: 2018-7-5
 Description: 条件等待
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CCondLock::wait()
{
	int result = 0;
	//因为wait需要加锁，所以不需要担心设置完m_signalWait会立刻发送信号
	if (!m_signalWait)
	{
		m_signalWait = true;
	}
	result = pthread_cond_wait(&m_cond, &(m_clsMutex->m_mutex));

	return result;
}

/********************************************************
   Func Name: timedwait
Date Created: 2018-7-5
 Description: 超时条件等待
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CCondLock::timedwait(long sec, long nsec)
{
	struct timespec ts;
	int result = 0;

	ts.tv_sec = sec;
	ts.tv_nsec = nsec;

	if (!m_signalWait)
	{
		m_signalWait = true;
	}
	clock_gettime(CLOCK_REALTIME, &ts);
	/*
	pthread_cond_timedwait的第三个参数获取的是绝对时间，并非相对时间
	*/
	ts.tv_sec += sec;
	ts.tv_nsec += nsec;
	result = pthread_cond_timedwait(&m_cond, &(m_clsMutex->m_mutex), &ts);

	return result;

}

/********************************************************
   Func Name: signal
Date Created: 2018-7-5
 Description: 发送信号
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CCondLock::signal()
{
	int result = 0;

	//防止信号丢失
	if (!m_signalWait)
	{
		return -1;
	}
	result = pthread_cond_signal(&m_cond);

	return result;
}

/********************************************************
   Func Name: broadcast
Date Created: 2018-9-21
 Description: 广播信号
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
int CCondLock::broadcast()
{
	return pthread_cond_broadcast(&m_cond);
}

/********************************************************

CReadWriteLock类

*********************************************************/

/********************************************************
   Func Name: CReadWriteLock
Date Created: 2018-7-5
 Description: 构造函数
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
CReadWriteLock::CReadWriteLock()
{
	pthread_rwlockattr_t *attr = NULL;
	pthread_rwlock_init(&m_rwlock, attr);
}

/********************************************************
   Func Name: ~CReadWriteLock
Date Created: 2018-7-5
 Description: 析构函数
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
CReadWriteLock::~CReadWriteLock()
{
	pthread_rwlock_destroy(&m_rwlock);
}

/********************************************************
   Func Name: readLock
Date Created: 2018-7-5
 Description: 获取读锁
       Input: 
      Output: 
      Return: 
     Caution:     如果其它一个线程占有写锁，则当前线程必须
	          等待写锁被释放，才能对保护资源进行访问 
*********************************************************/
void CReadWriteLock::readLock()
{
	pthread_rwlock_rdlock(&m_rwlock);
}

/********************************************************
   Func Name: writeLock
Date Created: 2018-7-5
 Description: 获取写锁
       Input: 
      Output: 
      Return: 
     Caution:  
*********************************************************/
void CReadWriteLock::writeLock()
{
	pthread_rwlock_wrlock(&m_rwlock);
}

/********************************************************
   Func Name: unLock
Date Created: 2018-7-5
 Description: 释放写锁
       Input: 
      Output: 
      Return: 
     Caution:  
*********************************************************/
void CReadWriteLock::unLock()
{
	pthread_rwlock_unlock(&m_rwlock);
}






































