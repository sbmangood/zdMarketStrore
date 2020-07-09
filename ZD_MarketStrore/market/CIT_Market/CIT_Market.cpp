#include "CIT_Market.h"
static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("CIT_Market");
CIT_Market::CIT_Market(CThostFtdcMdApi *pUserApi)
{
	m_pUserApi = pUserApi;

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
	std::string bd = "95533";
	std::string id = "30020196";
	memcpy(gReqUserLogin.BrokerID, bd.c_str(), bd.size());
	memcpy(gReqUserLogin.UserID, id.c_str(), id.size());
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
			memcpy((*pTtem), it->product.c_str(), it->product.size());
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
	logger->info("Sub {} maket data ok!", std::string(pSpecificInstrument->InstrumentID));
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
	zdd.price = pDepthMarketData->LastPrice;
	zdd.time = pDepthMarketData->UpdateTime;
	logger->info("测试用的新的行情数据:{}", zdd.contract);

	endPoint->send_msg(boost::any(zdd));

}