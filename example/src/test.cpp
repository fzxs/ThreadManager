
#ifdef TEST

#include <iostream>
#include <unistd.h>

#include "controller.h"

void test()
{
	CContrl contro;
	contro.exec();
	printf("please wait ...\n");
	while (1)
	{
		sleep(3);
	}
}


int main(int argc, char *argv[])
{
	test();
	getchar();
	return 0;
}


#endif








