
#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_

#include <string>
#include <vector>
#include "threadmgr/bgd_scheduler.h"

struct STAudioInfo
{
	std::string name;
	std::string word;
};

class CContrl:public BaseScheduler
{
public:
	CContrl(uint32_t n_thread = 10, uint32_t n_queue = 100)
		:BaseScheduler(n_thread, n_queue) {}
public:
	int exec();

};

#endif





