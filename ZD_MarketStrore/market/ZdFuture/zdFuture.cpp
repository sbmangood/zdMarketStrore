#include "zdFuture.h"
static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("zdFuture");

bool ZD_Future::Create()
{

	if (!hasEndPoint)
	{
		logger->error("must set endpoint first! in ZdMarket::Create");
		return false;
	}


	if (m_pFutureApi) {
		Destroy();
	}


	m_pFutureApi = CFutureApi::CreateFutureApi(true, "TestFuture.log", "");
	if (m_pFutureApi) {


		char addr[MAX_PATH];
		snprintf(addr, MAX_PATH, "tcp://%s", m_appConfig.serverAddress.c_str());

		LONG beat = atol(m_appConfig.heartBeat.c_str());

		m_pFutureApi->RegisterSpi(this);
		m_pFutureApi->RegisterNameServer(addr);
		m_pFutureApi->SetHeartBeatTimeout(beat);
		m_pFutureApi->Init();
		logger->info("��ʼ�����ڻ����׷�����....");
		
		return true;
	}
	else {
		logger->info("�ڻ��ӿڳ�ʼ��ʧ��.");		
	}
	return false;
}
void ZD_Future::Destroy()
{
	if (m_pFutureApi) {
		if (m_bIsLogin) {
			m_bIsLogin = false;
			CFutureReqUserLogoutField req = {};
			safe_cpy(req.UserId, m_appConfig.userId.c_str());
			m_pFutureApi->ReqUserLogout(&req, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		m_pFutureApi->Release();
		m_pFutureApi = NULL;
		logger->info("���� CMyFuture");
		
	}
}
bool ZD_Future::IsLogin()
{
	return m_bIsLogin;
}
bool ZD_Future::IsCreate()
{
	return m_pFutureApi != NULL;
}
void ZD_Future::ReqUserLogin()
{
	CFutureReqUserLoginField req = {};

	safe_cpy(req.UserId, m_appConfig.userId.c_str());
	safe_cpy(req.UserPwd, m_appConfig.password.c_str());
	safe_cpy(req.AuthorCode, m_appConfig.authorCode.c_str());
	safe_cpy(req.MacAddress, m_appConfig.macAddress.c_str());
	safe_cpy(req.ComputerName, m_appConfig.computerName.c_str());
	safe_cpy(req.SoftwareName, m_appConfig.softwareName.c_str());
	safe_cpy(req.SoftwareVersion, m_appConfig.softwareVersion.c_str());

	if (m_pFutureApi->ReqUserLogin(&req, 0)) {
		logger->info("��ʼ��½�ڻ�����...");
	}
	else {
		logger->error("�ڻ����׵�½ʧ��:{}", req.ErrorDescription);
	}
}

void ZD_Future::OnFrontConnected()
{
	logger->info("�����ڻ����׷������ɹ�!");
	m_bIsLogin = false;
	ReqUserLogin();
}
void ZD_Future::OnFrontDisconnected(int iReason)
{
	m_bIsLogin = false;
	logger->info("�ڻ����׷��������ӶϿ�, ԭ��= {}", iReason);	
}
void ZD_Future::OnHeartBeatWarning(int iTimeLapse)
{
	logger->info("����:{}", iTimeLapse);
}

void ZD_Future::OnRtnTrade(CFutureRtnTradeField *pRtnTrade, CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)
{

	if (pRspInfo->ErrorID == 0)
	{
		T_TradeDetailsData ttdd;
		logger->info("�¶����仯��Լ��:{}", pRtnTrade->TreatyCode);
		ttdd.reportNo = pRtnTrade->LocalNo;
		ttdd.tradeNo = pRtnTrade->OrderNo;
		ttdd.exchange = pRtnTrade->ExchangeCode;
		ttdd.brokeCompany = "ZD_Company";
		ttdd.contract = pRtnTrade->TreatyCode;
		ttdd.dueDate=StaticDatas::get_mutable_instance().getDueDayFromContractMap(ttdd.contract);
		ttdd.bidAsk = pRtnTrade->BuySale;
		ttdd.lotQty = pRtnTrade->FilledNumber;
		ttdd.price = pRtnTrade->FilledPrice;
		ttdd.charge = pRtnTrade->Commsion;
		ttdd.tradeDate = pRtnTrade->FilledDate;
		ttdd.tradeTime = pRtnTrade->FilledDate + std::string(" ") + pRtnTrade->FilledTime;
		ttdd.openClose = pRtnTrade->AddReduce;

		endPoint->send_msg(boost::any(ttdd));
	}
	else
	{
		logger->info("�¶����仯����:{}", pRspInfo->ErrorMsg);
	}
}
//�����仯
void ZD_Future::OnRtnOrder(CFutureRtnOrderField *pRtnOrder, CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)
{



}
void ZD_Future::OnRspUserLogin(CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)
{
	DB_LoginData dld;
	if (pRspInfo->ErrorID) {
		logger->error("��¼�ڻ����׷�����ʧ��.");
		m_bIsLogin = false;
		dld.msg = "API future login err.";
	}
	else {
		logger->info("��¼�ڻ����׷������ɹ�.");
		m_bIsLogin = true;
		dld.msg = "API future login ok";
		QryInstrument();
	}
	endPoint->send_msg(boost::any(dld));
}

bool ZD_Future::setEndPoint(std::shared_ptr<Endpoint> ep)
{
	endPoint = ep;
	if (ep != nullptr)
		hasEndPoint = true;
	else
		hasEndPoint = false;

	return hasEndPoint;
}


void ZD_Future::QryInstrument()
{
	
	CFutureQryInstrumentField qry = {};
	const std::list<outExProductSubStruct> &subList = StaticDatas::get_mutable_instance().outExProductSubList;
	for (auto it = subList.begin(); it != subList.end(); it++)
	{
		safe_cpy(qry.ExchangeNo, (*it).exchange.c_str());
		safe_cpy(qry.CommodityNo, (*it).product.c_str());
		if (!m_pFutureApi->ReqQryInstrument(&qry, 0)) {
			logger->error("��ѯ��Լ: {}", qry.ErrorDescription);
		}

	}
	

}

void ZD_Future::OnRspQryInstrument(CFutureRspInstrumentField *pRspInstrument, CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)
{
	if (pRspInfo->ErrorID) {
		logger->error("ErrorID={}", pRspInfo->ErrorID);
	}
	else {
		m_ContractInfor cIfor;
		cIfor.getData(pRspInstrument);
		std::string contract = pRspInstrument->CommodityCode;
		StaticDatas::get_mutable_instance().insertUpdateContractMap(contract, cIfor);
		logger->info("��ѯ�ĺ�Լ:{}", contract);

	}
}