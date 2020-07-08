#pragma once
#include<boost/signals2.hpp>
#include<boost/any.hpp>
#include<communicate/eventDispatcher/event_dispatcher.h>
#include<boost/any.hpp>


class IProtocol : public EventDispatcher<const boost::any&>
{
public:
	enum 
	{
		EVENT_MSG_ARRIVED = 0,
		EVENT_MSG_CORRUPTED,
	};
public:
	IProtocol()
	{

	}
	virtual ~IProtocol()
	{

	}
public:
	virtual void decode_data(const unsigned char* data,unsigned int length) = 0;
	virtual void encode_data(const boost::any& message, std::vector<unsigned char>& buffer)
	{};
	virtual void encode_data(const boost::any& message, std::vector<unsigned char>& buffer, int &size) = 0;
	
	virtual void reset() = 0;
private:
};