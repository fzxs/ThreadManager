
#ifndef __BGD_FUTURE_H_
#define __BGD_FUTURE_H_

#include "lock.h"

template <class T> class CFuture;

//要求T对象可以new，并且支持拷贝构造方法
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

private:
	 T * m_value;          //结果数据
	 int m_refCount;     //引用次数

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
private:
	CFuture_Rep<T> *m_futureRep;
};


/********************************************************
CFuture_Rep
*********************************************************/

/********************************************************
   Func Name: CFuture_Rep
Date Created: 2018-9-15
 Description: 构造函数
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
 Description: 析构函数
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
   Func Name: set
Date Created: 2018-9-15
 Description: 值输入
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture_Rep<T>::set(const T &r, CFuture<T> &caller)
{
	if (0 == m_value)
	{
		m_mutex->lock();
		if (0 == m_value)
		{
			T * temp = new T(r);
			m_value = temp;
			//激活get
			m_cond->signal();
		}
		m_mutex->unlock();
	}
	return 0;
}

/********************************************************
   Func Name: set
Date Created: 2018-9-15
 Description: 值输入
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture_Rep<T>::get(T &value) const
{
	if (0 == m_value)
	{
		m_mutex->lock();
		m_cond->wait();
		m_mutex->unlock();
		if (0 == m_value)
		{
			return -1;
		}
	}
	value = *m_value;
	return 0;
}

/********************************************************
CFuture
*********************************************************/

/********************************************************
   Func Name: CFuture
Date Created: 2018-9-18
 Description: 构造函数
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
 Description: 析构函数
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
   Func Name: set
Date Created: 2018-9-15
 Description: 值输入
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
 Description: 值输入
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







