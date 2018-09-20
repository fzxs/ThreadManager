
#ifndef __BGD_OBSERVER_H_
#define __BGD_OBSERVER_H_

/* 观察者父类 */

/*
观察者模式相当于回调方法
第一个版本不考虑回调
*/

#include "bgd_future.h"

template<typename T>
class AbsFutureObserver
{
public:
	virtual void update(const CFuture<T> &future) = 0;
};

#endif


