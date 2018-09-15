
#include "bgd_queue.h"

/********************************************************
TemplateTaskQueue
*********************************************************/

/********************************************************
   Func Name: TemplateTaskQueue
Date Created: 2018-9-15
 Description: 构造函数
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
TemplateTaskQueue<T>::TemplateTaskQueue()
{
	m_mutex = new CMutexLock;
	m_cond = new CCondLock(m_mutex);
}

/********************************************************
   Func Name: set
Date Created: 2018-9-15
 Description: 添加元素
       Input: 
      Output: 
      Return: 
     Caution: 
*********************************************************/
template<typename T>
void TemplateTaskQueue<T>::set(T obj)
{
}







