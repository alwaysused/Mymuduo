#ifndef _TIMEQUEUE_H
#define _TIMEQUEUE_H
#include<boost/scoped_ptr.hpp>
#include <set>
#include <utility>
#include <vector>
class Timestamp;
class Timer;
class Channel;
class EventLoop;
class TimeQueue
{
public:
	typedef std::pair<Timestamp, Timer*> Entry;
	typedef std::set<Entry> TimerList ;
    typedef TimerList::iterator Iterator;
	void addTimer(Timer*);
    bool insert(Timer*);
    void resetTimefd(Timer*);
	void handleRead();
    void readTimerfd();
    TimeQueue(EventLoop*);
    void addTimerInLoop(Timer*);
    void reset(Timestamp,std::vector<Entry>&);
    ~TimeQueue();
    std::vector<Entry> getExpiration(Timestamp now);
private:
  EventLoop* loop_;

  int timefd_;
  Channel* channel;
  TimerList timers_;
};
#endif
