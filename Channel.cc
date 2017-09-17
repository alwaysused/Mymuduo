#include "Channel.h"
#include<sys/epoll.h>
#include <stdio.h>
#include"EventLoop.h"
const int NewFd = 0;
const int OldFD = 1;


const int ReadEvent = EPOLLIN|EPOLLPRI;
Channel::Channel(EventLoop* loop, int fd):loop_(loop),fd_(fd), idx_(NewFd),
				events_(0), revents_(0){}
Channel::~Channel(){}
void Channel::enableRead()
{
  events_ |= ReadEvent;
  update();
}
void Channel::enableWrite()
{
    events_ |= EPOLLOUT;
    update();
}

void Channel::remove()
{disableAll();loop_->remove(this);}
void Channel::set_revents(int rev)
{
  revents_ = rev;
}

void Channel::handleEvent()
{
    printf("time time\n");
	if (revents_ & EPOLLIN)
        readcb_();
	 if (revents_ & EPOLLOUT)
		writecb_();

}
void Channel::update(){loop_->update(this);}
