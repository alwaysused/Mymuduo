#ifndef BLOCKINGQUEUE_H_
#define BLOCKINGQUEUE_H_

#include "Mutex.h"
#include "condition.h"
//#include "CountDownLatch.h"

#include <deque>
#include <utility>
template<typename T>
class BlockingQueue{
 public:
  BlockingQueue():lock_(),cond(lock_), queue_(){}
  void put(const T& t)
  {
  	MutexGuard guard(lock_);
  	queue_. push_back(t);
  	cond. notify();
  
  }
  T take()
  {
  	MutexGuard guard(lock_);
  	while(queue_.empty() )
  		cond. wait();
  	T front(std::move(queue_. front() ));
  	queue_. pop_front();
  	return front;	
  
  
  }
  int size() const
	{
		MutexGuard guard(lock_);
		return queue_. size();
	}



 private:
	mutable MutexLock lock_;
	condition cond; 
  std::deque<T> queue_;

};
#endif
