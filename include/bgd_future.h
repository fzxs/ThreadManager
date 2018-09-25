
#ifndef __BGD_FUTURE_H_
#define __BGD_FUTURE_H_

#include <iostream>
#include <list>

#include "lock.h"
#include "bgd_observer.h"

template <class T> class CFuture;

//Ҫ��T�������new������֧�ֿ������췽��
template<typename T>
class CFuture_Rep
{
private:
	friend class CFuture<T>;
private:
	CFuture_Rep();
	~CFuture_Rep();

private:
	int set(const T &r, CFuture<T> &caller);

	int get(T &value) const;

	//��ӹ۲���
	int attach(AbsFutureObserver<T> *observer, CFuture<T> &caller);

	//ɾ���۲���
	int detach(AbsFutureObserver<T> *observer);

private:
	 T * m_value;                                              //�������
	 int m_refCount;                                           //���ô���
	 std::list<AbsFutureObserver<T> *> m_observers;            //�۲����б�

private:
	CMutexLock *m_mutex;
	CCondLock *m_cond;

};

template<typename T>
class CFuture
{
public:
	CFuture();
	~CFuture();
public:
	int set (const T &r);

	int get (T &value) const;

	//��ӹ۲���
	int attach(AbsFutureObserver<T> *observer);

	//ɾ���۲���
	int detach (AbsFutureObserver<T> *observer);

private:
	CFuture_Rep<T> *m_futureRep;
};


/********************************************************
CFuture_Rep
*********************************************************/

/********************************************************
   Func Name: CFuture_Rep
Date Created: 2018-9-15
 Description: ���캯��
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
CFuture_Rep<T>::CFuture_Rep()
{
	m_mutex = new CMutexLock;
	m_cond = new CCondLock(m_mutex);
}

/********************************************************
   Func Name: ~CFuture_Rep
Date Created: 2018-9-15
 Description: ��������
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
CFuture_Rep<T>::~CFuture_Rep()
{
	delete m_value;
	m_value = NULL;

	delete m_cond;
	m_cond = NULL;

	delete m_mutex;
	m_mutex = NULL;
}

/********************************************************
   Func Name: attach
Date Created: 2018-9-15
 Description: ��ӹ۲���
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture_Rep<T>::attach(AbsFutureObserver<T> *observer, CFuture<T> &caller)
{
	if (NULL == observer)
	{
		return -1;
	}
	m_mutex->lock();
	if (0 == m_value)
	{
		m_observers.push_back(observer);
	}
	else
	{
		observer->update(caller);
	}
	m_mutex->unlock();

	return 0;
}

/********************************************************
   Func Name: attach
Date Created: 2018-9-15
 Description: ɾ���۲���
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture_Rep<T>::detach(AbsFutureObserver<T> *observer)
{
	typename std::list<AbsFutureObserver<T> *>::iterator itFind;

	if (NULL == observer)
	{
		return -1;
	}
	m_mutex->lock();
	itFind = std::find(m_observers.begin(), m_observers.end(), observer);
	if (itFind != m_observers.end())
	{
		m_observers.erase(itFind);
	}
	m_mutex->unlock();

	return 0;
}

/********************************************************
   Func Name: set
Date Created: 2018-9-15
 Description: ֵ����
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture_Rep<T>::set(const T &r, CFuture<T> &caller)
{
	typename std::list<AbsFutureObserver<T> *>::iterator it;
	AbsFutureObserver<T> * observer = NULL;

	if (0 == m_value)
	{
		m_mutex->lock();
		if (0 == m_value)
		{
			T * temp = new T(r);
			m_value = temp;
			//����۲���
			for (it = m_observers.begin(); it != m_observers.end(); ++it)
			{
				observer = *it;
				observer->update(caller);
			}
			//����get
			m_cond->signal();
		}
		m_mutex->unlock();
	}
	return 0;
}

/********************************************************
   Func Name: set
Date Created: 2018-9-15
 Description: ֵ����
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture_Rep<T>::get(T &value) const
{
	m_mutex->lock();
	if (0 == m_value)
	{
		m_cond->wait();
		
		if (0 == m_value)
		{
			return -1;
		}
	}
	value = *m_value;
	m_mutex->unlock();
	return 0;
}

/********************************************************
CFuture
*********************************************************/

/********************************************************
   Func Name: CFuture
Date Created: 2018-9-18
 Description: ���캯��
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
CFuture<T>::CFuture()
{
	m_futureRep = new CFuture_Rep<T>();
}

/********************************************************
   Func Name: ~CFuture
Date Created: 2018-9-19
 Description: ��������
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
CFuture<T>::~CFuture()
{
	delete m_futureRep;
	m_futureRep = NULL;
}

/********************************************************
   Func Name: attach
Date Created: 2018-9-15
 Description: ��ӹ۲���
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture<T>::attach(AbsFutureObserver<T> *observer)
{
	return m_futureRep->attach(observer, *this);
}

/********************************************************
   Func Name: detach
Date Created: 2018-9-15
 Description: ɾ���۲���
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture<T>::detach(AbsFutureObserver<T> *observer)
{
	return m_futureRep->detach(observer);
}

/********************************************************
   Func Name: set
Date Created: 2018-9-15
 Description: ֵ����
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture<T>::set(const T &r)
{
	return m_futureRep->set(r, *this);
}

/********************************************************
   Func Name: get
Date Created: 2018-9-18
 Description: ֵ����
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture<T>::get(T &value) const
{
	return m_futureRep->get(value);
}


#endif







