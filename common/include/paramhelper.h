
#ifndef __PARAMHELPER_H_
#define __PARAMHELPER_H_

/* �����в������� */

#include "linkedlist.h"

typedef struct tagSTOptionNode
{
	StListNode stNode;
	char gcOption[64];
	char gcValue[256];
}STOptionNode;

typedef struct option stLongOption;


#ifdef __cplusplus
extern "C"
{
#endif

	//�����в�������
	StListHead *parseParam(int argc, char ** argv, const char *pcOptString, const stLongOption *pstLongOption);

	//�ͷ���Դ
	void releaseSource(StListHead *pstHead);

#ifdef __cplusplus
}
#endif

#endif

