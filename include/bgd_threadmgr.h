

#ifndef __BGD_THREADMGR_H_
#define __BGD_THREADMGR_H_

#include "bgd_common.h"

/* �̹߳������� */

class CThreadManager
{
public:

	//���߳��˳�����
	int atExit(void *object, FUNC_THR_CLEANUP cleanup_hook, void *param) { return 0; }
};

#endif



