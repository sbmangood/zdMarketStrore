#include "CIT_Market.h"
static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("CIT_Market");
CIT_Market::CIT_Market(CThostFtdcMdApi *pUserApi,const CIT_MarketConfig &cf)
{
	m_pUserApi = pUserApi;
	cITMarketConfig = cf;

}

bool CIT_Market::setEndPoint(std::shared_ptr<Endpoint> ep)
{
	endPoint = ep;
	if (ep != nullptr)
		hasEndPoint = true;
	else
		hasEndPoint = false;

	return hasEndPoint;
}

void CIT_Market::OnFrontConnected()
{
	logger->info("CII 连接 成功!");
	if (cITMarketConfig.brokerID.size() <=sizeof(gReqUserLogin.BrokerID))//mf
	{
		memcpy(gReqUserLogin.BrokerID, cITMarketConfig.brokerID.c_str(), cITMarketConfig.brokerID.size());
	}		
	else
	{
		logger->error("brokerID 太长了");
	}
	if (cITMarketConfig.userID.size() <= sizeof(gReqUserLogin.UserID))
	{
		memcpy(gReqUserLogin.UserID, cITMarketConfig.userID.c_str(), cITMarketConfig.userID.size());
	}	
	else
	{
		logger->error("UserID 太长了");
	}
	if (cITMarketConfig.passwd.size() <= sizeof(gReqUserLogin.Password))
	{
		memcpy(gReqUserLogin.Password, cITMarketConfig.passwd.c_str(), cITMarketConfig.passwd.size());
	}
	else
	{
		logger->error("passwd 太长了");
	}
	// 发出登陆请求
	m_pUserApi->ReqUserLogin(&gReqUserLogin, 0);
}

void CIT_Market::OnFrontDisconnected(int nReason)
{
	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	logger->error("Connection broken, is reconnecting ... reason {}", nReason);
}

void CIT_Market::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	if (pRspInfo->ErrorID != 0) {
		// 端登失败，客户端需进行错误处理
		logger->error("Failed to login, errorcode={0} errormsg={1} requestid={2}chain={3}", 
			pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);

	}
	else
	{
		logger->info("CII 登录 成功!");
		const std::list<inExProductSubStruct> & subList = StaticDatas::get_mutable_instance().inExProductSubList;
		char** Instrumnet = new char*[subList.size()];
		char** pTtem = Instrumnet;
		for (auto it = subList.begin(); it != subList.end(); it++)
		{

			*pTtem = (char *)(it->product.c_str());
			pTtem++;
		}
		int ret = m_pUserApi->SubscribeMarketData(Instrumnet, subList.size());
		delete[]Instrumnet;

		logger->info("Subscribe {} contracts for market data", subList.size());

	}

}

void CIT_Market::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	logger->info("CIT_Market::OnRspUserLogout!");
}
void CIT_Market::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (pRspInfo->ErrorID == 0)
	{
		logger->info("Sub {} maket data ok!", std::string(pSpecificInstrument->InstrumentID));
	}
	else
	{
		logger->error("OnRspSubMarketData error");
	}
}

void CIT_Market::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	logger->info("OnRspUnSubMarketData:\n");
}

void CIT_Market::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	logger->error("OnRspError:{}", pRspInfo->ErrorMsg);
}

void CIT_Market::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	

	CIT_DBMarketData zdd;
	zdd.contract = pDepthMarketData->InstrumentID;
	zdd.price = std::to_string(pDepthMarketData->LastPrice);
	std::string time = std::string(pDepthMarketData->UpdateTime);
	dealTime(time);

	if (cITMarketConfig.uniqueMarket== "true"
		&&time<cITMarketConfig.endTime && time>cITMarketConfig.startTime)
	{
		return;
	}
	zdd.time = std::string(pDepthMarketData->TradingDay)+time ;
	
	endPoint->send_msg(boost::any(zdd));

}

void CIT_Market::dealTime(std::string &time)
{
	time.erase(std::remove(time.begin(), time.end(), ':'), time.end());
}