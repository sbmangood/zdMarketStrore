#pragma once
#include "globalInclude.h"
#include <communicate/endPoint/endpoint.h>
#include <staticDatas/staticDatas.h>
using namespace Directaccess;

class ZD_Future :public IFutureEvent
{
private:
	typedef map<string, CFutureRspAccountField> CMapAccount;
	// 成员变量
private:
	string				m_orderNo;				// 本地单号
	CLockBool			m_bIsLogin;				// 是否登陆
	string				m_systemNo;				// 系统单号
	LONG				m_iQryPage;				// 分页标志
	ZDMarketConfig		m_appConfig;			// 配置文件
	CFutureApi*			m_pFutureApi;			// 期货接口
	CMapAccount			m_mapAccounts;			// 所有账号

	std::shared_ptr<Endpoint>					endPoint;
	bool										hasEndPoint;

	GlobalInclude								globalInclude;

		// 构造析构
public:
	ZD_Future() {
		m_iQryPage = 0;
		m_bIsLogin = false;
		m_pFutureApi = 0;
		m_appConfig = ConfigManager::get_mutable_instance().zDMarketConfig;
	}
	virtual ~ZD_Future() {}

	bool Create();
	void Destroy();
	bool IsLogin();
	bool IsCreate();

	bool setEndPoint(std::shared_ptr<Endpoint> ep);
//连接函数
	void ReqUserLogin();
//查询请求函数
	void QryInstrument();
//连接函数
public:
	virtual void OnFrontConnected()override;
	virtual void OnFrontDisconnected(int iReason)override;
	virtual void OnHeartBeatWarning(int iTimeLapse)override;

	virtual void OnRspUserLogin(CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)override;
	
//推送函数
	virtual void OnRtnTrade(CFutureRtnTradeField *pRtnTrade, CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)override;
	virtual void OnRtnOrder(CFutureRtnOrderField *pRtnOrder, CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)override;
//查询函数
	virtual void OnRspQryInstrument(CFutureRspInstrumentField *pRspInstrument, CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)override;

};