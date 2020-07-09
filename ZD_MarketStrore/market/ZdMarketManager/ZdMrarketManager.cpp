#include "ZdMarketManager.h"

static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("ZdMarketManager");

void ZD_MarketManager::start()
{
	std::string tm= "tcp://116.236.253.145:42213";

	memcpy(gUpperURL, tm.c_str(), tm.size());

	if (!createEndPoint())
		return;
	createNewTables();

	createZD_Market();
	createCIT_Market();
	
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
	endPointFuture-> bind(boost::shared_ptr<MysqlChannel>(new MysqlChannel(pt1, mcc)), boost::bind(&ZD_MarketManager::noUseMessageRecive, this, _1, _2));
	endPointMarket->bind(boost::shared_ptr<MysqlChannel>(new MysqlChannel(pt2, mcc)), boost::bind(&ZD_MarketManager::noUseMessageRecive, this, _1, _2));
	endPointCII->bind(boost::shared_ptr<MysqlChannel>(new MysqlChannel(pt3, mcc)), boost::bind(&ZD_MarketManager::noUseMessageRecive, this, _1, _2));



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

	if (zdFuture->IsCreate()) {
		logger->info("{}", (zdFuture->IsLogin() ? "api 交易连接成功" : "api 交易连接失败"));
		return false;
	}
	else {
		zdFuture->Create();
	}

	if (zdMarket->IsCreate()) {
		logger->info("{}", (zdMarket->IsLogin() ? "api 行情连接成功" : "api 行情连接失败"));
		return false;
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
		citMarket = std::shared_ptr<CIT_Market>(new CIT_Market(pUserApi));
		citMarket->setEndPoint(endPointCII);
		// 注册一事件处理的实例
		pUserApi->RegisterSpi(citMarket.get());
		// 设置交易托管系统服务的地址，可以注册多个地址备用
		pUserApi->RegisterFront(gUpperURL);
		// 使客户端开始与后台服务建立连接
		pUserApi->Init();

	}
	catch (...)
	{
		logger->error("catch unkown err in createCIT_Market()");
	}

	return true;
}
void ZD_MarketManager::createNewTables()
{
	//切入指定数据库
	std::string cmd = "use " + ConfigManager::get_mutable_instance().mySqlConfig.dbName + ";";
	endPointFuture->send_msg(boost::any(cmd));
	endPointMarket->send_msg(boost::any(cmd));
	//表一
	cmd = glInclude.ofLogin.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//表二
	cmd = glInclude.topts.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//表三
	cmd = glInclude.ttdts.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//表四
	cmd = glInclude.zoest.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//表五
	cmd = glInclude.tipts.createTable();
	endPointFuture->send_msg(boost::any(cmd));
	//表六
	cmd = glInclude.ziest.createTable();
	endPointFuture->send_msg(boost::any(cmd));
}

