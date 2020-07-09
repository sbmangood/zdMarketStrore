#pragma once

#include "globalInclude.h"
using namespace Directaccess;
class StaticDatas :public  boost::serialization::singleton<StaticDatas>
{
public:
	//������Ҫ���ĵ�Ʒ���б�,����б�����ݱ�outExProductSubTable��ȡ
	std::list<outExProductSubStruct>					outExProductSubList;
	//������Ҫ���ĵ�Ʒ���б�,����б�����ݱ�inExProductSubTable��ȡ
	std::list<inExProductSubStruct>						inExProductSubList;

	
	//db
	std::shared_ptr<DBPool>								dbPool;
	MysqlConConfig										dbConfig;
	Connection											*con;

	ZD_outExProductSubTable				oSubTable;
	ZD_inExProductSubTable				iSubTable;

public:
	void init();

	void insertUpdateContractMap(const std::string &contract, const m_ContractInfor &cIfor);
	std::string getDueDayFromContractMap(const std::string &contract);

private:	
	bool readDbBaseData();
	bool readOutExProductSubList();
	bool readInExProductSubList();
	bool connectDb();


private:
	std::map<std::string, m_ContractInfor>				contractMap;//���̴ӽ��׷�������ȡ�ĺ�Լmap
	std::mutex											contractMapLock;

};