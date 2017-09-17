#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <boost/function.hpp>
#include <sys/epoll.h>
class EventLoop;

class Channel
{
public:
	typedef boost::function<void()> Callback;
    Channel(EventLoop*,int );
	~Channel();
	void setReadCallback(Callback cb){readcb_ = cb;}
    void setWriteCallback(Callback cb) {writecb_ = cb;}
    void setCloseCallback(Callback cb){closecb_ = cb;}
	void enableRead();
	void enableWrite();
    void set_revents(int);
	//oid handleRead(){readcb_();}
	void handleEvent();
    int events(){return events_;}
    int fd(){return fd_;}
    int index(){return idx_;}
    void handClose(){closecb_();}
    void disableAll(){events_ &= 0;update();}
    void disableWriting(){events_ &= ~EPOLLOUT;}
    void remove();
    bool isReading(){return events_ && EPOLLIN;}
    bool isWriting(){return events_&& EPOLLOUT;}
 private:
   void update();

   EventLoop* loop_;
   int fd_;
   Callback readcb_;
   Callback writecb_;
   Callback closecb_;
   int idx_;
   int events_;
   int revents_;
	
};
#endif
