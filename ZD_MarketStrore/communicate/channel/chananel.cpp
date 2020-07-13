#include "chananel.h"
#include <tools/log/log.h>
#include<string>
static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("trade_server.IChannel");

IChannel::IChannel(boost::shared_ptr<IProtocol> protocol):
protocol_(protocol)
{
	_sending = true;
	_send_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&IChannel::send_work, this)));
	protocol_->addListener(IProtocol::EVENT_MSG_ARRIVED, &IChannel::handlerMsgDecoded, this);
	protocol_->addListener(IProtocol::EVENT_MSG_CORRUPTED, &IChannel::handlerMsgCorrupted, this);
}

IChannel::~IChannel()
{

}

int IChannel::handlerMsgDecoded(unsigned int eventid, EventDispatcher< const boost::any& > & endpoint, const boost::any& msg)
{
	dispatchMessage(eventid, msg);
	return 0;
}

int IChannel::handlerMsgCorrupted(unsigned int eventid, EventDispatcher< const boost::any& > & endpoint, const boost::any& msg)
{
	dispatchMessage(eventid, msg);
	return 0;
}

void IChannel::onDataArrived(const unsigned char* buffer, size_t size)
{
	//logger->info(std::string((char*)buffer,size).c_str());
	protocol_->decode_data(buffer, size);
}

void IChannel::async_send(const boost::any& msg)
{
	try
	{
		boost::lock_guard<boost::mutex> lock(msg_queue_lock);
		_msg_queue.push(msg);
		send_event_.set();
	}
	catch (const std::exception& e)
	{
	}
	catch (...)
	{

	}
}

void IChannel::send_work()
{
	while (_sending)
	{
		{
			
			send_event_.wait(5);
			{
				//boost::unique_lock<boost::mutex> lock(msg_queue_lock);
				msg_queue_lock.lock();
				if (!_msg_queue.empty())
				{
					boost::any msg= _msg_queue.front();
					_msg_queue.pop();
					msg_queue_lock.unlock();
					int sendSize = 0;
					protocol_->encode_data(msg, buffer, sendSize);
					send_data(&buffer[0], sendSize);
				}
				else
				{
					msg_queue_lock.unlock();
				}
	
			}

		}
	}
}

void IChannel::stop()
{
	_sending = false;
	if (_send_thread->joinable())
	{
		_send_thread->join();
	}
}