#pragma once

#include "globalInclude.h"
#include "staticDatas/staticDatas.h"

class CIT_Market:public CThostFtdcMdSpi
{
	//������Ҫͨ������ĳɽ����Ƿ�仯���ж��Ƿ��гɽ� 
	struct CIT_MarketMapValue
	{
		std::string contract;
		//�ɽ���
		int vol;
	};

public:
	CIT_Market(CThostFtdcMdApi *pUserApi,const CIT_MarketConfig &cf);
	CIT_Market() = delete;
	~CIT_Market() {}

	bool setEndPoint(std::shared_ptr<Endpoint> ep);


	// ���ͻ����뽻���й�ϵͳͨ�����ӶϿ�ʱ���÷���������
	virtual void OnFrontConnected()override;
	virtual void OnFrontDisconnected(int nReason)override;
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;
	// ����û�����ĳ���֪ͨ
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;

	// ����Ӧ��
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)override;
	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)override;

private:
	std::shared_ptr<Endpoint>					endPoint;
	bool										hasEndPoint;

	GlobalInclude								globalInclude;



	// ָ��CThostFtdcMdApiʵ����ָ��
	CThostFtdcMdApi *m_pUserApi;
	//��¼����
	CThostFtdcReqUserLoginField gReqUserLogin;
	CIT_MarketConfig			cITMarketConfig;

	std::map<std::string, CIT_MarketMapValue>		CIT_MarketMap;
	
private:
	void dealTime(std::string &time);
	
	bool findAndUpdateCIT_MarketMap(const std::string &contract, int vol);
};