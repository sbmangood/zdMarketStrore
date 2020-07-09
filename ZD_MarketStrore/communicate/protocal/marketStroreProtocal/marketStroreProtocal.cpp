#include "marketStroreProtocal.h"
static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("marketStroreProtocal");


MarketStoreProtocal::MarketStoreProtocal()
{

}

void MarketStoreProtocal::decode_data(const unsigned char* data, unsigned int length)
{
	char *da = (char *)malloc(length);
	memcpy(da, data, length);
	std::string str = std::string(da, length);
	this->dispatchMessage(IProtocol::EVENT_MSG_ARRIVED, boost::any(str));
}
void MarketStoreProtocal::encode_data(const boost::any& msg, std::vector<unsigned char>& buffer, int &sendSize)
{
	buffer.clear();

	std::string cmd;
	if (msg.type() == typeid(std::string))
	{
		cmd = boost::any_cast<std::string>(msg);

	}
	else if (msg.type() == typeid(ZD_DBMarketData))
	{
		ZD_DBMarketData zmd= boost::any_cast<ZD_DBMarketData>(msg);
		cmd = "insert into " +
			globalInclude.topts.tableName + " "
			+ "("
			+ globalInclude.topts.contract + ","
			+ globalInclude.topts.dueDate + ","
			+ globalInclude.topts.time + ","
			+ globalInclude.topts.price + ","
			+ globalInclude.topts.exchange
			+ ") "
			+ "VALUES "
			+ "("
			+ toDbString(zmd.contract) + ","
			+ toDbString(zmd.dueDate)+ ","
			+ toDbString(zmd.time)+","
			+ toDbNum(zmd.price) + ","
			+ toDbString(zmd.exchange)
			+ ");";
			
	}
	else if (msg.type() == typeid(CIT_DBMarketData))
	{
		CIT_DBMarketData cdd = boost::any_cast<CIT_DBMarketData>(msg);
		cmd = "insert into " +
			globalInclude.tipts.tableName + " "
			+ "("
			+ globalInclude.tipts.constractNo + ","
			+ globalInclude.tipts.time + ","
			+ globalInclude.tipts.price		
			+ ") "

			+ "VALUES "

			+ "("
			+ toDbString(cdd.contract) + ","
			+ toDbString(cdd.time) + ","
			+ toDbNum(cdd.price)
			+ ");";
	}
	else if (msg.type() == typeid(T_TradeDetailsData))
	{
		T_TradeDetailsData ttdts = boost::any_cast<T_TradeDetailsData>(msg);
		cmd = "insert into "
			+ globalInclude.ttdts.tableName + " "
			+ "("
			+ globalInclude.ttdts.reportNo + ","
			+ globalInclude.ttdts.tradeNo + ","
			+ globalInclude.ttdts.exchange + ","
			+ globalInclude.ttdts.brokeCompany + ","
			+ globalInclude.ttdts.contract + ","
			+ globalInclude.ttdts.dueDate + ","
			+ globalInclude.ttdts.bidAsk + ","
			+ globalInclude.ttdts.lotQty + ","
			+ globalInclude.ttdts.price + ","
			+ globalInclude.ttdts.charge + ","
			+ globalInclude.ttdts.tradeDate + ","
			+ globalInclude.ttdts.tradeTime + ","
			+ globalInclude.ttdts.openClose
			+ ") "
			+ "VALUES "
			+ "("
			+ toDbString(ttdts.reportNo) + ","
			+ toDbString(ttdts.tradeNo) + ","
			+ toDbString(ttdts.exchange) + ","
			+ toDbString(ttdts.brokeCompany) + ","
			+ toDbString(ttdts.contract) + ","
			+ toDbString(ttdts.dueDate)+ ","
			+ toDbString(ttdts.bidAsk) + ","
			+ toDbNum(ttdts.lotQty) + ","
			+ toDbNum(ttdts.price) + ","
			+ toDbNum(ttdts.charge) + ","
			+ toDbString(ttdts.tradeDate)+ ","
			+ toDbString(ttdts.tradeTime)+","
			+ toDbString(ttdts.openClose)
			+ ");";

	}
	else if (msg.type() == typeid(DB_LoginData))
	{
		DB_LoginData ld = boost::any_cast<DB_LoginData>(msg);
		cmd = "insert into " +
			globalInclude.ofLogin.tableName  + " "
			+ "("
			+ globalInclude.ofLogin.result + ","
			+ globalInclude.ofLogin.date
			+ ") "
			+ "VALUES ("
			+ toDbString(ld.msg) + ","
			+ "LOCALTIME()"
			+ ");";					
	}
	if(buffer.capacity()< cmd.length())
		buffer.resize(cmd.length());
	sendSize = cmd.length();
	memcpy(&buffer[0], cmd.c_str(), cmd.length());

}
void MarketStoreProtocal::reset()
{
	//todo
}

std::string MarketStoreProtocal::toDbString(const std::string & str)
{
	std::string temp = "\"" + str + "\"";
	return temp;
}

std::string MarketStoreProtocal::toDbNum(const std::string &str)
{
	std::string temp ;
	for (auto it = str.begin(); it != str.end(); it++)
	{
		if(*it!=',')
			temp = temp + *it;
	}
	if (temp == "")
		temp = "0";
	return temp;
}

