
#ifndef __BGD_OBSERVER_H_
#define __BGD_OBSERVER_H_

/* π€≤Ï’ﬂ∏∏¿‡ */


#include "bgd_future.h"

template <class T> class CFuture;

template<typename T>
class AbsFutureObserver
{
public:
	virtual void update(const CFuture<T> &future) = 0;
};

#endif


