#include "Thread.h"
#include "CurrentThread.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/weak_ptr.hpp>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>
namespace muduo{
namespace Detail
{
	pid_t GetTid(){return static_cast<pid_t>(::syscall(SYS_gettid));}
	
	
	
	
	
	struct ThreadData
	{
		typedef muduo::Thread::ThreadFunc ThreadFunc;//typedef namespace is refered to file not namespace here
  	ThreadFunc func_;
  	string name_;
  	boost::weak_ptr<pid_t> wkTid_;

  	ThreadData(const ThreadFunc& func,
             const string& name,
             const boost::shared_ptr<pid_t>& tid)
    : func_(func),
      name_(name),
      wkTid_(tid)
  { }
  void RunInThread()
  {
  	pid_t tid = muduo::CurrentThread::tid();
  	
  	boost::shared_ptr<pid_t> ptid = wkTid_.lock();
  	if (ptid){
  		*ptid = tid;
  		ptid.reset();
  	
  	}
  	muduo::CurrentThread::t_threadName = name_.empty() ? "muduoThread" : name_.c_str();
  	func_();
  
  
  }
	};
	void* StartThread(void* p)
	{
		ThreadData* data = static_cast<ThreadData*> (p);
		data->RunInThread();
		delete data;
		return NULL; 
	
	}

}
namespace CurrentThread
{
	__thread int t_cachedTid = 0;
  __thread char t_tidString[32];
  __thread int t_tidStringLength = 6;
  __thread const char* t_threadName = "unknown";
  const bool sameType = boost::is_same<int, pid_t>::value;
  BOOST_STATIC_ASSERT(sameType);
  
  void cacheTid()
  {
  	if (t_cachedTid == 0)
  {
    t_cachedTid = muduo::Detail::GetTid();
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ",																t_cachedTid);
  }
  
  }
  
  
}



}
using namespace muduo;
AtomicInt32 Thread:: numCreated_;
void Thread::setDefaultName()
{
	int num = numCreated_. incrementAndGet();
	if (name_.empty())
	{
		char buff[32];
		snprintf(buff, sizeof buff, "thread %d", num);
		name_ = buff;	
	}

}
Thread::Thread(const ThreadFunc& func, const string&  n):
started_(false),joined_(false),PthreadId_(0),tid_(new pid_t(0)),
																							func_(func),
																							name_(n){setDefaultName();}
void Thread::start(){
	started_ = true;
	Detail::ThreadData* data = new Detail::ThreadData(func_, name_, tid_);
	if (pthread_create(&PthreadId_, NULL, &Detail::StartThread, data))
	{
		started_ = false;
		delete data;
	}

}
void Thread::join(){
	assert(started_);
	joined_ = true; 
	pthread_join(PthreadId_, NULL);


}
Thread::~Thread()
{
	if(started_&&!joined_)
		pthread_detach(PthreadId_);
}




