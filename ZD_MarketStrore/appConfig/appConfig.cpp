#include "appConfig.h"

static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("appConfig");

bool ConfigManager::parseConfig()
{	
	boost::unique_lock<boost::shared_mutex> lock(configLock);

	getMySqlConfig();
	getZDMarketConfig();
	getCITMarketConfig();
}


bool ConfigManager::getMySqlConfig()
{
	
	try
	{
		boost::property_tree::ptree	tree;
		boost::property_tree::json_parser::read_json(mysqlConConfigPath, tree);
		boost::property_tree::ptree		connectTree = tree.get_child("connect");

		mySqlConfig.url = connectTree.get<std::string>("url");
		mySqlConfig.user= connectTree.get<std::string>("user");
		mySqlConfig.passwd = connectTree.get<std::string>("passwd");
		mySqlConfig.dbName = connectTree.get<std::string>("dbName");

		return true;
	}
	catch (boost::property_tree::ptree_error&e)
	{
		logger->error("getMySqlConifg() paser config exception:{}", e.what());

		return false;
	}
}

bool ConfigManager::getZDMarketConfig()
{

	try
	{
		boost::property_tree::ptree	tree;
		boost::property_tree::json_parser::read_json(marketConfigPath, tree);
		boost::property_tree::ptree		zDMarketTree = tree.get_child("zDMarketConfig");

		zDMarketConfig.userId = zDMarketTree.get<std::string>("userId");
		zDMarketConfig.password = zDMarketTree.get<std::string>("password");
		zDMarketConfig.heartBeat = zDMarketTree.get<std::string>("heartBeat");
		zDMarketConfig.authorCode = zDMarketTree.get<std::string>("authorCode");
		zDMarketConfig.macAddress = zDMarketTree.get<std::string>("macAddress");
		zDMarketConfig.computerName = zDMarketTree.get<std::string>("computerName");
		zDMarketConfig.softwareName = zDMarketTree.get<std::string>("softwareName");
		zDMarketConfig.serverAddress = zDMarketTree.get<std::string>("serverAddress");
		zDMarketConfig.softwareVersion = zDMarketTree.get<std::string>("softwareVersion");

		return true;
	}
	catch (boost::property_tree::ptree_error&e)
	{
		logger->error("getZDMarketConfig() paser config exception:{}", e.what());

		return false;
	}
}

bool ConfigManager::getCITMarketConfig()
{

	try
	{
		boost::property_tree::ptree	tree;
		boost::property_tree::json_parser::read_json(marketConfigPath, tree);
		boost::property_tree::ptree		cITMarketTree = tree.get_child("CIT_MarketConfig");

		cITMarketConfig.url = cITMarketTree.get<std::string>("url");
		cITMarketConfig.brokerID = cITMarketTree.get<std::string>("brokerID");
		cITMarketConfig.userID = cITMarketTree.get<std::string>("userID");
		cITMarketConfig.passwd = cITMarketTree.get<std::string>("passwd");

		cITMarketConfig.uniqueMarket = cITMarketTree.get<std::string>("uniqueMarket");
		cITMarketConfig.startTime = cITMarketTree.get<std::string>("startTime");
		cITMarketConfig.endTime = cITMarketTree.get<std::string>("endTime");

		return true;
	}
	catch (boost::property_tree::ptree_error&e)
	{
		logger->error("getCITMarketConfig() paser config exception:{}", e.what());

		return false;
	}
}