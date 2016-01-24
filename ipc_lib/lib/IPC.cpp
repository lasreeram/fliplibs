#include <IPC.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <debug.h>
#include <errno.h>

namespace ipc_lib{

	Mutex::Mutex(const char* key, int num){
		_key = key;
		debug_lib::log( "mutex: attempt creating semaphore for key %s, set size = %d", _key.c_str(), num );
		if( num <= 0 )
			debug_lib::throw_error( "mutex: invalid value for set size %d", num );
		//if you use both IPC_EXCL and IPC_CREAT the call fails with EEXIST if the semaphore already exists.
		//SEM_R, SEM_A are for read and access respectively
		_mutex = semget( _key.c_str(), num, IPC_EXCL | IPC_CREAT | SEM_R | SEM_A );
		if ( _mutex >= 0 ){
			union semun arg
			arg.val = 1;
			arg.array = (short*) malloc ( sizeof(short) * num );
			for( int ix = 0; ix < num; ix++ )
				arg.array[ix] = 1;
			//initialize all semaphores to 1. When the semaphore value goes negative it gets locked.
			//set the 0th semaphore in the set to the value 1. 
			//for set all argument semnum is ignored. So it is passed as 0
			int rc = semctl( _mutex, 0, SETALL, arg );
			free(arg.array);
			if ( rc < 0 )
				debug_lib::throw_error( "cannot semctl for mutex %s", _key.c_str() );
			debug_lib::log( "mutex: semaphore %s created successfully for size %d and ready for access", 
					num, _key.c_str() );
		}else if ( errno = EEXIST ){
			debug_lib::log( "%s semaphore already exists, try accessing it", _key.c_str() );
			//second argument is 0 - don't care if the semaphore already exists
			_mutex = semget( key, 0, SEM_R | SEM_A );
			if ( _mutex < 0 )
				debug_lib::throw_error( "cannot semget for mutex %s", _key.c_str() );
			debug_lib::log( "mutex: semaphore %s ready for access. set size = %d", _key.c_str(), num );
		}
		_locks_held = 0;
	}

	void Mutex::lock(int num){
		struct sembuf lockbuf;
		debug_lib::log( "attempt locking %s semaphore for index %d", _key.c_str(), num );
		//unlock is achieved by decrementing the value by 1.
		lockbuf.sem_op = -1;
		//SEM_UNDO has the effect of releasing the semaphore if the process exits.
		//this is available only in System V and not in POSIX?
		lockbuf.sem_flag = SEM_UNDO;
		if ( semop( _mutex, &lockbuf, num) < 0 )
			debug_lib::throw_error( "cannot lock (semop) mutex %s, for index %d", _key.c_str(), num );

		_locks_held++;
	}

	void Mutex::unlock(int num){
		debug_lib::log( "attempt un-locking %s semaphore for index %d", _key.c_str(), num );
		struct sembuf lockbuf;
		//unlock is achieved by incrementing the value by 1.
		lockbuf.sem_op = 1;
		//SEM_UNDO has the effect of releasing the semaphore if the process exits.
		lockbuf.sem_flag = SEM_UNDO;
		if ( semop( _mutex, &lockbuf, numIndex ) < 0 )
			debug_lib::throw_error( "cannot unlock (semop) mutex %s for index %d", _key.c_str(), num );

		_locks_held--;
	}

	Mutex::~Mutex(){
		if ( _locks_held > 0 ){

			//
			//Stack Unwinding: when an exception is thrown and control passes from a try block to a handler, C++ run time 
			//calls the destructors for all automatic objects constructed since the beginning of try block. 
			//This process is called stack unwinding. The automatic objects are destroyed in the reverse order of their 
			//construction
			//
			if ( !std::uncaught_exception() )
			//returns true if an exception has been already thrown and not caught by its appropriate catch handler.
			//including stack unwinding.
				debug_lib::throw_fatal( "mutex:logic error. atleast one lock is not released for key %s", key.c_str() );
			else{
				debug_lib::log( "mutex: logic error. atleast one lock is not released for key %s", key.c_str() );
				//must allow the current exception to be caught.
			}
		}
	}



	SharedMemory::SharedMemory(const char* key, int blockSize, int numBlocks, ShmInitializer* initObj){
		_key = key;
		if ( (blockSize % 8) == 0 )
			_blockSize = blockSize;
		else
			_blockSize = blockSize + (8 - (blockSize % 8));
		if ( (numBlocks % 8) == 0 )
			_numBlocks = numBlocks;
		else
			_numBlocks = numBlocks + (8 - (numBlocks % 8));
		_createSize = (_blockSize * _numBlocks) + (sizeof(int) * _numBlocks) + sizeof(int);
		_indexTable = _shmAddress + (_blockSize * _numBlocks);
		_free_list_head = _indexTable + (sizeof(int) * _numBlocks);
		attach();
		init();
		
		initObj->initializer( _shmAddress, _numBlocks, _blockSize );
		delete initObj;
	}

