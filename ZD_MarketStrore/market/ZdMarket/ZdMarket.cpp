#include "ZdMarket.h"
static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("ZdMarket");

bool ZdMarket::setEndPoint(std::shared_ptr<Endpoint> ep)
{
	endPoint = ep;
	if (ep != nullptr)
		hasEndPoint = true;
	else
		hasEndPoint = false;

	return hasEndPoint;
}


bool ZdMarket::Create()
{

#define tempTest
#ifdef tempTest
/******** can delete later *********/
m_appConfig.heartBeat = "500";
m_appConfig.softwareName = "1";
m_appConfig.userId = "wkx01";
m_appConfig.password = "888888";
m_appConfig.softwareVersion = "2";
m_appConfig.macAddress = "song_123";
m_appConfig.computerName = "WJQ-PC";
m_appConfig.serverAddress = "192.168.1.215:9017";//"222.73.119.230:9010";222.73.119.230:9016 192.168.1.79:9988
m_appConfig.authorCode = "j2qUK+rcZC24+glu8C+XRubRYF1w4k0f";
/***********************************/
#endif


	if (!hasEndPoint)
	{
		logger->error("must set endpoint first! in ZdMarket::Create");
		return false;
	}

	
	if (m_pMarketApi) {
		Destroy();
	}
	m_pMarketApi = CMarketApi::CreateMarketApi(true, "TestMarket.log");
	if (m_pMarketApi) {
		logger->info("---------------- API ��Ϣ ----------------");
		logger->info("API Version: {}", m_pMarketApi->GetVersion());
		logger->info("Server Addr: {}", m_appConfig.serverAddress.c_str());
		logger->info("User ID: {}", m_appConfig.userId.c_str());
		

		char addr[MAX_PATH];
		snprintf(addr, MAX_PATH, "tcp://%s", m_appConfig.serverAddress.c_str());

		LONG beat = atol(m_appConfig.heartBeat.c_str());

		m_pMarketApi->RegisterSpi(this);
		m_pMarketApi->RegisterNameServer(addr);
		m_pMarketApi->SetHeartBeatTimeout(beat);
		m_pMarketApi->Init();

		logger->info("��ʼ�������������....");
	
		return true;
	}
	return false;
}

void ZdMarket::Destroy()
{
	if (m_pMarketApi) {
		if (m_bIsLogin) {
			m_bIsLogin = false;
			std::this_thread::sleep_for(chrono::milliseconds(100));
		}

		m_pMarketApi->Release();
		m_pMarketApi = NULL;
		logger->info("���� CMyMarket");

	}
}

