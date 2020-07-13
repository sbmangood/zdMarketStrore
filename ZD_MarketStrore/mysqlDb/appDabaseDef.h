#pragma once

#include <iostream>
#include <string>




/*********************************************************************************************
	*	table
/*********************************************************************************************/
class DB_TableStructStoreBase
{
public:
	virtual std::string createTable() = 0;
	std::string getTodayStr()
	{
		time_t t;
		time(&t);
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%Y_%m_%d", localtime(&t));
		return tmp;
	}
};

//国外期货登陆记录表格
class OutFutureLoginTableStruct :public DB_TableStructStoreBase
{
public:
	const std::string tableName = "outFutureLoginTable_";
	const std::string id = "ID";
	const std::string result = "Result";
	const std::string date = "Time";

	std::string createTable()override
	{
		std::string cmd = "create table if not exists "
			+ tableName
			+ "("
			+ id + " INT unsigned auto_increment, "
			+ result + " VARCHAR(100), "
			+ date + " DATETIME, "
			+ "primary key (" + id + ") "
			+ ") ENGINE=InnoDB DEFAULT CHARSET=utf8;";

		return cmd;
	}

};
//内盘期货实时行情表格
class T_InnPricesTableStruct : public DB_TableStructStoreBase
{
public:
	const std::string tableName = "T_InnPrices";
	const std::string constractNo = "sContractNo";
	const std::string time = "sTime";
	const std::string price = "nPrice";
	std::string createTable()override
	{
		std::string  cmd = "create table if not exists "
			+ tableName
			+ "( "
			+ constractNo + " NVARCHAR(20), "
			+ time + " DATETIME, "
			+ price + " NUMERIC(15,6),"
			+ "insertTime timestamp NOT NULL default CURRENT_TIMESTAMP"
			+ ")ENGINE=InnoDB DEFAULT CHARSET=utf8;";
		return cmd;
	}
};

//国外期货实时行情表格
class T_OutPricesTableStruct :public DB_TableStructStoreBase
{
public:
	const std::string tableName = "T_OutPrices";
	const std::string contract = "sContractNo";
	const std::string dueDate = "dDueDate";
	const std::string time = "sTime";
	const std::string price = "nPrice";
	const std::string exchange = "sExchangePlace";

	std::string createTable() override
	{
		std::string  cmd = "create table if not exists "
			+ tableName
			+ "( "
			+ contract + " NVARCHAR(20), "
			+ dueDate + " DATE, "
			+ time + " DATETIME, "
			+ price + " NUMERIC(15,6), "
			+ exchange + " NVARCHAR(20),"
			+"insertTime timestamp NOT NULL default CURRENT_TIMESTAMP"
			+ " )ENGINE=InnoDB DEFAULT CHARSET=utf8;";
		return cmd;
	}
};
//国外当前的成交记录表格
class T_TradeDetailsTableStruct :public DB_TableStructStoreBase
{
public:
	const std::string tableName = "T_TradeDetails";
	const std::string reportNo = "ReportNo";
	const std::string tradeNo = "TradeNo";
	const std::string exchange = "sExchangePlace";
	const std::string brokeCompany = "sBrokeCompany";
	const std::string contract = "sContractNo";
	const std::string dueDate = "dDueDate";
	const std::string bidAsk = "sBuySale";
	const std::string lotQty = "nLotQty";
	const std::string price = "nPrice";
	const std::string charge = "nCharge";
	const std::string tradeDate = "sTradeDate";
	const std::string tradeTime = "sTradeTime";
	const std::string openClose = "sOpenClose";

	std::string createTable()override
	{
		std::string cmd = "create table if not exists "
			+ tableName
			+ "("
			+ reportNo + " NVARCHAR(20), "
			+ tradeNo + " NVARCHAR(20), "
			+ exchange + " NVARCHAR(20), "
			+ brokeCompany + " NVARCHAR(20), "
			+ contract + " NVARCHAR(20), "
			+ dueDate + " DATE, "
			+ bidAsk + " NVARCHAR(4),"
			+ lotQty + " NUMERIC(10,2), "
			+ price + " NUMERIC(15,6), "
			+ charge + " NUMERIC(10,2), "
			+ tradeDate + " DATE, "
			+ tradeTime + " DATETIME, "
			+ openClose + " NVARCHAR(10),"
			+"insertTime timestamp  NOT NULL default CURRENT_TIMESTAMP"
			+ " )ENGINE=InnoDB DEFAULT CHARSET=utf8;";

		return cmd;
	}

};


//外盘需要订阅的品种列表,这个列表从数据表outExProductSubTable读取
struct ZD_outExProductSubTable :public DB_TableStructStoreBase
{
	const std::string tableName = "outExProductSubTable";
	const std::string subProduct = "subProduct";//like "CME:CL"
	std::string createTable()override
	{
		std::string  cmd = "create table if not exists "
			+ tableName
			+ "( "
			+ subProduct + " NVARCHAR(20)"		
			+ " )ENGINE=InnoDB DEFAULT CHARSET=utf8;";
		return cmd;
	}
};

//内盘需要订阅的品种列表,这个列表从数据表inExProductSubTable读取
struct ZD_inExProductSubTable :public DB_TableStructStoreBase
{
	const std::string tableName = "inExProductSubTable";
	const std::string subProduct = "subProduct";//like "CME:CL"
	std::string createTable()override
	{
		std::string  cmd = "create table if not exists "
			+ tableName
			+ "( "
			+ subProduct + " NVARCHAR(20)"
			+ " )ENGINE=InnoDB DEFAULT CHARSET=utf8;";
		return cmd;
	}
};



/*********************************************************************************************
	*	data
/*********************************************************************************************/
//订阅的外盘品种结构定义
struct outExProductSubStruct
{
	std::string exchange;
	std::string product;
};

//订阅的内盘品种结构定义
struct inExProductSubStruct
{
	std::string exchange;
	std::string product;
};

//外盘行情存数据库(直达)
struct ZD_DBMarketData
{
	std::string contract;
	std::string dueDate;
	std::string time;
	std::string price;
	std::string exchange;

};

//内盘行情存数据库(CIT)
struct CIT_DBMarketData
{
	std::string contract;
	std::string time;
	std::string price;
};
//当天交易成交记录存数据库
struct T_TradeDetailsData
{
	std::string reportNo;
	std::string tradeNo;
	std::string exchange;
	std::string brokeCompany;
	std::string contract;
	std::string dueDate;
	std::string bidAsk;
	std::string lotQty;
	std::string price;
	std::string charge;
	std::string tradeDate;
	std::string tradeTime;
	std::string openClose;
};
struct DB_LoginData
{
	std::string msg;
};



/*********************************************************************************************
	*	other
/*********************************************************************************************/
//用于切换数据库指令
struct SwitchDataBase
{
	std::string dbName;
};