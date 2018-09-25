
#include <iostream>
#include <unistd.h>

#include "bgd_scheduler.h"
#include "bgd_servant.h"
#include "bgd_future.h"

using namespace std;

//2.����һ��ִ������
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
	//1.����һ��������
	CScheduler scheduler(20);
	//���������
	scheduler.activate(10);
	//3.�������洢
	CFuture<int> future;
	//4.���巽������
	CServant *servant = new CServant();
	//4.����һ����������
	CExecRequest<int> *request = NULL;
	for (int i = 0; i < NUM; i++)
	{
		long k = i + 3;
		request = new CExecRequest<int>(&future, servant, (void *)k);
		//������������뵽������
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







