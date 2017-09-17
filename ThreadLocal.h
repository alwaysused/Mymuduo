#include "Singleton.h"

template <typename T>
class ThreadLocal
{
public:
	ThreadLocal(){pthread_key_create(&key_, destroy);}
	~ThreadLocal(){pthread_key_delete(key_);}
	T& value()
	{
	  T* perThreadValue = static_cast<T*> (pthread_getspecific(key_));
	  if(!perThreadValue)
	  {
	  	perThreadValue = new T();
	  	pthread_setspecific(key_, perThreadValue);
	  	return *perThreadValue;
	  }
	  return *perThreadValue;
	}
	
 private:
   static void destroy(void* p)
   {
   	 T* data = static_cast<T*>(p);
   	 delete data;
   }
   pthread_key_t key_;
};
