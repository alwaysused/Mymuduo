#ifndef EPOLLER_H
#define EPOLLER_H
#include<vector>
#include<sys/epoll.h>
#include "Poller.h"
class Channel;
class Epoller:public Poller
{
public:
    typedef std::vector<struct epoll_event> EventList;
	Epoller(EventLoop* loop):Poller(loop),epollfd_(epoll_create(EPOLL_CLOEXEC)),
							 events_(kSize){}
	~Epoller(){}
	void update(Channel*);
    void poll(int , std::vector<Channel*>* );
    void remove(Channel*);
private:
  static const int kSize = 16;
  
  int epollfd_;
  EventList events_;
	
};
#endif










