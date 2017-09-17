#include "Timestamp.h"
#include "TimeQueue.h"
#include "Logging.h"
#include "Timer.h"
#include "EventLoop.h"
#include <dlfcn.h>
#include <algorithm>
#include<sys/timerfd.h>
#include<stdio.h>
#include<boost/bind.hpp>
#include"Channel.h"
#include<stdint.h>
struct timespec howLongFromNow(Timer* timer){

    int64_t useconds = timer->expiration().microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
	


    struct timespec it;
    it. tv_sec = static_cast<time_t>(useconds / Timestamp::kMicroSecondsPerSecond);

	it. tv_nsec = static_cast<time_t>(useconds % Timestamp::kMicroSecondsPerSecond * 1000);

	return it;

	
}

int creatTimefd(){
	int timefd =  timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (timefd < 0)
		LOG_FATAL<<"Failed in timerfd_create";
	return timefd;
}

TimeQueue::TimeQueue(EventLoop* loop):loop_(loop),timefd_(creatTimefd()),channel(new Channel(loop_, timefd_))
{
    channel->setReadCallback(boost::bind(&TimeQueue::handleRead, this));
    channel->enableRead();

}

TimeQueue::~TimeQueue(){}
void TimeQueue::addTimer(Timer* ptimer)
{
    loop_->runInLoop(boost::bind(&TimeQueue::addTimerInLoop,this,ptimer));
}

void TimeQueue::addTimerInLoop(Timer* ptimer){

  bool earliest = insert(ptimer);
  if (earliest){
    resetTimefd(ptimer);
  }
}

bool TimeQueue::insert(Timer* timer){
	bool earliest = false;
    Entry entry(timer->expiration(), timer);
	Iterator it = timers_.begin();
	if (it == timers_. end()||entry < *it)
	{
	  earliest = true;
	}
	{
      timers_. insert(entry);
	}
	return earliest;

}
void TimeQueue::resetTimefd(Timer* timer){
    struct timespec value = howLongFromNow(timer);
    struct itimerspec newValue,old;
    bzero(&newValue, sizeof newValue);
    bzero(&old, sizeof old);
    newValue. it_value = value;
    int i = timerfd_settime(timefd_, 0, &newValue,&old) ;

    if (i)
		LOG_FATAL<<"settime error";


}
void TimeQueue::reset(Timestamp now, std::vector<Entry>& expired){
  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
      {
      	if (it->second->repeat())
      	{
      		it->second->reset();
            insert(it->second);

      		
      	}  

	}
  if (!timers_. empty())
    resetTimefd((*(timers_.begin())).second);
}
void TimeQueue::readTimerfd()
{
    int64_t exp = 0;
    if ( (read(timefd_,&exp,sizeof exp)) != sizeof exp)
        LOG_FATAL<<"timerfd read error";

}

void TimeQueue::handleRead(){
  readTimerfd();
  Timestamp now(Timestamp::now());
  std::vector<Entry> expired = getExpiration(now);

  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    it->second->run();
  }
  reset(now, expired);
	
}
std::vector<TimeQueue::Entry> TimeQueue::getExpiration(Timestamp now){
    Entry entry (now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	Iterator it = timers_.lower_bound(entry);
	std::vector<Entry> expired;
	std::copy(timers_.begin(), it, back_inserter(expired));
	timers_. erase(timers_.begin(), it);
	return expired;

}
