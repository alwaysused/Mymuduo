#ifndef THREAD_H_
#define THREAD_H_
#include "Atomic.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>
#include <string>
namespace muduo{
using std::string;
 

class Thread{ 
 public:
  typedef boost::function<void()> ThreadFunc;
  Thread(const ThreadFunc& func, const string& name = string());
																							
	~Thread();												
  void start();


	static int numCreated(){return numCreated_.get();}
	void join();
 private:
	void setDefaultName();
  bool started_;
  bool joined_;
  pthread_t PthreadId_;
  boost::shared_ptr<pid_t> tid_;
  ThreadFunc func_;
  string name_;

 static AtomicInt32 numCreated_;
};
}

#endif
