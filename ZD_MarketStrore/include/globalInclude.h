#pragma once
#pragma warning( disable : 4996 )

#include <list>

#include "../tools/log/log.h"
#include "../appConfig/appConfig.h"
#include "../mysqlDb/mysqlConnPool/mySqlConnPool.h"
#include "../tools/nlohmann/json.hpp"
#include "../tools/winDump/WinDump.h"
#include "../communicate/endPoint/endpoint.h"
#include "mysqlDb/appDabaseDef.h"

#include <ZdApi/DAMarketApi.h>
#include <ZdApi/DAMarketStruct.h>
#include <ZdApi/DAFutureApi.h>
#include <ZdApi/DAFutureStruct.h>



#define safe_cpy(tar, src)	SafeStrcpy(tar, sizeof(tar), src);

inline void SafeStrcpy(char* tar, size_t tar_size, const char* src)
{
	size_t n = __min(strlen(src), tar_size - 1);
	memcpy(tar, src, n);
	tar[n] = 0;
}

struct m_ContractInfor
{
	std::string contract;		//合约号
	std::string dueDate;		//截止日期

	void getData(Directaccess::CFutureRspInstrumentField *pRspInstrument)
	{
		contract = pRspInstrument->CommodityCode;
		dueDate = pRspInstrument->LastTradeDay;
	}

};

class CLockBool
{
private:
	std::mutex		m_mutex;		// 数值锁
	bool			m_value;		// 布尔值

public:
	CLockBool() :m_value(false) {}
	virtual ~CLockBool() {}

public:
	inline void operator = (bool value)
	{
		m_mutex.lock();
		m_value = value;
		m_mutex.unlock();
	}
	inline operator bool()
	{
		m_mutex.lock();
		bool temp = m_value;
		m_mutex.unlock();
		return temp;
	}
};




class GlobalInclude
{
public:
	GlobalInclude();

	std::string getTimeStr();
	std::string getTodayStr();

public:
	//db name
	const std::string dbName = "ywh_test";
	//table names
	OutFutureLoginTableStruct ofLogin;	//国外期货登陆记录表格
	T_OutPricesTableStruct topts;		//国外期货实时行情表格	
	T_TradeDetailsTableStruct ttdts;	//国外当前的成交记录表格
	ZD_outExProductSubTable zoest;		//外盘需要订阅的品种列表,这个列表从数据表outExProductSubTable读取

	std::string lastDay;

};

