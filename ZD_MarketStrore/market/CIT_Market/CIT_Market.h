#pragma once

#include "globalInclude.h"
#include "staticDatas/staticDatas.h"

class CIT_Market:public CThostFtdcMdSpi
{
	//内盘主要通过行情的成交量是否变化来判断是否有成交 
	struct CIT_MarketMapValue
	{
		std::string contract;
		//成交量
		int vol;
	};

public:
	CIT_Market(CThostFtdcMdApi *pUserApi,const CIT_MarketConfig &cf);
	CIT_Market() = delete;
	~CIT_Market() {}

	bool setEndPoint(std::shared_ptr<Endpoint> ep);


	// 当客户端与交易托管系统通信连接断开时，该方法被调用
	virtual void OnFrontConnected()override;
	virtual void OnFrontDisconnected(int nReason)override;
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;
	// 针对用户请求的出错通知
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;

	// 行情应答
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)override;
	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;

private:
	std::shared_ptr<Endpoint>					endPoint;
	bool										hasEndPoint;

	GlobalInclude								globalInclude;



	// 指向CThostFtdcMdApi实例的指针
	CThostFtdcMdApi *m_pUserApi;
	//登录数据
	CThostFtdcReqUserLoginField gReqUserLogin;
	CIT_MarketConfig			cITMarketConfig;

	std::map<std::string, CIT_MarketMapValue>		CIT_MarketMap;
	
private:
	void dealTime(std::string &time);
	
	bool findAndUpdateCIT_MarketMap(const std::string &contract, int vol);
};