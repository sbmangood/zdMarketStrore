#pragma once
#include "globalInclude.h"
#include <communicate/endPoint/endpoint.h>
#include <staticDatas/staticDatas.h>
using namespace Directaccess;

class ZD_Future :public IFutureEvent
{
private:
	typedef map<string, CFutureRspAccountField> CMapAccount;
	// ��Ա����
private:
	string				m_orderNo;				// ���ص���
	CLockBool			m_bIsLogin;				// �Ƿ��½
	string				m_systemNo;				// ϵͳ����
	LONG				m_iQryPage;				// ��ҳ��־
	ZDMarketConfig		m_appConfig;			// �����ļ�
	CFutureApi*			m_pFutureApi;			// �ڻ��ӿ�
	CMapAccount			m_mapAccounts;			// �����˺�

	std::shared_ptr<Endpoint>					endPoint;
	bool										hasEndPoint;

	GlobalInclude								globalInclude;

		// ��������
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
//���Ӻ���
	void ReqUserLogin();
//��ѯ������
	void QryInstrument();
//���Ӻ���
public:
	virtual void OnFrontConnected()override;
	virtual void OnFrontDisconnected(int iReason)override;
	virtual void OnHeartBeatWarning(int iTimeLapse)override;

	virtual void OnRspUserLogin(CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)override;
	
//���ͺ���
	virtual void OnRtnTrade(CFutureRtnTradeField *pRtnTrade, CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)override;
	virtual void OnRtnOrder(CFutureRtnOrderField *pRtnOrder, CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)override;
//��ѯ����
	virtual void OnRspQryInstrument(CFutureRspInstrumentField *pRspInstrument, CFutureRspInfoField *pRspInfo, int iRequestID, bool bIsLast)override;

};