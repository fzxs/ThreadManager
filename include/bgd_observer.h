
#ifndef __BGD_OBSERVER_H_
#define __BGD_OBSERVER_H_

/* �۲��߸��� */

/*
�۲���ģʽ�൱�ڻص�����
��һ���汾�����ǻص�
*/

#include "bgd_future.h"

template<typename T>
class AbsFutureObserver
{
public:
	virtual void update(const CFuture<T> &future) = 0;
};

#endif


