#pragma once
#include <globalInclude.h>




class MarketStoreProtocal :public IProtocol
{
public:
	MarketStoreProtocal();
public:
	virtual void decode_data(const unsigned char* data, unsigned int length) override;
	virtual void encode_data(const boost::any& message, std::vector<unsigned char>& buffer, int &sendSize) override;
	virtual void reset()override;

private:
	GlobalInclude globalInclude;

private:
	std::string toDbString(const std::string & str);
	std::string toDbNum(const std::string &str);

};