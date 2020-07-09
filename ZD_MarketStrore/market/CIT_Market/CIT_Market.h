#pragma once

#include "globalInclude.h"
#include "staticDatas/staticDatas.h"

class CIT_Market:public CThostFtdcMdSpi
{
public:
	CIT_Market(CThostFtdcMdApi *pUserApi);
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
	char gProductDBStr[255];
};