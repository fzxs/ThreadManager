
#include "controller.h"
#include <string.h>
#include "worker.h"
#include "tts_request.h"

#include <stdio.h>

/************************************************************************
CContrl                                                          
************************************************************************/


/********************************************************
	Func Name: parse
Date Created: 2018-10-13
	Description: ½âÎöº¯Êý
		Input: 
		Output: 
		Return: 
		Caution: 
*********************************************************/
int CContrl::exec()
{
	int error_code = 0;
	servant::CServantRequest *request = NULL;
	servant::CEngineer *worker = NULL;

	for (int i =0; i < 1000; i++)
	{
		worker = new servant::CEngineer;
		request = new servant::CServantRequest(worker);

		error_code = addRequest(request, 60 * 5);
		if (error_code)
		{
			break;
		}
	}

	return 0;
}