	SharedMemory::~SharedMemory(){
	}

	void SharedMemory::attach(){
		//create size is space for buffers, space for links between blocks, free_list_head
		_semid = shmget( _key.c_str(), _createSize, SHM_R | SHM_W | IPC_CREAT );
		if ( _semid < 0 )
			debug_lib::throw_error( "cannot create shared memory for key %s", _key.c_str() );
		
		_shmAddress = shmat( _semid, NULL, 0 );	
		if ( _shmAddress == (void*) -1 )
			debug_lib::throw_error( "attach to shared memory for key %s failed", _key.c_str() );
	}

	void SharedMemory::init(){
		for(int i = 0; i < (_numBlocks - 1); i++ )
			_indexTable[i] = i+1;
		_indexTable[_numBlocks-1] = -1;
		*_free_list_head = 0;
	}

	void* SharedMemory::allocBlock(int& index){
		void* bp;
		index = -1;
		if ( *_free_list_head < 0 ){
			debug_lib::throw_error( "out of shared memory buffers for key %s", _key.c_str() );
		}
		index = *_free_list_head;
		if ( checkIndexForAccess(index) )
			debug_lib::throw_fatal( "shared memory corrupt for key %s", _key.c_str() );
		bp = _shmAddress + index;
		int next = _indexTable[index];
		if ( checkIndex(next) )
			debug_lib::throw_fatal( "shared memory corrupt for key %s", _key.c_str() );
		*_free_list_head = next;
		_indexTable[index] = -1;
		return bp;
	}

	bool SharedMemory::checkIndex(int index){
		if ( index == -1 )
			return true;
		return checkIndexForAccess(index);
	}

	bool SharedMemory::checkIndexForAccess(int index){
		if ( index < 0 || index > (_numBlocks-1) ){
			dumpMemory();
			return false;
		}
		return true;
	}

	void SharedMemory::freeBlock(void* bp){
		int index = (bp - _shmAddress)/_blockSize;
		if ( !checkIndexForAccess(index) )
			debug_lib::throw_fatal( "cannot free block, invalid address" );
		freeBlock(index);
	}

	void SharedMemory::freeBlock(int index){
		if ( _indexTable[index] != -1 ){
			dump_memory();
			debug_lib::throw_fatal( "cannot free block, invalid address" );
		}
			
		_indexTable[index] = *_free_list_head;
		*_free_list_head = index;
	}

	void SharedMemory::dumpMemory(){
		debug_lib::log ( "free list head = %d", *_free_list_head );
		for( int i = 0; i < _numBlocks; i++ )
			debug_lib::log_no_newline( "%d->%d,", i, _indexTable[i] );
		debug_lib::log_no_newline("\n");
	}

	int SharedMemory::getIndexForBlock(void* addr){
		int index = (addr - _shmAddress)/_blockSize;
		return index;
	}

	void* SharedMemory::getAddressForIndex(int index){
		void* ptr = ( _shmAddress + (index * _blockSize) );
		if ( !isValidAddress(ptr) )
			debug_lib::log( "getAddress for index %d: shared memory is corrupt", index );
		return ptr;
	}

	bool SharedMemory::isValidAddress(void* ptr){
		void* startAddr = getAddressForIndex(0);
		void* endAddr = startAddr + _createSize;

		if ( ptr >= startAddr && ptr <= endAddr )
			return true;

		return false;
	}

	int SharedMemory::isEndOfMemory(void* ptr){
		if ( !isValidAddress( ptr ) && isValidAddress( ptr-1 ) )
			return 1;

		if ( !isValidAddress( ptr ) && !isValidAddress( ptr-1 ) )
			return -1;

		return 0;
	}

	ShmIPC::ShmIPC(){
		//for keeping messages. maximum msg size is 1k
		_msgQShm = new SharedMemory("msgq", 1024, 1000, new DataInitializer() );

		//mailbox name to id mapping
		_mailboxIdShm = new SharedMemory( "mboxid", 64, 100, new MailboxIdInitializer() );

		//mailbox que by id. the number of blocks here should be equal to mailboxIdShm
		_mailboxQShm = new SharedMemory( "mboxq", 1024, 100, new MailboxQInitializer() );

		//meta data for shared memory. For example keeping running counter for mbid
		_mailboxMetaData = new SharedMemory( "mboxmeta", 1024, 1, new MailboxMetaInitializer() );
		int* _mb_metadata = (int*) _mailboxMetaData.getAddressForIndex(0);
		int* _mbId_counter = _mb_metadata;
        }

