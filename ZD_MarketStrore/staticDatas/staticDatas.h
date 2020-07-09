#pragma once

#include "globalInclude.h"
using namespace Directaccess;
class StaticDatas :public  boost::serialization::singleton<StaticDatas>
{
public:
	//外盘需要订阅的品种列表,这个列表从数据表outExProductSubTable读取
	std::list<outExProductSubStruct>					outExProductSubList;
	//外盘需要订阅的品种列表,这个列表从数据表inExProductSubTable读取
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
	std::map<std::string, m_ContractInfor>				contractMap;//外盘从交易服务器读取的合约map
	std::mutex											contractMapLock;

};