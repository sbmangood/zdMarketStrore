#pragma once

#include "globalInclude.h"
#include "market/marketManager/marketManagerBase.h"
#include "market/ZdFuture/zdFuture.h"
#include "market/ZdMarket/ZdMarket.h"
#include "communicate/protocal/marketStroreProtocal/marketStroreProtocal.h"
#include "communicate/mysqlChannel/mysqlChanel.h"
#include "market/CIT_Market/CIT_Market.h"

class ZD_MarketManager:public boost::serialization::singleton<ZD_MarketManager>
{
public:
	virtual void start();
	virtual void stop();

	

private:
	std::shared_ptr<Endpoint>	endPointFuture;
	std::shared_ptr<Endpoint>	endPointMarket;
	std::shared_ptr<Endpoint>	endPointCII;

	GlobalInclude				glInclude;
	
	std::shared_ptr<ZD_Future>	zdFuture;
	std::shared_ptr<ZdMarket>	zdMarket;

	std::shared_ptr<CIT_Market>	citMarket;
	CThostFtdcMdApi				*pUserApi;

	char						gUpperURL[128];
	CIT_MarketConfig			cITMarketConfig;
private:
	void createNewTables();
	bool createEndPoint();
	bool createCIT_Market();
	bool createZD_Market();

	void noUseMessageRecive(unsigned int, const boost::any& msg)
	{}
};