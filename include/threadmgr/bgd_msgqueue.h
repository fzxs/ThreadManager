
#ifndef __BGD_MSGQUEUE_H_
#define __BGD_MSGQUEUE_H_

#include <queue>

template<typename T>
class CMsgQueue
{
public:
	CMsgQueue(size_t nSize);
public:
	/*
	enqueue
	@timeout�������Ϣ����
	   0��ʾ��Ϣ���������ֱ�Ӿܾ�;
	   -1��ʾ��Ϣ���������һֱ�ȴ�;
	   >0��ʾ��Ϣ��������󣬳�ʱ�ȴ�
	*/
	int enqueue(T obj, long timeout);

	/*
	dequeue
	@timeout����ȡ��Ϣ����
	   0��ʾ��Ϣ����Ϊ��ʱ��ֱ�ӷ��ش���;
	   -1��ʾ��Ϣ����Ϊ��ʱ��һֱ�ȴ��������;
	   >0��ʾ��Ϣ����Ϊ��ʱ����ʱ�ȴ�
	*/
	int dequeue(T &obj, long timeout);

	int isFull();

	int isEmpty();

private:

	int waitNotFull(long timeout);

	int waitNotEmpty(long timeout);

	void signalDequeueWaiters();

	void signalEnqueueWaiters();

private:

	size_t m_highWaterMark;    // Lowest number before unblocking occurs. --��ˮλ
	size_t m_lowWaterMark;   // Greatest number of bytes before blocking. --��ˮλ
	size_t m_curCount;         // Current number of bytes in the queue.   --��ǰ����

	CMutexLock *m_mutex;
	CCondLock *m_emptyCond;      //�ǿ���������
	CCondLock *m_fullCond;       //������������
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
		//�ȴ���ʱ������
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
	
	//�ж϶����Ƿ�Ϊ��
	if (waitNotEmpty(timeout))
	{
		m_mutex->unlock();
		return -1;
	}
	obj = m_queue.front();
	m_queue.pop();

	//֪ͨ����
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
	//��ֹ����ź�
	while (isFull())
	{
		if (0 == timeout)
		{
			result = -1;
			break;
		}else if (timeout < 0)
		{
			// һֱ�ȴ�����
			m_fullCond->wait();
		}
		else
		{
			//��ʱ�ȴ����� timeout > 0
			//��ʱ�ȴ�����Ҫ�˳��ģ��ǳ�ʱ����Ҫ����Ϊ�û�ϣ�����õȴ�
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

	while (isEmpty())
	{
		if (0 == timeout)
		{
			result = -1;
			break;
		}else if (timeout < 0)
		{
			m_emptyCond->wait();
		}
		else
		{
			//��ʱ�ȴ�����Ҫ�˳��ģ��ǳ�ʱ����Ҫ����Ϊ�û�ϣ�����õȴ�
			result = m_emptyCond->timedwait(timeout);
			if (result)
			{
				result = -1;
				break;
			}
		}

	}

	return result;
}



#endif


