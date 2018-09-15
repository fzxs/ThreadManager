
#include "lock.h"

/********************************************************
   Func Name: CMutexLock
Date Created: 2018-7-5
 Description: ���캯��
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
CMutexLock::CMutexLock()
{
	//��ʱ����Ҫ���mutexattr���Ժ�����Ҫ�����ⲿ��������mutexattr
	pthread_mutexattr_t *mutexattr = NULL;
	//��ʼ����
	pthread_mutex_init(&m_mutex, mutexattr);
}

/********************************************************
   Func Name: ~CMutexLock
Date Created: 2018-7-5
 Description: ��������
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
 Description: ����
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
 Description: ����
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

CCondLock��

*********************************************************/

/********************************************************
   Func Name: CCondLock
Date Created: 2018-7-5
 Description: ���캯��
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
CCondLock::CCondLock(CMutexLock * pclsMutex):m_clsMutex(pclsMutex)
{
	pthread_condattr_t *cond_attr = NULL;
	pthread_cond_init(&m_cond, cond_attr);
}

/********************************************************
   Func Name: ~CCondLock
Date Created: 2018-7-5
 Description: ��������
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
 Description: �����ȴ�
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
void CCondLock::wait()
{
	pthread_cond_wait(&m_cond, &(m_clsMutex->m_mutex));
}

/********************************************************
   Func Name: signal
Date Created: 2018-7-5
 Description: �����ź�
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
void CCondLock::signal()
{
	pthread_cond_signal(&m_cond);
}

/********************************************************

CReadWriteLock��

*********************************************************/

/********************************************************
   Func Name: CReadWriteLock
Date Created: 2018-7-5
 Description: ���캯��
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
 Description: ��������
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
 Description: ��ȡ����
       Input: 
      Output: 
      Return: 
     Caution:     �������һ���߳�ռ��д������ǰ�̱߳���
	          �ȴ�д�����ͷţ����ܶԱ�����Դ���з��� 
*********************************************************/
void CReadWriteLock::readLock()
{
	pthread_rwlock_rdlock(&m_rwlock);
}

/********************************************************
   Func Name: writeLock
Date Created: 2018-7-5
 Description: ��ȡд��
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
 Description: �ͷ�д��
       Input: 
      Output: 
      Return: 
     Caution:  
*********************************************************/
void CReadWriteLock::unLock()
{
	pthread_rwlock_unlock(&m_rwlock);
}






































