
#include <iostream>
#include <unistd.h>
#include <vector>

#include "bgd_scheduler.h"
#include "bgd_servant.h"
#include "bgd_future.h"
#include "bgd_observer.h"
#include "bgd_servant.h"
#include "bgd_proxy.h"

using namespace std;

//2.����һ������
class CServant :public AbsServant<int>
{
public:
	CServant(long arg) :m_arg(arg) {}
public:
	virtual int invoke();
private:
	long m_arg;
};

int CServant::invoke()
{
	sleep(5);
	//printf("i am worker ,i recv a = %ld\n", m_arg);

	return m_arg * 100;
}

//����۲���
class CObserver :public AbsFutureObserver<int>
{
public:
	~CObserver();
public:
	void invoke(const int &data);
};

CObserver::~CObserver()
{
	printf("i am observer , i am release .\n");
}

void CObserver::invoke(const int &data)
{
	printf("i am observer , result is [%d]\n", data);
}

#define NUM 100


void test()
{
	CScheduler scheduler;
	CProxy<int> proxy(&scheduler);
	CObserver *observer = NULL;
	std::vector<CFuture<int> *> futureList;
	std::vector<CFuture<int> *>::iterator it;

	for (int i = 0; i < NUM; i++)
	{
		long k = i + 3;
		CServant *servant = new CServant(k);
		observer = new CObserver();
		futureList.push_back(proxy.servantRegister(servant, observer));
	}

	for (it = futureList.begin(); it != futureList.end(); ++it)
	{
		int k = 0;
		future = *it;
		future->get(k);
		printf("i am get data[%d] and i am [%p]\n", k, future);

		delete future;
	}

	//�̵߳ȴ�
	CThreadManager::instance()->wait();
}

int main()
{
	test();
	getchar();
	return 0;
}







