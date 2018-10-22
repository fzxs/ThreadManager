
#ifdef TEST

#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;
//
////2.定义一个仆人
//class CServant :public AbsServant<int>
//{
//public:
//	CServant(long arg) :m_arg(arg) {}
//public:
//	virtual int invoke();
//private:
//	long m_arg;
//};
//
//int CServant::invoke()
//{
//	sleep(5);
//	//printf("i am worker ,i recv a = %ld\n", m_arg);
//
//	return m_arg * 100;
//}
//
////定义观察者
//class CObserver :public AbsFutureObserver<int>
//{
//public:
//	~CObserver();
//public:
//	void invoke(const int &data);
//};
//
//CObserver::~CObserver()
//{
//	printf("i am observer , i am release .\n");
//}
//
//void CObserver::invoke(const int &data)
//{
//	printf("i am observer , result is [%d]\n", data);
//}
//
//#define NUM 100
//
//
//void test()
//{
//	CScheduler scheduler;
//	CProxy<int> proxy(&scheduler);
//	std::vector<CFuture<int> *> futureList;
//	std::vector<CFuture<int> *>::iterator it;
//
//	for (int i = 0; i < NUM; i++)
//	{
//		long k = i + 3;
//		CServant *servant = new CServant(k);
//		futureList.push_back(proxy.registerTask(servant));
//	}
//
//	for (it = futureList.begin(); it != futureList.end(); ++it)
//	{
//		int k = 0;
//		CFuture<int> * future = *it;
//		future->get(k);
//		printf("i am get data[%d] and i am [%p]\n", k, future);
//
//		delete future;
//	}
//
//	//线程等待
//	CThreadManager::instance()->wait();
//}

void test()
{
}

int main()
{
	test();
	getchar();
	return 0;
}


#endif




