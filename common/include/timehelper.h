
#ifndef __TIMEHELPER_H_
#define __TIMEHELPER_H_

#include <time.h>

/* ʱ����غ��� */

#ifdef __cplusplus
extern "C"
{
#endif

	//ʱ���ַ���תʱ���
	time_t timestrToNumber(const char *pcTimestr);

	//��ȡ��ǰʱ���
	time_t getCurrentTime();

	//��ȡ�����
	int getRandomNumber(int iRange);

	//��ȡUUID
	char * getUUID();

	//��ȡ����ʱ��
	time_t getMonotonicTime();

	//�붨ʱ��
	void secondTimer(long lSec);

	//΢�붨ʱ��
	void microsecondTimer(long lusec);

#ifdef __cplusplus
}
#endif

#endif

