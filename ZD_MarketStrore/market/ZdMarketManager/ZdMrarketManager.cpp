#include "ZdMarketManager.h"

static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("ZdMarketManager");

void ZD_MarketManager::start()
{
	EndPointMarketLock = new std::mutex();
	EndPointCIILock = new std::mutex();
	cITMarketConfig = ConfigManager::get_mutable_instance().cITMarketConfig;

	if (!createEndPoint())
		return;

	createNewTables();
	createZD_Market();
	createCIT_Market();

	std::this_thread::sleep_for(chrono::milliseconds(100));
	std::vector<std::shared_ptr<Endpoint>> vc;
	vc.push_back(endPointMarket);
	vc.push_back(endPointCII);

	std::thread thr(batchLoopThread, vc);
	thr.detach();

}

void ZD_MarketManager::stop()
{

}


bool ZD_MarketManager::createEndPoint()
{
	boost::shared_ptr<MarketStoreProtocal> pt1 = boost::make_shared<MarketStoreProtocal>();
	boost::shared_ptr<MarketStoreProtocal> pt2 = boost::make_shared<MarketStoreProtocal>();
	boost::shared_ptr<MarketStoreProtocal> pt3 = boost::make_shared<MarketStoreProtocal>();
	endPointFuture = std::shared_ptr<Endpoint>(new Endpoint());
	endPointMarket = std::shared_ptr<Endpoint>(new Endpoint());
	endPointCII = std::shared_ptr<Endpoint>(new Endpoint());
	MysqlConConfig mcc;
	std::string dbName = ConfigManager::get_mutable_instance().mySqlConfig.dbName;
	endPointFuture-> bind(boost::shared_ptr<MysqlChannel>(new MysqlChannel(pt1, mcc, dbName,2)), boost::bind(&ZD_MarketManager::noUseMessageRecive, this, _1, _2));
	endPointMarket->bind(boost::shared_ptr<MysqlChannel>(new MysqlChannel(pt2, mcc, dbName,12)), boost::bind(&ZD_MarketManager::noUseMessageRecive, this, _1, _2));
	endPointCII->bind(boost::shared_ptr<MysqlChannel>(new MysqlChannel(pt3, mcc, dbName,12)), boost::bind(&ZD_MarketManager::noUseMessageRecive, this, _1, _2));



	if ( (!endPointFuture->start()) || (!endPointMarket->start()) || (!endPointCII->start()))
	{
		logger->error("!endPoint->start() in ZD_MarketManager::createEndPoint");
		return false;
	}
	return true;
}
bool ZD_MarketManager::createZD_Market()
{
	zdFuture = std::shared_ptr<ZD_Future>(new ZD_Future());
	zdMarket = std::shared_ptr<ZdMarket>(new ZdMarket());
	zdFuture->setEndPoint(endPointFuture);
	zdMarket->setEndPoint(endPointMarket);
	zdMarket->setMutex(EndPointMarketLock);


	if (zdFuture->IsCreate()) {
		logger->info("{}", (zdFuture->IsLogin() ? "api �������ӳɹ�" : "api ��������ʧ��"));
		
	}
	else {
		zdFuture->Create();
	}

	if (zdMarket->IsCreate()) {
		logger->info("{}", (zdMarket->IsLogin() ? "api �������ӳɹ�" : "api ��������ʧ��"));
		
	}
	else {
		zdMarket->Create();
	}
	return true;
}

bool ZD_MarketManager::createCIT_Market()
{

	try {
		pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
		citMarket = std::shared_ptr<CIT_Market>(new CIT_Market(pUserApi, cITMarketConfig));
		citMarket->setEndPoint(endPointCII);
		citMarket->setMutex(EndPointCIILock);
		// ע��һ�¼������ʵ��
		pUserApi->RegisterSpi(citMarket.get());
		// ���ý����й�ϵͳ����ĵ�ַ������ע������ַ����
		pUserApi->RegisterFront((char*)cITMarketConfig.url.c_str());
		// ʹ�ͻ��˿�ʼ���̨����������
		pUserApi->Init();

	}
	catch (...)
	{
		logger->error("catch unkown err in createCIT_Market()");
		return false;
	}

	return true;
}
void ZD_MarketManager::createNewTables()
{

	std::string cmd;
	std::string dbName = ConfigManager::get_mutable_instance().mySqlConfig.dbName;

	//��һ
	cmd = glInclude.ofLogin.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//���
	cmd = glInclude.topts.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//����
	cmd = glInclude.ttdts.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//����
	cmd = glInclude.zoest.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//����
	cmd = glInclude.tipts.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//����
	cmd = glInclude.ziest.createTable();
	endPointFuture->send_msg(boost::any(cmd));
}

void batchLoopThread(std::vector<std::shared_ptr<Endpoint>> vc)
{
	DB_BatchLoopData bld;

	while (batchWork)
	{
		std::this_thread::sleep_for(chrono::seconds(5));
		for (auto ep : vc)
		{
			ep->send_msg(bld);
		}

		logger->info("��ʱ����!");
	}
}
