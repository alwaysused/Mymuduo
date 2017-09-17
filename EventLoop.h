#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include"Channel.h"
#include"CurrentThread.h"
#include"Mutex.h"
#include <vector>

#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include<boost/bind.hpp>
#include<sys/syscall.h>
#include <assert.h>


class TimeQueue;
class Epoller;
class Channel;
class Poller;
struct epoll_event;
class EventLoop
{
public:
	
    typedef boost::function<void()> Callback;
    typedef std::vector<Channel*> ChannelList;
    typedef boost::function<void()> Functor;
    typedef std::vector<Functor> PendingFunc;

	void runAt(double ,Callback);
	void runAfter(double, Callback);
	void runEvery(double, Callback);
	void update(Channel* );
    void quit(){quit_ = true;}
    void loop();
    bool isInCreateThread(){return muduo::CurrentThread::tid() == threadId_;}
    void assertInLoopThread(){assert(muduo::CurrentThread::tid() == threadId_);}
    void wakeup();
    void handleRead();
    void runInLoop(Functor  func);
    void waitInLoop(Functor func);
    void remove(Channel*);
	EventLoop();
	~EventLoop();
	private:
	 static const int kWaitTime = 10000;
     int eventfd_;
     const pid_t threadId_;
     MutexLock mutex_;
     Channel channel_;
     boost::scoped_ptr<Epoller> poller_;
     ChannelList activeChannels_;
     boost::scoped_ptr<TimeQueue> queue_;
     bool quit_;
     PendingFunc pendingFunctors;

};
#endif
