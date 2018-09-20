
#ifndef __BGD_SERVANT_H_
#define __BGD_SERVANT_H_

class AbsServant
{
public:
	//需要异步执行的计算
	virtual int invoke(void *pvArg) = 0;
};

#endif



