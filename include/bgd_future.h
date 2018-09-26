
#ifndef __BGD_FUTURE_H_
#define __BGD_FUTURE_H_

#include <iostream>
#include <list>

#include "lock.h"
#include "bgd_observer.h"

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

	//立刻获取数据
	int getImmediate(T &value) const;

	//添加观察者
	int attach(AbsFutureObserver<T> *observer, CFuture<T> &caller);

	//删除观察者
	int detach(AbsFutureObserver<T> *observer);

private:
	//创建CFuture_Rep对象
	static CFuture_Rep<T> * create(void); 

	//添加引用
	static CFuture_Rep<T> * refAttach(CFuture_Rep<T> * &rep);

	//删除引用
	static void refdetach(CFuture_Rep<T> * &rep);

	//分配引用
	static void assign (CFuture_Rep<T> *&rep, CFuture_Rep<T> *new_rep);

private:
	 T * m_value;                                              //结果数据
	 int m_refCount;                                           //引用次数
	 std::list<AbsFutureObserver<T> *> m_observers;            //观察者列表

private:
	CMutexLock *m_mutex;
	CCondLock *m_cond;

};

template<typename T>
class CFuture
{
public:
	CFuture();
	CFuture(const CFuture<T> &r);
	~CFuture();
public:
	//设置数据
	int set (const T &r);

	//等待获取数据
	int get (T &value) const;

	//立刻获取数据
	int getImmediate(T &value) const;

	//添加观察者
	int attach(AbsFutureObserver<T> *observer);

	//删除观察者
	int detach (AbsFutureObserver<T> *observer);

	//重载=
	void operator=(const CFuture<T> &r);

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
CFuture_Rep<T>::CFuture_Rep():m_value(0),m_refCount(0)
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
   Func Name: create
Date Created: 2018-9-15
 Description: 创建CFuture_Rep对象实例
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
CFuture_Rep<T> * CFuture_Rep<T>::create(void)
{
	CFuture_Rep<T> * temp = new CFuture_Rep<T>();
	return temp;
}

/********************************************************
   Func Name: refAttach
Date Created: 2018-9-15
 Description: 添加引用
       Input: 
      Output: 
      Return: 
     Caution: 静态函数，不要使用实例属性
*********************************************************/
template<typename T>
CFuture_Rep<T> * CFuture_Rep<T>::refAttach(CFuture_Rep<T> * &rep)
{
	if (NULL == rep)
	{
		return NULL;
	}

	//用自己的锁锁自己
	rep->m_mutex->lock();
	rep->m_refCount++;
	rep->m_mutex->unlock();

	return rep;
}

/********************************************************
   Func Name: refdetach
Date Created: 2018-9-15
 Description: 删除引用
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
void CFuture_Rep<T>::refdetach(CFuture_Rep<T> * &rep)
{
	if (NULL == rep)
	{
		return;
	}
	//自己的锁 锁自己
	rep->m_mutex->lock();
	if (0 == rep->m_refCount--)
	{
		//释放对象前，先把锁解开
		rep->m_mutex->unlock();
		delete rep;
		rep = NULL;

		return;
	}
	rep->m_mutex->unlock();
}

/********************************************************
   Func Name: assign
Date Created: 2018-9-15
 Description: 分配引用
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
void CFuture_Rep<T>::assign(CFuture_Rep<T> *&rep, CFuture_Rep<T> *new_rep)
{
	CFuture_Rep<T> *old = NULL;
	if (NULL == rep || NULL == new_rep)
	{
		return;
	}
	old = rep;
	//将原来的引用替换掉
	rep = new_rep;
	//将原来的引用计数器减一，因为已经被替换掉了
	//自己的锁 锁自己
	old->m_mutex->lock();
	if (0 == old->m_refCount--)
	{
		//释放对象前，先把锁解开
		old->m_mutex->unlock();
		delete old;
		old = NULL;

		return;
	}
	old->m_mutex->unlock();
}

/********************************************************
   Func Name: attach
Date Created: 2018-9-15
 Description: 添加观察者
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
 Description: 删除观察者
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
 Description: 值输入
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
			//激活观察者
			for (it = m_observers.begin(); it != m_observers.end(); ++it)
			{
				observer = *it;
				observer->update(caller);
			}
			//激活get
			m_cond->signal();
		}
		m_mutex->unlock();
	}
	
	return 0;
}

/********************************************************
   Func Name: get
Date Created: 2018-9-15
 Description: 获取数据
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
			m_mutex->unlock();
			return -1;
		}
	}
	value = *m_value;
	m_mutex->unlock();
	return 0;
}

/********************************************************
   Func Name: get
Date Created: 2018-9-15
 Description: 获取数据
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture_Rep<T>::getImmediate(T &value) const
{
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
CFuture<T>::CFuture():m_futureRep(CFuture_Rep<T>::create())
{
}

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
CFuture<T>::CFuture(const CFuture<T> &r):m_futureRep(CFuture_Rep<T>::refAttach(r.m_futureRep))
{
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
	//本身引用计数减一
	CFuture_Rep<T>::refdetach(m_futureRep);
}

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
void CFuture<T>::operator=(const CFuture<T> &r)
{
	//右值的引用计数加一
	//CFuture_Rep<T>::refAttach(r.m_futureRep)
	//替换本身的引用
	CFuture_Rep<T>::assign(this->m_futureRep, CFuture_Rep<T>::refAttach(r.m_futureRep));
}

/********************************************************
   Func Name: attach
Date Created: 2018-9-15
 Description: 添加观察者
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
 Description: 删除观察者
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

/********************************************************
   Func Name: getImmediate
Date Created: 2018-9-15
 Description: 立刻获取数据
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
int CFuture<T>::getImmediate(T &value) const
{
	return m_futureRep->getImmediate(value);
}


#endif







