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


};

struct MysqlConConfig
{
	std::string url;
	std::string user;
	std::string passwd;
	std::string dbName;

	
};

//内盘CIT 接口配置
struct CIT_MarketConfig
{
	std::string url;
	std::string brokerID;
	std::string userID;
	std::string passwd;

	//因为CIT接口早上9点左右会重复发送昨晚的行情,所以程序后面会判断不存储
	//uniqueMarket="true"去重,其他的不去重
	//startTime-endTime 之内数据粗略的不重复发送 
	//格式:比如 083000-093000 
	std::string uniqueMarket;
	std::string startTime;
	std::string endTime;
	//to do
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
	CIT_MarketConfig			cITMarketConfig; //内盘CIT 接口配置

private:
	bool getMySqlConfig();
	bool getZDMarketConfig();
	bool getCITMarketConfig();

};