#include "BlockingQueue.h"
#include "CountDownLatch.h"
#include "Thread.h"
#include "Timestamp.h"
#include "CurrentThread.h"

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <map>
#include <string>
#include <stdio.h>
#include <algorithm>
class Bench
{
 public:
  Bench(int numThreads)
    : latch_(numThreads),
      threads_(numThreads)
  {
    for (int i = 0; i < numThreads; ++i)
    {
      char name[32];
      snprintf(name, sizeof name, "work thread %d", i);
      threads_.push_back(new muduo::Thread(
            boost::bind(&Bench::threadFunc, this), muduo::string(name)));
    }
    for_each(threads_.begin(), threads_.end(), boost::bind(&muduo::Thread::start, _1));
  }

	void run(int times)
  {
    printf("waiting for count down latch\n");
    latch_.wait();
    printf("all threads started\n");
    for (int i = 0; i < times; ++i)
    {
      Timestamp now(Timestamp::now());
      queue_.put(now);
      usleep(1000);
    }
  }

  void joinAll()
  {
    for (size_t i = 0; i < threads_.size(); ++i)
    {
      queue_.put(Timestamp::invalid());
    }

    for_each(threads_.begin(), threads_.end(), boost::bind(&muduo::Thread::join, _1));
  }

	
	void threadFunc()
	{
		printf("tid = %d, %s started\n",muduo::CurrentThread::tid(),muduo::CurrentThread::name());
		latch_.down_and_notify();
		std::map<int,int> delays;
		bool running = true;
		while(running)
		{
		Timestamp t(queue_.take());
     Timestamp now(Timestamp::now());
      if (t.valid())
      {
        int delay = static_cast<int>(timeDifference(now, t) * 1000000);
        //printf("tid=%d, latency = %d us\n",
               //muduo::CurrentThread::tid(), delay);
        ++delays[delay];
      }
      running = t.valid();

    }printf("tid=%d, %s stopped\n",
           muduo::CurrentThread::tid(),
           muduo::CurrentThread::name());
    for (std::map<int, int>::iterator it = delays.begin();
        it != delays.end(); ++it)
    {
      printf("tid = %d, delay = %d, count = %d\n",
             muduo::CurrentThread::tid(),
             it->first, it->second);
    }
  }

	
 private:
	CountDownLatch latch_;
	boost::ptr_vector<muduo::Thread> threads_;
	BlockingQueue<Timestamp> queue_;
 };
int main(int argc, char* argv[])
{
  int threads = argc > 1 ? atoi(argv[1]) : 1;

  Bench t(threads);
  t.run(10000);
  t.joinAll();
}



