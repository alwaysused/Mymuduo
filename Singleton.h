#ifndef SINGLETON_T_
#define SINGLETON_T_
#include <pthread.h>


template <typename T>
class Singleton
{
 public:
	static T& instance()
	{
		pthread_once(&ponce_, init);
		return *value_;
	}

	

 private:
	static void init()
	{
		value_ = new T();
	}
	static pthread_once_t ponce_;
	static T* value_;

};
template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;
template<typename T>
T* Singleton<T>::value_ = NULL;
#endif
