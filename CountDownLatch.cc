#include "CountDownLatch.h"
#include <assert.h>

void CountDownLatch::wait(){
	MutexGuard guard(lock_);
	while(count_>0)
		NotZero_.wait();
	



}
void CountDownLatch::down_and_notify(){
	MutexGuard guard(lock_);
	assert(count_>0);
	
	--count_;
	if (count_ == 0)
		NotZero_.notify();



}

