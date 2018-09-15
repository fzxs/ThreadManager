

#ifndef __BGD_THREADMGR_H_
#define __BGD_THREADMGR_H_

#include "bgd_common.h"

/* 线程管理器类 */

class CThreadManager
{
public:

	//子线程退出函数
	int atExit(void *object, FUNC_THR_CLEANUP cleanup_hook, void *param) { return 0; }
};

#endif



