

#ifndef __BGD_METHODREQUEST_H_
#define __BGD_METHODREQUEST_H_

#include "threadmgr/bgd_future.h"

/*命令方法*/

class AbsMethodRequest
{
public:
	virtual ~AbsMethodRequest() {}
public:
	//调用函数
	virtual int call(void) = 0;
};

#endif