	int ShmIPC::createMailBox(const char* name){
		int idIndex;
		idIndex = locateMailbox(name);
		if( idIndex >= 0 )
			return idIndex;
		AutoMutex(&_global_mutex, 0);
		char* ptr = (char*) _mailboxIdShm.allocBlock(&idIndex);
		if ( strlen(name) > (_mailboxIdShm.getBlockSize()-1) )
			debug_lib::throw_fatal( "name %s in createMailbox is too long", name );
		strcpy( ptr, name );
		return idIndex;
	}

	int ShmIPC::locateMailBox(const char* name){
		char* ptr = (char*) _mailboxIdShm.getAddressForIndex(0);
		for( int i = 0; i < _mailboxIdShm.getNumBlocks(); i++ ){
			if ( strcmp( ptr, name ) == 0 ){
				return i;
			}
			ptr = ptr + _mailboxIdShm.getBlockSize();
		}
		return -1;
	}
	
        int ShmIPC::send(char* mbname, void* ptr, int len){
		int mbid = locateMailBox(mbname);
		if ( mbid < 0 ){
			debug_lib::log( "cannot locate destination mailbox %s\n", mbname );
			return -1;
		}
		return send( mbid, ptr, len );
	}

        int ShmIPC::send(int mbid, void* ptr, int len){

		if ( len > (_msgQshm.getBlockSize() - sizeof(int) ) )
			debug_lib::throw_error( "message length cannot be more than size = %d", (_msgQShm.getBlockSize() - sizeof(int)) );
		int index;
		AutoMutex gMutex(&_global_mutex, 0);
		char* msgbuf = (char*)_msgQShm.allocBlock(&index);
		gMutex.release();
		memcpy( msgbuf, &len, sizeof(int) );
		memcpy( mbsgbuf+sizeof(int), ptr, len );

		AutoMutex mutex(&_record_mutex, index);
		int* mbIdQ = (int*)_mailboxQShm.getAddressForIndex(mbid);
		while ( *mbIdQ > 0 ){
			mbIdQ += sizeof(int);
			int endOfMemory = _mailboxQShm.isEndOfMemory(mbIdQ);
			if ( endOfMemory == 1 )
				debug_lib::throw_fatal( "too many messages in the mailbox queue %d", mbid);
			else if ( endOfMemory == -1 )
				debug_lib::throw_fatal( "mailbox memory is corrupt - %d", mbid);
		}
		*mbIdQ = index;
		return 0;
        }

        int ShmIPC::recv(char* mbname, void* ptr, int size){
		int mbid = locateMailBox(mbname);
		if ( mbid < 0 ){
			debug_lib::log( "cannot locate destination mailbox %s\n", mbname );
			return -1;
		}
		return recv(mbid, ptr, size );
        }

        int ShmIPC::recv_no_wait(char* mbname, void* ptr, int size){
		int mbid = locateMailBox(mbname);
		if ( mbid < 0 ){
			debug_lib::log( "cannot locate destination mailbox %s\n", mbname );
			return -1;
		}
		return recv_no_wait(mbid, ptr, size );
	}

        int ShmIPC::recv_no_wait(int mbid, void* ptr, int size){
		int* mbIdQ;
		mbIdQ = (int*)_mailboxQShm.getAddressForIndex(mbid);
		if( *mbIdQ == -1 )
			return 0; //no data
		else if ( *mbIdQ < 0 )
			debug_lib:throw_error("invalid value in queue %d", *mbIdQ );
			
		return recv_priv( mbIdQ, ptr, size );
	}

        int ShmIPC::recv(int mbid, void* ptr, int size){
		int* mbIdQ;
		while( true ){
			mbIdQ = (int*)_mailboxQShm.getAddressForIndex(mbid);
			if ( *mbIdQ >= 0 )
				break;
		}
		return recv_priv( mbIdQ, ptr, size );
	}


        int ShmIPC::recv_priv(int* mbIdQ, void* ptr, int size){
		AutoMutex(&_record_mutex, mbid);
		char* msgbuf = _msgQShm.getAddressForIndex(*mbIdQ);
		int len;
		memcpy(&len, msgbuf, sizeof(int) );
		if ( len > (_msgQshm.getBlockSize() - sizeof(int)) )
			debug_lib::throw_error( "msg recv size greater than %d", _msgQShm.getBlockSize() - sizeof(int) );
		if( len > size )
			debug_lib::throw_error( "msg recv len %d greater than buffer size %d", len, size );
		memcpy( ptr, msgbuf+sizeof(int), len );

		int* mbIdQPrev = NULL;
		while ( *mbIdQ >= 0 ){
			mbIdQPrev = mbIdQ;
			mbIdQ += sizeof(int);
			if ( _mailboxQShm.isEndOfMemory(mbIdQ) )
				debug_lib::throw_fatal( "too many messages in the mailbox queue %s", mbname );
			*mbIdQPrev = *mbIdQ;
		}
		return len;
	}

	ShmIPC::~ShmIPC(){
		delete _msgQShm;
		delete _mailboxQShm;
		delete _mailboxIdShm;
		delete _mailboxMetaData;
	}
}
