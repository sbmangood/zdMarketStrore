#pragma once
#include "communicate/eventDispatcher/event_dispatcher.h"
#include <communicate/protocal/protocol.h>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include <communicate/eventDispatcher/event/event.h>
#include <queue>
#include <boost/any.hpp>



class IChannel: public EventDispatcher<const boost::any&>
{
public:
	IChannel(boost::shared_ptr<IProtocol> protocol);
	virtual ~IChannel();
public:
	virtual bool start() = 0;
	virtual void stop();
	virtual bool send_data(const unsigned char* buffer, unsigned int size) = 0;
	
	
	virtual void async_send(const boost::any& msg);

	//接收数据的事件通知函数
	void onDataArrived(const unsigned char* buffer, size_t size);

protected:
	virtual int handlerMsgDecoded(unsigned int eventid, EventDispatcher< const boost::any& > & endpoint, const boost::any& msg);
	virtual int handlerMsgCorrupted(unsigned int eventid, EventDispatcher< const boost::any& > & endpoint, const boost::any& msg);
private:
	void send_work();
protected:
	bool _sending;
	boost::shared_ptr<IProtocol> protocol_;
	boost::mutex msg_queue_lock;
	std::queue<boost::any> _msg_queue;
	boost::shared_ptr<boost::thread> _send_thread;
	Event send_event_;
	std::vector<unsigned char> buffer;
};