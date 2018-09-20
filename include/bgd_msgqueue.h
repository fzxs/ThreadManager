
#ifndef __BGD_MSGQUEUE_H_
#define __BGD_MSGQUEUE_H_

#include <queue>

template<typename T>
class CMsgQueue
{
public:
	CMsgQueue(size_t nSize);
public:
	int enqueue(T obj, long timeout = 0);

	int dequeue(T &obj, long timeout = 0);

	int isFull();

private:

	int waitNotFull(long timeout);

	int waitNotEmpty(long timeout);

	int isEmpty();

	void signalDequeueWaiters();

	void signalEnqueueWaiters();

private:

	size_t m_highWaterMark;    // Lowest number before unblocking occurs. --高水位
	size_t m_lowWaterMark;   // Greatest number of bytes before blocking. --低水位
	size_t m_curCount;         // Current number of bytes in the queue.   --当前数量

	CMutexLock *m_mutex;
	CCondLock *m_emptyCond;      //非空条件变量
	CCondLock *m_fullCond;       //非满条件变量
	std::queue<T> m_queue;
};

template<typename T>
CMsgQueue<T>::CMsgQueue(size_t nSize)
{
	m_mutex = new CMutexLock;
	m_emptyCond = new CCondLock(m_mutex);
	m_fullCond = new CCondLock(m_mutex);
	m_highWaterMark = nSize;
	m_lowWaterMark = 0;
	m_curCount = 0;
}

template<typename T>
int CMsgQueue<T>::enqueue(T obj, long timeout)
{
	m_mutex->lock();
	if (waitNotFull(timeout))
	{
		m_mutex->unlock();
		//等待超时，丢弃
		return -1;
	}
	m_queue.push(obj);
	m_curCount++;

	signalDequeueWaiters();
	m_mutex->unlock();

	return 0;
}

template<typename T>
int CMsgQueue<T>::dequeue(T &obj,long timeout)
{
	//T obj;
	m_mutex->lock();

	//判断队列是否为空
	if (waitNotEmpty(timeout))
	{
		m_mutex->unlock();
		return -1;
	}
	obj = m_queue.front();
	m_queue.pop();
	//通知插入
	signalEnqueueWaiters();
	m_curCount--;
	m_mutex->unlock();

	return 0;
}

template<typename T>
int CMsgQueue<T>::isFull()
{
	return m_curCount >= m_highWaterMark;
}

template<typename T>
int CMsgQueue<T>::isEmpty()
{
	return 0 == m_curCount;
}


template<typename T>
void CMsgQueue<T>::signalDequeueWaiters()
{
	m_emptyCond->signal();
}

template<typename T>
void CMsgQueue<T>::signalEnqueueWaiters()
{
	m_fullCond->signal();
}

template<typename T>
int CMsgQueue<T>::waitNotFull(long timeout)
{
	int result = 0;
	//防止虚假信号
	while (isFull())
	{
		if (0 == timeout)
		{
			//一直等待策略
			m_fullCond->wait();
		}
		else
		{
			//超时等待策略
			if (m_fullCond->timedwait(timeout))
			{
				result = -1;
				break;
			}
		}
	}
	return result;
}

template<typename T>
int CMsgQueue<T>::waitNotEmpty(long timeout)
{
	int result = 0;
	//防止虚假信号
	while (isEmpty())
	{
		if (0 == timeout)
		{
			m_emptyCond->wait();
		}
		else
		{
			//超时等待是需要退出的，非超时则不需要，因为用户希望永久等待
			if (m_emptyCond->timedwait(timeout))
			{
				result = -1;
				break;
			}
		}
	}
	return result;
}



#endif


