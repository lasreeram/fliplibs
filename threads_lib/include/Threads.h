#ifndef _MY_PTHREADS_LIB_
#define _MY_PTHREADS_LIB_
#include <pthread.h>
#include <time.h>
#include <string>

namespace pthreads_lib{

	typedef void *(*thread_function) (void*);
	class Thread{
		public:
			Thread( thread_function, pthread_attr_t* attributes, void* arg );
			Thread( thread_function, void* arg );
			Thread( pthread_t thread_id );
			virtual ~Thread();
			int join( pthread_t thread_id, void** retval );
			pthread_t getMyId();
			bool isMe(pthread_t id);
			int yield();
			void setName(const char* name);
			void setName(const std::string& name);
			std::string getName();
	
		private:
			pthread_t _thread_id;
			std::string _name;
	};

	class Mutex{
		public:
			Mutex(const pthread_mutexattr_t* attr);
			void lock();
			void unlock();
			pthread_mutex_t getMyId();
			~Mutex();

		private:
			pthread_mutex_t _mutex;
			bool _locked;
	};

	class Condition{
		public:
			Condition(Mutex* mutex, pthread_condattr_t* attr);
			~Condition();
			int wait();
			int timedwait(struct timespec* t);
			int signal();
			int broadcast();

		private:
			pthread_cond_t _cond;
			Mutex* _mutex;
	};

}
#endif
