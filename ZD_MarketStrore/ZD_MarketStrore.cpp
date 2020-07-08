
#define			VERSION				"V1.0.0"

#include <iostream>


#include "market/ZdMarketManager/ZdMarketManager.h"
#include "staticDatas/staticDatas.h"

static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("main");
void tcpMessageRecieve(unsigned int, const boost::any& msg);
int main()
{
	WinDumpTool::setExceptionFilter();
	logger->info("<-------------------------------------------Program start!-------------------------------------------------------->");
	logger->info("                                   This program version is: {0}", VERSION);
	logger->info("<----------------------------------------------------------------------------------------------------------------->");
	
	StaticDatas::get_mutable_instance().init();
	ZD_MarketManager::get_mutable_instance().start();

	while (1);
}

