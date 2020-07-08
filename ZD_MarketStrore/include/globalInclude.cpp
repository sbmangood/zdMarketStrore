#include "globalInclude.h"

GlobalInclude::GlobalInclude()
{
	lastDay = getTodayStr();
}

std::string GlobalInclude::getTodayStr()
{
	time_t t;
	time(&t);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y_%m_%d", localtime(&t));
	return tmp;
}

std::string GlobalInclude::getTimeStr()
{
	time_t t;
	time(&t);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H-%M-%S", localtime(&t));
	return tmp;
}
