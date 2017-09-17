#include"Epoller.h"
#include<vector>
#include<stdio.h>
#include"Channel.h"
void Epoller::poll(int timeout, std::vector<Channel* >* activeChannels)
{
    int num = epoll_wait(epollfd_, &* events_. begin(), events_.size(), 10000);//
    
    for (int i = 0; i != num; ++i)
	{

    Channel* channel = static_cast<Channel*>(events_[i]. data. ptr);


    channel-> set_revents(events_[i]. events);

    activeChannels->push_back(channel);

	}
}
void Epoller::remove(Channel * chptr)
{
    struct epoll_event ev;
    ev. events = chptr -> events() ;
    epoll_ctl(epollfd_,EPOLL_CTL_DEL,chptr->fd(),&ev);
}
void Epoller::update(Channel* channel)
{
	struct epoll_event ev;
	ev. events = channel -> events() ;
	ev. data. ptr = channel;


	int i;
	if ( (i = channel->index()) == 0)
    {int j = epoll_ctl(epollfd_, EPOLL_CTL_ADD, channel->fd(), &ev);
        
	else if (i == 1)
        {epoll_ctl(epollfd_, EPOLL_CTL_MOD, channel->fd(), &ev);}
}
