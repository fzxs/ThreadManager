
#ifndef __BGD_OBSERVER_H_
#define __BGD_OBSERVER_H_

/* 观察者父类 */


#include "bgd_future.h"

template <class T> class CFuture;

/*
之所以设置invoke()方法，是因为update()在加锁的环境下执行的，无法再通过CFuture<T>对象去获取数据
*/

template<typename T>
class AbsFutureObserver
{
public:
	virtual ~AbsFutureObserver() {}
public:
	//数据更新
	void update(const CFuture<T> &future);

	//用户只需方法
	virtual void invoke(const T &r) = 0;
};

/********************************************************
   Func Name: update
Date Created: 2018-9-26
 Description: 更新数据
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
void AbsFutureObserver<T>::update(const CFuture<T> &future)
{
	T data;

	//获取数据
	future.getImmediate(data);

	invoke(data);

	//释放观察者
	delete this;
}
#endif


