#pragma once
#include <globalInclude.h>
#include <stdio.h>
#include <communicate/endPoint/endpoint.h>
#include "../staticDatas/staticDatas.h"



using namespace Directaccess;


class ZdMarket : public IMarketEvent
{
public:
	const std::string ZD_CONNECTING_SERVER = "正在连接并登陆服务器....";
	const std::string ZD_LONIN_SERVER = "已经连接并登陆上了服务器.";

private:
	typedef map<string, CMarketRspUserLoginField> CMapAccount;

	// 成员变量
private:
	CLockBool			m_bIsLogin;				// 是否登陆
	ZDMarketConfig		m_appConfig;			// 配置文件
	CMarketApi*			m_pMarketApi;			// 行情接口

	std::shared_ptr<Endpoint>		endPoint;
	bool							hasEndPoint;


	//构造析构
public:
	ZdMarket() {
		m_bIsLogin = false;
		m_pMarketApi = 0;
		hasEndPoint = false;
		m_appConfig = ConfigManager::get_mutable_instance().zDMarketConfig;
	}
	virtual ~ZdMarket() {}

	// 创建销毁
public:

	bool setEndPoint(std::shared_ptr<Endpoint> ep);
	bool Create();

	void Destroy();

	bool IsLogin()
	{
		return m_bIsLogin;
	}
	bool IsCreate()
	{
		return m_pMarketApi != NULL;
	}

	//一般函数
public:
	string GetDateTime();
	bool juggePrice(const std::string &price);

	void SubscStockBroker();

	void UnsubscAllMarket();

	void SendLoginRequest();

	void SubscStockMarket();
	
	void SubscFutureBroker();

	void SubscFutureMarket();

	void UnsubscStockMarket();

	void UnsubscFutureMarket();

	void SubscFutureMarketByExg();


	//虚函数
public:
	virtual void OnFrontConnected() override;

	virtual void OnFrontDisconnected(int iReason) override;

	virtual void OnHeartBeatWarning(int iTimeLapse);

	virtual void OnRspUserLogin(CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) override;

	virtual void OnRspUserLogout(CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) override;

	virtual void OnRspTradeDate(CMarketRspTradeDateField *pRspTradeDate, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) override;

	virtual void OnRspBrokerData(CMarketRspBrokerDataField *pRspBrokerData, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) override;

	virtual void OnRspMarketData(CMarketRspMarketDataField *pRspMarketData, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) override;

	
private:

	GlobalInclude globalInclude;


};

