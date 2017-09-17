#include "Timestamp.h"
#include "TimeQueue.h"
#include "Timer.h"
#include"EventLoop.h"
#include "Epoller.h"
#include "Channel.h"


#include <stdio.h>
#include <sys/eventfd.h>

int creatEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

      return evtfd;
}
void EventLoop::remove(Channel* chptr)
{
    poller_->remove(chptr);
}
void EventLoop::runAt(double point,Callback cb)
{
  Timer* timer = new Timer(Timestamp::delay(point), cb, 0);
  queue_-> addTimer(timer);
}
void EventLoop::runAfter(double delay, Callback cb){
    runAt(delay, cb);
	}


void EventLoop::runEvery(double delay, Callback cb){
  Timer* timer = new Timer(Timestamp::delay(delay), cb, delay);
  queue_-> addTimer(timer);
	}
EventLoop::~EventLoop(){}
EventLoop::EventLoop():
    eventfd_(creatEventfd()),threadId_(muduo::CurrentThread::tid()),mutex_(),channel_(this,eventfd_),
    poller_(new Epoller(this)), quit_(false), queue_(new TimeQueue(this))
{
    channel_.setReadCallback(boost::bind(&EventLoop::handleRead,this));
    channel_.enableRead();
}

void EventLoop::update(Channel* ch){poller_->update(ch);}

void EventLoop::handleRead()
{
    uint64_t one;
    read(eventfd_,&one, sizeof one);
}

//EventLoop():poller_(this){}

void EventLoop::loop(){
    while(!quit_){
   
	activeChannels_. clear();
    poller_->poll(kWaitTime, &activeChannels_);

	for (ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end(); ++it)
		{
            (*it) -> handleEvent();
       
		}

}

}

void EventLoop::runInLoop(Functor func)
{
    if (isInCreateThread())
        func();
    else
        waitInLoop(func);
}

void EventLoop::waitInLoop(Functor func)
{
    {
        MutexGuard guard(mutex_);
        pendingFunctors. push_back(func);

    }
    if (!isInCreateThread())
        wakeup();
}

void EventLoop::wakeup()
{	uint64_t one = 1;
    write(eventfd_, &one, sizeof one);
}

