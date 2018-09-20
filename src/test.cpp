
#include <iostream>
#include <unistd.h>

#include "bgd_scheduler.h"
#include "bgd_servant.h"
#include "bgd_future.h"

using namespace std;

//2.定义一个执行任务
class CServant :public AbsServant
{
public:
	virtual int invoke(void *pvArg);
};

int CServant::invoke(void *pvArg)
{
	long a = (long)pvArg;
	sleep(10);
	printf("a = %ld\n", a);

	return 0;
}

#define NUM 12

void test()
{
	//1.定义一个调度器
	CScheduler scheduler(20);
	//激活调度器
	scheduler.activate(1, 10);
	//3.定义结果存储
	CFuture<int> future;
	//4.定义方法对象
	CServant *servant = new CServant();
	//4.定义一个任务请求
	CExecRequest<int> *request = NULL;
	for (int i = 0; i < NUM; i++)
	{
		long k = i + 3;
		request = new CExecRequest<int>(&future, servant, (void *)k);
		//将任务请求加入到调度器
		scheduler.addRequest(request);
	}

	while (true)
	{
		sleep(1000);
	}
	//scheduler.wait();
	printf("1111111\n");
}

int main()
{
	test();
	getchar();
	return 0;
}







