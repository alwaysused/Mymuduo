#ifndef COUNTDOWNLATCH_H_
#define COUNTDOWNLATCH_H_
#include "Condition.h"
#include "Mutex.h"

#include <boost/noncopyable.hpp>

class CountDownLatch:boost::noncopyable
{
 public:
 CountDownLatch(int count):lock_(), NotZero_(lock_), count_(count){}
	void wait();
	void down_and_notify();
	


 private:
  MutexLock lock_;
  Condition NotZero_;
  int count_;


};
#endif
