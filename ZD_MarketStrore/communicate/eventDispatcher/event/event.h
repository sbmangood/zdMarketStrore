#pragma once
#include <boost/thread/mutex.hpp>  
#include <boost/thread/condition.hpp>  

class Event
{
public:
	Event();
	~Event();
public:
	enum
	{
		Event_Time_Out = 0,
		Event_Ok,
	};
public:
	int wait(unsigned long long timeOut = 0xffffffffffffffLL);
	void set();
	void set_one();
private:
	boost::mutex mtx;
	boost::condition_variable cond;
};