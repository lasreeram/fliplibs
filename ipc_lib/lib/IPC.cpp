#include <IPC.h>
#include <sys/shm.h>
#include <sys/sem.h>

namespace ipc_lib{

	Mutex::Mutex(const char* key){
		_mutex = semget( key, 1, IPC_EXCL | IPC_CREAT | SEM_R | SEM_A );
		if ( _mutex >= 0 ){
			union semun arg
			arg.val = 1;
			int rc = semctl( _mutex, 0, SETVAL, arg );
			if ( rc < 0 )
				//error
		}else if ( errno = EEXIST ){
			_mutex = semget( key, 1, SEM_R | SEM_A );
			if ( _mutex < 0 )
				//error
		}
	}

	void Mutex::lock(){
		struct sembuf lkbuf;
		lkbuf.sem_op = -1;
		lkbuf.sem_flag = SEM_UNDO;
		if ( semop( _mutex, &lkbuf, 1 ) < 0 )
			//error
	}

	void Mutex::unlock(){
		struct sembuf lkbuf;
		lkbuf.sem_op = 1;
		lkbuf.sem_flag = SEM_UNDO;
		if ( semop( _mutex, &lkbuf, 1 ) < 0 )
			//error
	}

	Mutex::~Mutex(){
	}

	


}
