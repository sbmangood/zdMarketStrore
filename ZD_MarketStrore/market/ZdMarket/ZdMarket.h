#pragma once
#include <globalInclude.h>
#include <stdio.h>
#include <communicate/endPoint/endpoint.h>
#include "../staticDatas/staticDatas.h"



using namespace Directaccess;


class ZdMarket : public IMarketEvent
{
public:
	const std::string ZD_CONNECTING_SERVER = "�������Ӳ���½������....";
	const std::string ZD_LONIN_SERVER = "�Ѿ����Ӳ���½���˷�����.";

private:
	typedef map<string, CMarketRspUserLoginField> CMapAccount;

	// ��Ա����
private:
	CLockBool			m_bIsLogin;				// �Ƿ��½
	ZDMarketConfig		m_appConfig;			// �����ļ�
	CMarketApi*			m_pMarketApi;			// ����ӿ�

	std::shared_ptr<Endpoint>		endPoint;
	bool							hasEndPoint;


	//��������
public:
	ZdMarket() {
		m_bIsLogin = false;
		m_pMarketApi = 0;
		hasEndPoint = false;
		m_appConfig = ConfigManager::get_mutable_instance().zDMarketConfig;
	}
	virtual ~ZdMarket() {}

	// ��������
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

	//һ�㺯��
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


	//�麯��
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

