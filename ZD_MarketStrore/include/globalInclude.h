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
	std::string contract;		//��Լ��
	std::string dueDate;		//��ֹ����

	void getData(Directaccess::CFutureRspInstrumentField *pRspInstrument)
	{
		contract = pRspInstrument->CommodityCode;
		dueDate = pRspInstrument->LastTradeDay;
	}

};

class CLockBool
{
private:
	std::mutex		m_mutex;		// ��ֵ��
	bool			m_value;		// ����ֵ

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
	OutFutureLoginTableStruct ofLogin;	//�����ڻ���½��¼����
	T_OutPricesTableStruct topts;		//�����ڻ�ʵʱ�������	
	T_TradeDetailsTableStruct ttdts;	//���⵱ǰ�ĳɽ���¼����
	ZD_outExProductSubTable zoest;		//������Ҫ���ĵ�Ʒ���б�,����б������ݱ�outExProductSubTable��ȡ

	std::string lastDay;

};
