#ifndef POLLER_H
#define POLLER_H
#include<vector>
class Channel;

class EventLoop;
class Poller
{
public:
	Poller(EventLoop* loop):looper_(loop){}
	~Poller(){}
	virtual void update(Channel* ) = 0;
    virtual void poll(int timeout, std::vector<Channel* >* activeChannels) = 0;
private:
  EventLoop* looper_;	
	
};
#endif
