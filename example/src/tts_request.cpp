
#include "tts_request.h"

namespace servant
{
	/************************************************************************
	CServantRequest                                                          
	************************************************************************/

	/********************************************************
	   Func Name: process
	Date Created: 2018-10-13
	 Description: ������
		   Input: 
		  Output: 
		  Return: 
		 Caution: 
	*********************************************************/
	int CServantRequest::call()
	{
		_worker->process();
		//ע��workerɾ��λ��
		delete _worker;
		_worker = NULL;
		return 0;
	}


}






