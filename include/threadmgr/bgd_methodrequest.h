

#ifndef __BGD_METHODREQUEST_H_
#define __BGD_METHODREQUEST_H_

#include "threadmgr/bgd_future.h"
#include "threadmgr/bgd_servant.h"

/*�����*/

class AbsMethodRequest
{
public:
	virtual ~AbsMethodRequest() {}
public:
	//���ú���
	virtual int call(void) = 0;
};

#endif

