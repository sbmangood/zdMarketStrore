#pragma once
#include <globalInclude.h>

class MarketManagerBase
{
public:
	explicit MarketManagerBase(std::shared_ptr<Endpoint> ep):
		endPoint_ptr(ep)
	{

	}
	~MarketManagerBase() 
	{
		stop();
	}
	virtual void start()=0;
	virtual void stop() = 0;
private:
	MarketManagerBase()
	{}
	
private:
	std::shared_ptr<Endpoint>		endPoint_ptr;
};