#pragma once

#include "globalInclude.h"
#include "market/marketManager/marketManagerBase.h"
#include "market/ZdFuture/zdFuture.h"
#include "market/ZdMarket/ZdMarket.h"
#include "communicate/protocal/marketStroreProtocal/marketStroreProtocal.h"
#include "communicate/mysqlChannel/mysqlChanel.h"

class ZD_MarketManager:public boost::serialization::singleton<ZD_MarketManager>
{
public:
	virtual void start();
	virtual void stop();

	

private:
	std::shared_ptr<Endpoint>	endPointFuture;
	std::shared_ptr<Endpoint>	endPointMarket;
	GlobalInclude				glInclude;
	std::shared_ptr<ZD_Future>	zdFuture;
	std::shared_ptr< ZdMarket>	zdMarket;


private:
	void createNewTables();
	bool createEndPoint();

	void noUseMessageRecive(unsigned int, const boost::any& msg)
	{}
};