#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>
#include <boost/noncopyable.hpp>
class MutexLock:boost::noncopyable
{ 
 public:
  MutexLock(){pthread_mutex_init(&mutex,NULL);}
  ~MutexLock(){pthread_mutex_destroy(&mutex);}
  pthread_mutex_t* GetMutex(){return &mutex;}
 	void lock(){pthread_mutex_lock(&mutex);}
 	void unlock(){pthread_mutex_unlock(&mutex);}
 private:
  pthread_mutex_t mutex;


};

class MutexGuard{
 public:
  MutexGuard(MutexLock& mutex): _lock(mutex){_lock.lock();}
  ~MutexGuard(){_lock.unlock();}
 private:
  MutexLock& _lock;


};
#endif
