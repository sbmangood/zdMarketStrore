#include "event.h"
#include <boost/scoped_ptr.hpp>

Event::Event()
{

}

Event::~Event()
{

}

int Event::wait(unsigned long long timeOutInS)
{
	boost::chrono::milliseconds timeOut(timeOutInS);
	boost::unique_lock<boost::mutex> lock(mtx);
	return cond.wait_for(lock, timeOut) == boost::cv_status::no_timeout ? Event_Ok : Event_Time_Out;
}

void Event::set()
{
	cond.notify_all();
}

void Event::set_one()
{
	cond.notify_one();
}