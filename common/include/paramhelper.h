
#ifndef __PARAMHELPER_H_
#define __PARAMHELPER_H_

/* 命令行参数处理 */

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

	//命令行参数解析
	StListHead *parseParam(int argc, char ** argv, const char *pcOptString, const stLongOption *pstLongOption);

	//释放资源
	void releaseSource(StListHead *pstHead);

#ifdef __cplusplus
}
#endif

#endif

