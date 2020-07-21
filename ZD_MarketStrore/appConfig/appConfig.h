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

	//因为行情数据量太大存数据库太费时,必须批处理,时间和数量两个条件满足一个条件就触发批处理
	//需要根据行情实际情况合理的设置值
	int batchTime;//秒
	int batchNum;

	
};

//内盘CIT 接口配置
struct CIT_MarketConfig
{
	std::string url;
	std::string brokerID;
	std::string userID;
	std::string passwd;

	//因为CIT接口早上7:40点左右会重复发送昨晚的行情,所以程序后面会判断不存储
	//另外下午16:09-21:00之间也会发生重复数据
	//uniqueMarket!="false"不去重,其他的去重
	//startTime-endTime 之内数据粗略的不重复发送 
	//格式:比如 083000-093000 
	std::string uniqueMarket;
	
	std::vector<std::string>	startTimeVc;
	std::vector<std::string>	endTimeVc;
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