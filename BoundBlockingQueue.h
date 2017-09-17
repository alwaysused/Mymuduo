#ifndef MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H
#define MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H

#include "Mutex.h"
#include "condition.h"
#include "Thread.h"

#include <boost/circular_buffer.hpp>
//#include <>
template<typename T>
class BoundBlockingQueue
{
 public:
  BoundBlockingQueue(int count): lock_(), notEmpty_(lock_),notFull_(lock_),queue_(count){}
	void put(const T& t)
	{	
		MutexGuard guard(lock_);
		while(queue_. full())
			notFull_. wait();
		queue_.push_back(t);
		notEmpty_.notify();		


	}
	T take()
	{
		MutexGuard guard(lock_);
		while (queue_.empty())
			notEmpty_.wait();
		T front(queue_.front());
		queue_.pop_front();
		notFull_. notify();
		return front;

	}
	int size() const{MutexGuard guard(lock_); return queue_.size();}
 private:
	mutable MutexLock lock_;
  	condition notEmpty_;
	condition notFull_;
	boost::circular_buffer<T> queue_;


};
#endif
