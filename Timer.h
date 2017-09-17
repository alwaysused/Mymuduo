#ifndef TIMER_H_
#define TIMER_H_
#include<boost/function.hpp>

#include "Timestamp.h"


class Timer
{
 public:
    typedef boost::function<void()> Callback;
	Timer(Timestamp tp, Callback cb,double interval):expiration_(tp), 
                                                callback_(cb),interval_(interval),
												repeat_(interval_ > 0){}
	Timestamp expiration(){return expiration_;}
	bool repeat(){return repeat_;}
	void run(){callback_();}
    ~Timer(){}
    void reset(){expiration_ = Timestamp::delay(interval_);}
 private:
   Timestamp expiration_;
   Callback callback_;
   double interval_;
   bool repeat_;	
};
#endif

