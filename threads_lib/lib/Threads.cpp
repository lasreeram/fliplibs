#include <Threads.h>
#include <debug.h>

namespace pthreads_lib{

	Mutex::Mutex(const pthread_mutexattr_t* attr){
		int ret = pthread_mutex_init(&_mutex, attr);
		if ( ret != 0 )
			debug_lib::throw_error( "cannot initialize mutex %d", ret );
	}

	pthread_mutex_t Mutex::getMyId(){
		return _mutex;
	}

	void Mutex::lock(){
		int ret = pthread_mutex_lock(&_mutex);
		if ( ret != 0 )
			debug_lib::throw_fatal_error( "cannot lock mutex %d", ret );
		_locked = true;
	}

	void Mutex::unlock(){
		if( !_locked )
			debug_lib::throw_error( "logic error. mutex not locked" );
		int ret = pthread_mutex_unlock(&_mutex);
		_locked = false;
		if ( ret != 0 ){
			debug_lib::throw_fatal_error( "cannot unlock mutex %d", ret );
		}
	}

	Mutex::~Mutex(){
		pthread_mutex_destroy(&_mutex);
	}


	//Threads
	Thread::Thread(thread_function functor, pthread_attr_t* attr, void* arg){
		int ret = pthread_create( &_thread_id, attr, functor, arg);
		if ( ret != 0 )
			debug_lib::throw_fatal_error( "cannot create thread %d", ret );
	}

	Thread::Thread(thread_function functor, void* arg){
		int ret = pthread_create( &_thread_id, NULL, functor, arg);
		if ( ret != 0 )
			debug_lib::throw_fatal_error( "cannot create thread %d", ret );

		debug_lib::log( "thread id = %p created", _thread_id );
	}

	Thread::Thread(pthread_t id){ _thread_id = id; }

	int Thread::join(pthread_t id, void ** retval){
		int ret = pthread_join( id, retval );
		if ( ret != 0 ){
			debug_lib::throw_error( "cannot join thread id = %d, reason = %d", id, ret );
		}
		return ret;
	}

	pthread_t Thread::getMyId(){
		return _thread_id;	
	}

	bool Thread::isMe(pthread_t id){
		int ret = pthread_equal(id, _thread_id);	
		if ( ret == 0 )
			return false;
		return true;
	}

	int Thread::yield(){
		int ret = pthread_yield();
		if ( ret != 0 ){
			debug_lib::throw_error( "yield returned an error %d", ret );
		}
		return ret;
	}	

	void Thread::setName(const char* name){
		//debug_lib::log( "thread::setName set id = %ld to name %s - char\n", getMyId(), name );
		debug_lib::init( getMyId(), name );
		_name = name;
	}

	std::string Thread::getName(){
		return _name;
	}

	void Thread::setName(const std::string& name){
		pthread_t tid = getMyId();
		//debug_lib::log( "thread::setName set id = %ld to name %s - string\n", getMyId(), name.c_str() );
		debug_lib::init( tid, name.c_str() );
		_name = name;
	}

	Thread::~Thread(){
	}


	//Condition
	Condition::Condition(Mutex* mutex, pthread_condattr_t* attr){
		int ret = pthread_cond_init( &_cond, attr );
		if ( ret != 0 )
			debug_lib::throw_error( "cannot initialize condition" );
		_mutex = mutex;
	}

	int Condition::wait(){
		pthread_mutex_t mu = _mutex->getMyId();
		int ret = pthread_cond_wait( &_cond, &mu );
		return ret;
	}

	int Condition::timedwait(struct timespec* wait_time){
		pthread_mutex_t mu = _mutex->getMyId();
		int ret = pthread_cond_timedwait( &_cond, &mu, wait_time );
		return ret;
	}

	int Condition::signal(){
		int ret = pthread_cond_signal( &_cond );
		return ret;
	}

	int Condition::broadcast(){
		int ret = pthread_cond_broadcast( &_cond );
		return ret;
	}
}
