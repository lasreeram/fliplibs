#include <IPC.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <debug.h>
#include <errno.h>

namespace ipc_lib{

	Mutex::Mutex(const char* key){
		_key = key;
		debug_lib::log( "attempt creating %s semaphore", _key.c_str() );
		_mutex = semget( key, 1, IPC_EXCL | IPC_CREAT | SEM_R | SEM_A );
		if ( _mutex >= 0 ){
			union semun arg
			arg.val = 1;
			int rc = semctl( _mutex, 0, SETVAL, arg );
			if ( rc < 0 )
				debug_lib::throw_error( "cannot semctl for mutex %s", _key.c_str() );
		}else if ( errno = EEXIST ){
			debug_lib::log( "%s semaphore already exists, try accessing it", _key.c_str() );
			_mutex = semget( key, 1, SEM_R | SEM_A );
			if ( _mutex < 0 )
				debug_lib::throw_error( "cannot semget for mutex %s", _key.c_str() );
		}
	}

	void Mutex::lock(){
		struct sembuf lkbuf;
		debug_lib::log( "attempt locking %s semaphore", _key.c_str() );
		lkbuf.sem_op = -1;
		lkbuf.sem_flag = SEM_UNDO;
		if ( semop( _mutex, &lkbuf, 1 ) < 0 )
			debug_lib::throw_error( "cannot lock (semop) mutex %s", _key.c_str() );
	}

	void Mutex::unlock(){
		debug_lib::log( "attempt un-locking %s semaphore", _key.c_str() );
		struct sembuf lkbuf;
		lkbuf.sem_op = 1;
		lkbuf.sem_flag = SEM_UNDO;
		if ( semop( _mutex, &lkbuf, 1 ) < 0 )
			debug_lib::throw_error( "cannot unlock (semop) mutex %s", _key.c_str() );
	}

	Mutex::~Mutex(){
	}



	SharedMemory::SharedMemory(const char* key, int size){
		_key = key;
		_semid = shmget( key, size, SHM_R | SHM_W | IPC_CREAT );
		if ( _semid < 0 )
			debug_lib::throw_error( "cannot create shared memory %s", _key.c_str() );
	}

	SharedMemory::~SharedMemory(){
	}


	
}