string ZdMarket::GetDateTime()
{
#ifdef _WIN32
	SYSTEMTIME st = {};
	GetLocalTime(&st);

	char szBuffer[MAX_PATH];
	snprintf(szBuffer, sizeof(szBuffer), "��%04d-%02d-%02d %02d:%02d:%02d.%03d��",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return szBuffer;
#else
	char szBuffer[MAX_PATH];

	time_t now;
	time(&now);
	tm *tm_now = localtime(&now);

	timeval tv;
	gettimeofday(&tv, NULL);

	snprintf(szBuffer, sizeof(szBuffer), "��%04d-%02d-%02d %02d:%02d:%02d.%03d��",
		tm_now->tm_year + 1900, tm_now->tm_mon + 1, tm_now->tm_mday,
		tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, tv.tv_usec / 1000);

	return szBuffer;
#endif
}
//price �ַ�����Ӧ����0.0
bool ZdMarket::juggePrice(const std::string &price)
{
	for (auto it = price.begin(); it != price.end(); it++)
	{
		if ((*it) != '0' && (*it) != '.')
			return true;
	}
	return false;
}
void ZdMarket::SubscStockBroker()
{
	CMarketReqBrokerDataField qry = {};

	const char cont[] = "00700.HK";
	safe_cpy(qry.ContCode, cont);
	if (m_pMarketApi->ReqBrokerData(&qry, 0))
		logger->info ("���Ĺ�Ʊ�����̳ɹ�: ");
	else
		logger->info ("���Ĺ�Ʊ������ʧ��: {}", cont);
}

void ZdMarket::UnsubscAllMarket()
{
	CMarketReqMarketDataField qry = {};

	qry.MarketType = DAF_TYPE_Unknown;
	qry.SubscMode = DAF_SUB_UnsubcribeAll;
	if (m_pMarketApi->ReqMarketData(&qry, 0))
		logger->info ("�˶�ȫ������ɹ�");
	else
		logger->info ("�˶�ȫ������ʧ��");
}

void ZdMarket::SendLoginRequest()
{
	CMarketReqUserLoginField req = {};



	logger->info("�û���:{}", m_appConfig.userId);
	logger->info("����:******");

	safe_cpy(req.UserId, m_appConfig.userId.c_str());
	safe_cpy(req.UserPwd, m_appConfig.password.c_str());
	safe_cpy(req.AuthorCode, m_appConfig.authorCode.c_str());
	safe_cpy(req.MacAddress, m_appConfig.macAddress.c_str());
	safe_cpy(req.ComputerName, m_appConfig.computerName.c_str());
	safe_cpy(req.SoftwareName, m_appConfig.softwareName.c_str());
	safe_cpy(req.SoftwareVersion, m_appConfig.softwareVersion.c_str());

	if (m_pMarketApi->ReqUserLogin(&req, 0)) {
		logger->info("��ʼ��¼....");
	}
	else {
		logger->info("{}", req.ErrorDescription);
	}
}

void ZdMarket::SubscStockMarket()
{
	CMarketReqMarketDataField qry = {};

	qry.MarketType = DAF_TYPE_Stock;
	qry.SubscMode = DAF_SUB_Append;

	static int times = 0;

	if (times > 90)
		times = 0;
	char cont[30] = "HKEX,01000.HK";

	int num = 10;

	const char cont0[] = "HKEX,00001.HK";
	const char cont1[] = "HKEX,00700.HK";
	const char cont2[] = "HKEX,00002.HK";
	const char cont3[] = "HKEX,00003.HK";
	const char cont4[] = "HKEX,00004.HK";
	const char cont5[] = "HKEX,00005.HK";
	const char cont6[] = "HKEX,00006.HK";
	const char cont7[] = "NASD,AABA.US";
	const char cont8[] = "NASD,A.US";
	const char cont9[] = "NASD,AA.US";

	if (times > 0)
		qry.MarketCount = 10;
	else
		qry.MarketCount = 20;

	for (int i = 0; i < 10; i++)
	{
		std::string str = std::to_string((num*times + i + 1000));

		memcpy(cont + 6, str.c_str(), 4);
		safe_cpy(qry.MarketTrcode[i], cont);
	}

	safe_cpy(qry.MarketTrcode[10], cont0);
	safe_cpy(qry.MarketTrcode[11], cont1);
	safe_cpy(qry.MarketTrcode[12], cont2);
	safe_cpy(qry.MarketTrcode[13], cont3);
	safe_cpy(qry.MarketTrcode[14], cont4);
	safe_cpy(qry.MarketTrcode[15], cont5);
	safe_cpy(qry.MarketTrcode[16], cont6);
	safe_cpy(qry.MarketTrcode[17], cont7);
	safe_cpy(qry.MarketTrcode[18], cont8);
	safe_cpy(qry.MarketTrcode[19], cont9);

	times++;

	if (m_pMarketApi->ReqMarketData(&qry, 0)) {
		logger->info ("���Ĺ�Ʊ����ɹ�") ;
	}
	else {
		logger->info ("���Ĺ�Ʊ����ʧ��") ;
		logger->info ("����,{}", qry.ErrorDescription);
	}
}


void ZdMarket::SubscFutureBroker()
{
	CMarketReqBrokerDataField qry = {};

	const char cont[] = "GC1903";
	safe_cpy(qry.ContCode, cont);
	if (m_pMarketApi->ReqBrokerData(&qry, 0))
		logger->info ("�����ڻ������̳ɹ�: {}" ,cont);
	else
		logger->info ("�����ڻ�������ʧ��: {}",cont );
}

void ZdMarket::SubscFutureMarket()
{
	CMarketReqMarketDataField qry = {};

	qry.SubscMode = DAF_SUB_Append;
	qry.MarketType = DAF_TYPE_Future;

	qry.MarketCount = 2;
	safe_cpy(qry.MarketTrcode[0], "XEurex,FDAX2003");
	safe_cpy(qry.MarketTrcode[1], "HKEX,HSI2003");


	if (m_pMarketApi->ReqMarketData(&qry, 0))
		logger->info ("�����ڻ�����ɹ�");
	else
		logger->info ("�����ڻ�����ʧ��");
}


void ZdMarket::UnsubscFutureMarket()
{
	CMarketReqMarketDataField qry = {};

	qry.MarketType = DAF_TYPE_Future;
	qry.SubscMode = DAF_SUB_Unsubcribe;


	qry.MarketCount = 2;

	safe_cpy(qry.MarketTrcode[0], "XEurex,FDAX2003");
	safe_cpy(qry.MarketTrcode[1], "HKEX,HSI2003");

	if (m_pMarketApi->ReqMarketData(&qry, 0))
		logger->info("�˶��ڻ�����ɹ�") ;
	else
		logger->info("�˶��ڻ�����ʧ��") ;
}

void ZdMarket::SubscFutureMarketByExg()
{
	const std::list<outExProductSubStruct> &subList = StaticDatas::get_mutable_instance().outExProductSubList;
	CMarketReqMarketDataField qry = {};
	qry.SubscMode = DAF_SUB_Append;
	qry.MarketType = DAF_TYPE_Future;
	qry.MarketCount = 1;
	for (auto it = subList.begin(); it != subList.end(); it++)
	{
		std::string tem = (*it).exchange + "," + (*it).product + "*";
		safe_cpy(qry.MarketTrcode[0], tem.c_str());
		if (!m_pMarketApi->ReqMarketData(&qry, 0))		
			logger->error("�������������ڻ����鷢��ʧ��:{}", tem);
	}	

}

void ZdMarket::OnFrontConnected()
{
	logger->info ("���ӷ������ɹ�") ;
	m_bIsLogin = false;
	SendLoginRequest();
}
void ZdMarket::OnFrontDisconnected(int iReason)
{
	m_bIsLogin = false;
	logger->info ("���ӶϿ�, ԭ�� = {}.", iReason);
}
void ZdMarket::OnHeartBeatWarning(int iTimeLapse)
{
	logger->info ("����:{}", iTimeLapse);
}
void ZdMarket::OnRspUserLogin(CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast)
{


	DB_LoginData ld;
	if (pRspInfo->ErrorID) {
		logger->info ("��¼����ӿ�ʧ�ܣ�") ;
		logger->info("ErrorCode = {}", pRspInfo->ErrorID);
		logger->info ("ErrorMsg = {}", pRspInfo->ErrorMsg);
		m_bIsLogin = false;
		ld .msg = "Login err��";
	}
	else {
		logger->info("��¼����ӿڳɹ������Զ���������....");
		ld.msg = "Market Login ok!....";
		m_bIsLogin = true;
	}
	endPoint->send_msg(boost::any(ld));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SubscFutureMarketByExg();
}
void ZdMarket::OnRspUserLogout(CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast)
{
	if (pRspInfo->ErrorID) {
		logger->info ("�ǳ�����ӿ�ʧ��.");
		logger->info ("ErrorID = {}", pRspInfo->ErrorID);
		logger->info ("ErrorMsg = {}", pRspInfo->ErrorMsg);
		m_bIsLogin = false;
	}
	else {
		logger->info("�ǳ�����ӿڳɹ�.");
		m_bIsLogin = true;
	}

}
void ZdMarket::OnRspTradeDate(CMarketRspTradeDateField *pRspTradeDate, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast)
{
	if (pRspInfo->ErrorID) {
		logger->info("ErrorID = {}", pRspInfo->ErrorID);
		logger->info("ErrorMsg = {}", pRspInfo->ErrorMsg);
	}
	else {
		printf("%s TradeDate=%-10s TradeProduct=%-10s\r\n",
			GetDateTime().c_str(),
			pRspTradeDate->TradeDate,
			pRspTradeDate->TradeProduct);
	}
}
void ZdMarket::OnRspBrokerData(CMarketRspBrokerDataField *pRspBrokerData, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast)
{
	if (pRspInfo->ErrorID) {
		logger->info ("ErrorID = {}", pRspInfo->ErrorID);
		logger->info("ErrorMsg = {}", pRspInfo->ErrorMsg);
	}
	else {
		logger->info ("BrokerData= {}", pRspBrokerData->BrokerData) ;
	}
}
void ZdMarket::OnRspMarketData(CMarketRspMarketDataField *pRspMarketData, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast)
{


	if (pRspInfo->ErrorID) {
		logger->info ("ErrorID = {}" , pRspInfo->ErrorID);
		logger->info ( "ErrorMsg = {}",  pRspInfo->ErrorMsg);
	}
	else {
		ZD_DBMarketData md;
		
		//printf("%s TrCode=%-10s CurrPrice=%-10s CurrNumber=%-7s FilledNum=%-10s QuoteType=%s\r\n",
		//	GetDateTime().c_str(), pRspMarketData->TreatyCode, pRspMarketData->CurrPrice,
		//	pRspMarketData->CurrNumber, pRspMarketData->FilledNum, pRspMarketData->QuoteType);

		md.contract = pRspMarketData->TreatyCode;
		md.exchange = pRspMarketData->ExchangeCode;
		md.dueDate = StaticDatas::get_mutable_instance().getDueDayFromContractMap(md.contract);
		md.time = pRspMarketData->Time;
		md.price = pRspMarketData->CurrPrice;
		if (juggePrice(md.price))
		{
			endPoint->send_msg(boost::any(md));
		}		
		
	}
	
}


