#ifndef CONDITION_H_
#define CONDITION_H_
#include "Mutex.h"

#include <boost/noncopyable.hpp>
#include <pthread.h>
class Condition:boost::noncopyable
{
 public:
 	Condition(MutexLock& lock):lock_(lock)
 														{pthread_cond_init(&cond_,NULL);}
    ~Condition(){pthread_cond_destroy(&cond_);}
 	void wait()	
 	{
 		//MutexGuard guard(lock_);
 		pthread_cond_wait(&cond_, lock_.GetMutex());}
 	void notify() 
 	{
 		//MutexGuard guard(lock_);
 		pthread_cond_signal(&cond_);}
 private:
	MutexLock& lock_;
	pthread_cond_t cond_; 


};
#endif
