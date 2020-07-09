#pragma once

#include <iostream>
#include<string>
#include<vector>
#include <map>
#include <set>
#include"boost/container/detail/singleton.hpp"
#include"boost/serialization/singleton.hpp"
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream> 

#include "../tools/log/log.h"

struct ZDMarketConfig
{
	std::string	userId;
	std::string	password;
	std::string	heartBeat;
	std::string	authorCode;
	std::string	macAddress;
	std::string	computerName;
	std::string	softwareName;
	std::string	serverAddress;
	std::string	softwareVersion;

	ZDMarketConfig()
	{
		heartBeat = "150";
		softwareName = "api";
		userId = "sy01";//
		password = "888888";
		softwareVersion = "2";
		macAddress = "song_123";
		computerName = "WJQ-PC";
		serverAddress = "127.0.0.1:12345";
		authorCode = "porl99bbo/jrfib5xxgagza5giggzr/u";
	}
};

struct MysqlConConfig
{
	std::string url;
	std::string user;
	std::string passwd;
	std::string dbName;
	MysqlConConfig()
	{
		url = "tcp://127.0.0.1:3306";
		user = "root";
		passwd = "123456";
		dbName = "ywh_test";
	}
	
};





class ConfigManager :public boost::serialization::singleton<ConfigManager>
{
public:

	ConfigManager()
	{
		parseConfig();
	}

	bool parseConfig();

private:
	boost::shared_mutex			configLock;

	const std::string			configPath= "./configs/mainConfig.json";	
	const std::string 			mysqlConConfigPath = "./configs/mySqlConfig/mySqlConfig.json";
	const std::string			marketConfigPath = "./configs/marketConfig/marketConfig.json";
public:	
	MysqlConConfig				mySqlConfig;	//数据库配置
	ZDMarketConfig				zDMarketConfig; //直达(外盘)账户配置


private:
	bool getMySqlConfig();
	bool getZDMarketConfig();

};