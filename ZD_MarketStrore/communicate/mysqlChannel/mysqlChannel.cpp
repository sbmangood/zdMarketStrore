#include "mysqlChanel.h"

static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("mysqlChanel");


MysqlChannel::MysqlChannel(boost::shared_ptr<IProtocol> protocol, MysqlConConfig &dbC) :
	dbConfig(dbC),IChannel(protocol)
{
	dbPool = std::make_shared<DBPool>(DBPool::GetInstance());
}



MysqlChannel::~MysqlChannel()
{

}

bool MysqlChannel::start()
{
	dbConfig = ConfigManager::get_const_instance().mySqlConfig;
	dbPool->initPool(dbConfig.url, dbConfig.user, dbConfig.passwd,20);
	con = dbPool->GetConnection();
	int times = 10;
	while (times--)
	{
		if (con == nullptr)
			con = dbPool->GetConnection();
		else
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}
	if (times <= 0)
		return false;
	return true;
}

bool MysqlChannel::send_data(const unsigned char* buffer, unsigned int size)
{
	std::string cmd = std::string(buffer, buffer+size);
	if (cmd.size() < 2)
		return true;
	Statement *state=nullptr;
	logger->error("Time start ");
	try {
		//if (con->isClosed)
		//{
		//	dbPool->ReleaseConnection(con);
		//	con = dbPool->GetConnection();
		//	Statement *st= con->createStatement();
		//	std::string cmd = "use " + globalInclude.dbName + ";";

		//}
		if (con == nullptr)
		{
			logger->error("con == nullptr in MysqlChannel::send_data");
			return false;
		}
		
		state = con->createStatement();
		state->execute(cmd);
		delete state;
	
	}
	catch (sql::SQLException &e) {
		logger->error("error:{} in MysqlChannel::send_data", e.what());
		logger->error("error sql: {}", cmd);
		if(state!=nullptr)
			delete state;
		return false;

	}
	logger->error("Time end ");
	return true;
}