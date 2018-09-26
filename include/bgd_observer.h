
#ifndef __BGD_OBSERVER_H_
#define __BGD_OBSERVER_H_

/* �۲��߸��� */


#include "bgd_future.h"

template <class T> class CFuture;

/*
֮��������invoke()����������Ϊupdate()�ڼ����Ļ�����ִ�еģ��޷���ͨ��CFuture<T>����ȥ��ȡ����
*/

template<typename T>
class AbsFutureObserver
{
public:
	virtual ~AbsFutureObserver() {}
public:
	//���ݸ���
	void update(const CFuture<T> &future);

	//�û�ֻ�跽��
	virtual void invoke(const T &r) = 0;
};

/********************************************************
   Func Name: update
Date Created: 2018-9-26
 Description: ��������
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
void AbsFutureObserver<T>::update(const CFuture<T> &future)
{
	T data;

	//��ȡ����
	future.getImmediate(data);

	invoke(data);

	//�ͷŹ۲���
	delete this;
}
#endif


