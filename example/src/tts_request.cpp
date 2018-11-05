
#include "tts_request.h"

namespace servant
{
	/************************************************************************
	CServantRequest                                                          
	************************************************************************/

	/********************************************************
	   Func Name: process
	Date Created: 2018-10-13
	 Description: 处理函数
		   Input: 
		  Output: 
		  Return: 
		 Caution: 
	*********************************************************/
	int CServantRequest::call()
	{
		_worker->process();
		//注意worker删除位置
		delete _worker;
		_worker = NULL;
		return 0;
	}


}






