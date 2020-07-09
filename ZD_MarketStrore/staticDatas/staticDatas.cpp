#include "staticDatas.h"

static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("staticDatas");

void StaticDatas::init()
{
	dbConfig = ConfigManager::get_mutable_instance().mySqlConfig;
	if (!connectDb())
	{
		logger->error("!connectDb in StaticDatas::readDbBaseData()");
		return;
	}
	readDbBaseData();
}

bool StaticDatas::readDbBaseData()
{
	if (readOutExProductSubList() && readInExProductSubList())
	{
		return true;
	}
	else
	{
		logger->error("readOutExProductSubList() && readInExProductSubList() err");
		return false;
	}

}

bool StaticDatas::readOutExProductSubList()
{
	Statement *state;
	ResultSet *result;
	//查询
	state = con->createStatement();

	try
	{
		std::string cmd = "use " + dbConfig.dbName;
		state->execute(cmd);
	}
	catch (sql::SQLException &e)
	{
		logger->error("{}", e.what());
		return false;

	}
	std::string query = "select " + oSubTable.subProduct + " from " + oSubTable.tableName;
	try
	{
		result = state->executeQuery(query);
	}
	catch (sql::SQLException &e)
	{
		logger->error("{}", e.what());
		return false;
	}

	logger->info("外盘订阅品种列表: ");
	int cnt = 1;
	//输出查询
	while (result->next())
	{
		try
		{
			std::string subProduct = result->getString(oSubTable.subProduct);
			size_t find = subProduct.find(":");
			if (find == std::string::npos)
				break;
			outExProductSubStruct subS;
			subS.exchange = subProduct.substr(0, find);
			subS.product = subProduct.substr(find + 1);
			outExProductSubList.push_back(std::move(subS));
			logger->info("{0}->{1}", cnt++, subProduct);

		}
		catch (sql::SQLException &e) {
			logger->error("{}", e.what());

		}
	}


	delete result;
	delete state;

	return true;
}
bool StaticDatas::readInExProductSubList()
{
	Statement *state;
	ResultSet *result;
	//查询
	state = con->createStatement();

	try
	{
		std::string cmd = "use " + dbConfig.dbName;
		state->execute(cmd);
	}
	catch (sql::SQLException &e)
	{
		logger->error("{}", e.what());
		return false;

	}
	std::string query = "select " + iSubTable.subProduct + " from " + iSubTable.tableName;
	try
	{
		result = state->executeQuery(query);
	}
	catch (sql::SQLException &e)
	{
		logger->error("{}", e.what());
		return false;
	}

	logger->info("内盘订阅合约列表: ");
	int cnt = 1;
	//输出查询
	while (result->next())
	{
		try
		{
			std::string subProduct = result->getString(iSubTable.subProduct);

			inExProductSubStruct subS;
	
			subS.product = subProduct;
			inExProductSubList.push_back(std::move(subS));
			logger->info("{0}->{1}", cnt++, subProduct);

		}
		catch (sql::SQLException &e) {
			logger->error("{}", e.what());

		}
	}


	delete result;
	delete state;
	return true;
}

bool StaticDatas::connectDb()
{
	dbPool = std::make_shared<DBPool>(DBPool::GetInstance());
	dbPool->initPool(dbConfig.url, dbConfig.user, dbConfig.passwd, 2);
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

void StaticDatas::insertUpdateContractMap(const std::string &contract, const m_ContractInfor &cIfor)
{
	std::lock_guard<std::mutex>		lock(contractMapLock);
	if (contractMap.find(contract) == contractMap.end())
	{
		contractMap.insert(std::pair<string,m_ContractInfor>(contract, cIfor));
	}
	else
	{
		contractMap.find(contract)->second = cIfor;
	}
}

std::string StaticDatas::getDueDayFromContractMap(const std::string &contract)
{
	std::lock_guard<std::mutex>		lock(contractMapLock);
	if (contractMap.find(contract) == contractMap.end())
	{
		return "19700101";
	}
	else
	{
		return contractMap[contract].dueDate;
	}
}