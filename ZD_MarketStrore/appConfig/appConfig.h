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

//����CIT �ӿ�����
struct CIT_MarketConfig
{
	std::string url;
	std::string brokerID;
	std::string userID;
	std::string passwd;

	//��ΪCIT�ӿ�����9�����һ��ظ��������������,���Գ��������жϲ��洢
	//uniqueMarket="true"ȥ��,�����Ĳ�ȥ��
	//startTime-endTime ֮�����ݴ��ԵĲ��ظ����� 
	//��ʽ:���� 083000-093000 
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
	MysqlConConfig				mySqlConfig;	//���ݿ�����
	ZDMarketConfig				zDMarketConfig; //ֱ��(����)�˻�����
	CIT_MarketConfig			cITMarketConfig; //����CIT �ӿ�����

private:
	bool getMySqlConfig();
	bool getZDMarketConfig();
	bool getCITMarketConfig();

};