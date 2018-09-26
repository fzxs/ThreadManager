
#ifndef __BGD_SERVANT_H_
#define __BGD_SERVANT_H_

template<typename T>
class AbsServant
{
public:
	virtual ~AbsServant() {}
public:
	//需要异步执行的计算
	/*
	为什么没有参数?
	    因为面向对象，数据和计算是封装在一起的，为了减少参数的传递，请将参数封装在AbsServant的派生类中
	*/
	virtual T invoke() = 0;

	//清理工作--派生类如果有自己内存需要释放，请在该release方法中实现
	virtual int release() { return 0; }
};

#endif



