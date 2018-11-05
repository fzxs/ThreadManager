
#ifndef __TTS_REQUEST_H_
#define __TTS_REQUEST_H_

#include "threadmgr/bgd_methodrequest.h"
#include "threadmgr/bgd_future.h"
#include "worker.h"

namespace servant
{
	class CServantRequest :public AbsMethodRequest
	{
	public:
		CServantRequest(CEngineer *worker) :_worker(worker)
		{
		}
	public:
		//回调方法
		virtual int call(void);

	private:
		CEngineer *_worker;
		//CFuture<int> _result;

	};
}


#endif









