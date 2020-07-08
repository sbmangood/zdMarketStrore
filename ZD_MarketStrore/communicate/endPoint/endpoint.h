#pragma once

#include "communicate/channel/chananel.h"

class Endpoint
{
public:
	Endpoint();
	~Endpoint();
public:
	typedef boost::function<void(unsigned int,const boost::any&)> callbackHandler;
public:
	bool bind(boost::shared_ptr<IChannel> chnanel, callbackHandler callFun);
	bool start();
	void stop();
	void send_msg(const boost::any& msg);
protected:
	int handlerMsgDecoded(unsigned int eventid, EventDispatcher< const boost::any& > & endpointm, const boost::any& msg);
private:
	boost::shared_ptr<IChannel> chnanel_;
	boost::signals2::signal<void(unsigned int, const boost::any&)> messgae_Arrivaled_;
};