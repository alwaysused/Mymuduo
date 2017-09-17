#include "Singleton.h"
#include "CurrentThread.h"
#include "Thread.h"
#include "ThreadLocal.h"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <unistd.h>

class Test : boost::noncopyable
{
 public:
  Test()
  {
    printf("tid=%d, constructing %p\n", muduo::CurrentThread::tid(), this);
  }

  ~Test()
  {
    printf("tid=%d, destructing %p %s\n", muduo::CurrentThread::tid(), this, name_.c_str());
  }

  const muduo::string& name() const { return name_; }
  void setName(const muduo::string& n) { name_ = n; }

 private:
  muduo::string name_;
};

#define STL Singleton<ThreadLocal<Test> >::instance().value()

void print()
{
  printf("tid=%d, %p name=%s\n",
         muduo::CurrentThread::tid(),
         &STL,
         STL.name().c_str());
}

void threadFunc(const char* changeTo)
{
  print();
  STL.setName(changeTo);
  sleep(1);
  print();
}

int main()
{
  STL.setName("main one");//创建thloc<test>:.key,test*
  muduo::Thread t1(boost::bind(threadFunc, "thread1"));
  muduo::Thread t2(boost::bind(threadFunc, "thread2"));
  t1.start();//test*
  t2.start();//test*
  t1.join();
  print();
  t2.join();
  pthread_exit(0);
}

