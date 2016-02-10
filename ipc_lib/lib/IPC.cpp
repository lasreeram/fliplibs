#include <IPC.h>
#include <debug.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>

namespace ipc_lib{

	Mutex::Mutex(const char* key, int num){
		_key = key;
		if ( _key.length() != sizeof(int) )
			debug_lib::throw_error( "mutex logic error: length of key %s must be %d", _key.c_str(), sizeof(int) );
		memcpy( &_keyid, _key.c_str(), sizeof(int) );
		debug_lib::log( "mutex: attempt creating semaphore for key %s, set size = %d", _key.c_str(), num );
		if( num <= 0 )
			debug_lib::throw_error( "mutex: invalid value for set size %d", num );
		//if you use both IPC_EXCL and IPC_CREAT the call fails with EEXIST if the semaphore already exists.
		//0660 grants read and write permissions to owner and group with no permissions to any other user.
		_mutex = semget( _keyid, num, IPC_EXCL | IPC_CREAT | 0660 );
		if ( _mutex >= 0 ){
			union semun arg;
			arg.val = 1;
			arg.array = (unsigned short *) malloc ( sizeof(short) * num );
			for( int ix = 0; ix < num; ix++ )
				arg.array[ix] = 1;
			//initialize all semaphores to 1. When the semaphore value goes negative it gets locked.
			//for set all argument semnum is ignored. So it is passed as 0
			int rc = semctl( _mutex, 0, SETALL, arg );
			free(arg.array);
			if ( rc < 0 )
				debug_lib::throw_error( "cannot semctl for mutex %s", _key.c_str() );
			debug_lib::log( "mutex: semaphore %s created successfully, id = %d, for size %d and ready for access", 
					_key.c_str(), _mutex, num );
		}else if ( errno == EEXIST ){
			debug_lib::log( "mutex: %s semaphore already exists, try accessing it", _key.c_str() );
			//second argument is 0 - don't care if the semaphore already exists
			_mutex = semget( _keyid, 0, 0660 );
			if ( _mutex < 0 )
				debug_lib::throw_error( "cannot semget for mutex %s", _key.c_str() );
			debug_lib::log( "mutex: semaphore %s ready for access. id = %d, set size = %d", _key.c_str(), _mutex, num );
		}
		_locks_held = 0;
	}

	void Mutex::lock(int num){
		struct sembuf lockbuf;
		debug_lib::log( "attempt locking %s semaphore for index %d", _key.c_str(), num );
		//lock is achieved by decrementing the value by 1. 
		//another call to lock attempts to make the value negative thereby locking the semaphore.
		lockbuf.sem_op = -1;
		lockbuf.sem_num = num;
		//SEM_UNDO has the effect of releasing the semaphore if the process exits.
		//this is available only in System V and not in POSIX?
		lockbuf.sem_flg = SEM_UNDO;
		if ( semop( _mutex, &lockbuf, 1) < 0 )
			debug_lib::throw_error( "cannot lock (semop) mutex %s, for index %d", _key.c_str(), num );

		_locks_held++;
	}

	void Mutex::unlock(int num){
		debug_lib::log( "attempt un-locking %s semaphore for index %d", _key.c_str(), num );
		struct sembuf lockbuf;
		//unlock is achieved by incrementing the value by 1.
		lockbuf.sem_op = 1;
		lockbuf.sem_num = num;
		//SEM_UNDO has the effect of releasing the semaphore if the process exits.
		lockbuf.sem_flg = SEM_UNDO;
		if ( semop( _mutex, &lockbuf, 1) < 0 )
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
				debug_lib::throw_fatal_error( "mutex:logic error. atleast one lock is not released for key %s", _key.c_str() );
			else{
				debug_lib::log( "mutex: logic error. atleast one lock is not released for key %s", _key.c_str() );
				//must allow the current exception to be caught.
			}
		}
	}



	SharedMemory::SharedMemory(const char* key, int blockSize, int numBlocks, ShmInitializer* initObj){
		_key = key;
		if ( _key.length() != sizeof(int) )
			debug_lib::throw_fatal_error( "shm logic error: key %s length must be %d ", _key.c_str(), sizeof(int) );
		memcpy( &_keyid, _key.c_str(), sizeof(int) );
		if ( (blockSize % 8) == 0 )
			_blockSize = blockSize;
		else
			_blockSize = blockSize + (8 - (blockSize % 8));
		if ( (numBlocks % 8) == 0 )
			_numBlocks = numBlocks;
		else
			_numBlocks = numBlocks + (8 - (numBlocks % 8));
		_createSize = (_blockSize * _numBlocks) + (sizeof(int) * _numBlocks) + sizeof(int);
		bool need_to_initialize = (attach() == 0);
		int indexTableOffset = (_blockSize * _numBlocks); 
		int freeListHeadOffset = (_blockSize * _numBlocks) + (sizeof(int) * _numBlocks);
		_indexTable = (int*)( (char*)_shmAddress + indexTableOffset );
		_free_list_head = (int*)( (char*)_shmAddress + freeListHeadOffset );

		if ( !need_to_initialize )
			return;
		init();
		initObj->initialize( _shmAddress, _numBlocks, _blockSize );
		delete initObj;
	}

	SharedMemory::~SharedMemory(){
	}

	int SharedMemory::attach(){
		int exists = 0;

		//create size is space for buffers, space for links between blocks, free_list_head
		_semid = shmget( _keyid, _createSize, SHM_R | SHM_W | IPC_CREAT | IPC_EXCL );
		if ( errno == EEXIST ){
			_semid = shmget( _keyid, _createSize, SHM_R | SHM_W | IPC_CREAT );
			exists = 1;
		}
			
		if ( _semid < 0 )
			debug_lib::throw_error( "cannot create shared memory for key %s", _key.c_str() );
		
		_shmAddress = shmat( _semid, NULL, 0 );	
		if ( _shmAddress == (void*) -1 )
			debug_lib::throw_error( "attach to shared memory for key %s failed", _key.c_str() );

		debug_lib::log( "successfully attached to shared memory %s, semid=%d, %s", _key.c_str(), _semid, 
				((exists == 0)? "created" : "attached to existing") );
		return exists;
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
			debug_lib::throw_error( "allocBlock: out of shared memory buffers for key %s", _key.c_str() );
		}
		index = *_free_list_head;
		if ( !checkIndexForAccess(index) )
			debug_lib::throw_fatal_error( "allocBlock1: shared memory corrupt for key %s", _key.c_str() );
		bp = getAddressForIndex(index);
		int next = _indexTable[index];
		if ( !checkIndex(next) )
			debug_lib::throw_fatal_error( "allocBlock2: shared memory corrupt for key %s", _key.c_str() );
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
			dumpMemory(NULL);
			return false;
		}
		return true;
	}

	void SharedMemory::freeBlock(void* bp){
		int index = getIndexForAddress(bp);
		freeBlock(index);
	}

	void SharedMemory::freeBlock(int index){
		if ( _indexTable[index] != -1 ){
			dumpMemory(NULL);
			debug_lib::throw_fatal_error( "cannot free block, invalid address %s, index=%d", 	
					_key.c_str(), index );
		}
			
		_indexTable[index] = *_free_list_head;
		*_free_list_head = index;
	}

	void SharedMemory::dumpMemory(ShmDataDumper* applicationDumper){
		debug_lib::log ( "legend: A=Allocated\n" );
		debug_lib::log ( "free list head = %d", *_free_list_head );
		for( int i = 0; i < _numBlocks; i++ ){
			if ( _indexTable[i] == -1 )
				debug_lib::log_no_newline( "%d-->%c,", i, 'A' );
			else
				debug_lib::log_no_newline( "%d-->%d,", i, _indexTable[i] );
		}
		debug_lib::log_no_newline("\n\n");
		if( applicationDumper == NULL )
			return;
		applicationDumper->dump( _shmAddress, _numBlocks, _blockSize );
		delete applicationDumper;
	}

	int SharedMemory::getIndexForAddress(void* addr){
		int diff = ((char*)addr - (char*)_shmAddress);
		if ( ((diff % 8) != 0) || (diff < 0) )
			debug_lib::throw_fatal_error( "getIndexForAddress, invalid address" );
			
		int index = diff/_blockSize;
		if ( !checkIndexForAccess(index) )
			debug_lib::throw_fatal_error( "getIndexForAddress, invalid index" );
		return index;
	}

	void* SharedMemory::getAddressForIndex(int index){
		if ( !checkIndexForAccess(index) )
			debug_lib::throw_fatal_error( "getAddressForIndex, invalid address" );
		
		void* ptr = ( (char*)_shmAddress + (index * _blockSize) );
		if ( !isValidAddress(ptr) )
			debug_lib::log( "getAddress for index %d: shared memory is corrupt", index );
		return ptr;
	}

	bool SharedMemory::isValidAddress(void* ptr){
		int index = getIndexForAddress(ptr);
		if ( checkIndexForAccess(index) )
			return true;
		return false;
	}

	int SharedMemory::isEndOfMemory(void* ptr){
		if ( !isValidAddress( (char*)ptr ) && isValidAddress( (char*)ptr-1 ) )
			return 1;

		if ( !isValidAddress( (char*)ptr ) && !isValidAddress( (char*)ptr-1 ) )
			return -1;

		return 0;
	}

	ShmIPC::ShmIPC(){
		//for keeping messages. maximum msg size is 1k
		//_msgQShm = new SharedMemory("MBMQ", 1024, 100, new DataInitializer() );
		_msgQShm = new SharedMemoryVariableSize( 1024, 8192, 100);

		//mailbox name to id mapping
		_mailboxIdShm = new SharedMemory( "MBID", 64, 100, new MailBoxIdInitializer() );

		//mailbox que by id. the number of blocks here should be equal to mailboxIdShm
		_mailboxQShm = new FixedIndexSharedMemory( "MBIQ", 1024, 100, new IntegerMinusOneInitializer() );

		//meta data for shared memory. For example keeping running counter for mbid
		_mailboxMetaData = new SharedMemory( "MBMD", 1024, 1, new MailBoxMetaInitializer() );

		_record_mutex = new Mutex( "SHMM", 100 );
		_global_mutex = new Mutex( "GLBM", 1 );
        }

	int ShmIPC::createMailBox(const char* name){
		int idIndex;
		idIndex = locateMailBox(name);
		if( idIndex >= 0 )
			return idIndex;
		AutoMutex(_global_mutex, 0);
		char* ptr = (char*) _mailboxIdShm->allocBlock(idIndex);
		if ( strlen(name) > ((unsigned int)(_mailboxIdShm->getBlockSize()-1)) )
			debug_lib::throw_fatal_error( "name %s in createMailbox is too long", name );
		strcpy( ptr, name );
		debug_lib::log( "mailbox created %s at %p index %d", ptr, ptr, idIndex );
		return idIndex;
	}

	void ShmIPC::getMailBoxName(int id, std::string& name){
		char* ptr = (char*) _mailboxIdShm->getAddressForIndex(id);
		if ( !_mailboxIdShm->isValidAddress(ptr) )
			debug_lib::throw_fatal_error( "getmailboxname: shared memory is corrupt" );
		name = ptr;
	}
	
	int ShmIPC::locateMailBox(const char* name){
		char* ptr = (char*) _mailboxIdShm->getAddressForIndex(0);
		for( int i = 0; i < _mailboxIdShm->getNumBlocks(); i++ ){
			if ( *ptr == '\0' )
				continue;
			if ( strcmp( ptr, name ) == 0 ){
				return i;
			}
			ptr = ptr + _mailboxIdShm->getBlockSize();
		}
		return -1;
	}
	
        int ShmIPC::send(const char* mbname, void* ptr, int len){
		debug_lib::log( "send: attempt to send %d bytes", len );
		int mbid = locateMailBox(mbname);
		if ( mbid < 0 ){
			debug_lib::log( "send: cannot locate destination mailbox %s", mbname );
			return -1;
		}
		return send( mbid, ptr, len );
	}

        int ShmIPC::send(int mbid, void* ptr, int len){

		if ( ((unsigned int)len) > (unsigned int)_msgQShm->getMaxMessageSize() )
			debug_lib::throw_error( "message length cannot be more than size = %d", _msgQShm->getMaxMessageSize() );
		int index;
		AutoMutex gMutex(_global_mutex, 0);
		_msgQShm->allocateAndCopy(ptr, len, index);
		gMutex.release();

		AutoMutex mutex(_record_mutex, mbid);
		int* mbIdQ = (int*)_mailboxQShm->getAddressForIndex(mbid);
		while ( *mbIdQ >= 0 ){
			mbIdQ += sizeof(int);
			int endOfMemory = _mailboxQShm->isEndOfMemory(mbIdQ);
			if ( endOfMemory == 1 )
				debug_lib::throw_fatal_error( "too many messages in the mailbox queue %d", mbid);
			else if ( endOfMemory == -1 )
				debug_lib::throw_fatal_error( "mailbox memory is corrupt - %d", mbid);
		}
		*mbIdQ = index;
		return 0;
        }

        int ShmIPC::recv(const char* mbname, void* ptr, int size){
		debug_lib::log( "recv: waiting to recv on %s", mbname );
		int mbid = locateMailBox(mbname);
		if ( mbid < 0 ){
			debug_lib::log( "recv: cannot locate destination mailbox %s", mbname );
			return -1;
		}
		return recv(mbid, ptr, size );
        }

        int ShmIPC::recv_no_wait(const char* mbname, void* ptr, int size){
		int mbid = locateMailBox(mbname);
		if ( mbid < 0 ){
			debug_lib::log( "recv_no_wait: cannot locate destination mailbox %s", mbname );
			return -1;
		}
		return recv_no_wait(mbid, ptr, size );
	}

        int ShmIPC::recv_no_wait(int mbid, void* ptr, int size){
		int* mbIdQ;
		mbIdQ = (int*)_mailboxQShm->getAddressForIndex(mbid);
		if( *mbIdQ == -1 )
			return 0; //no data
		else if ( *mbIdQ < 0 )
			debug_lib::throw_error("invalid value in queue %d", *mbIdQ );
			
		return recv_priv( mbIdQ, mbid, ptr, size );
	}

        int ShmIPC::recv(int mbid, void* ptr, int size){
		int* mbIdQ;
		while( true ){
			mbIdQ = (int*)_mailboxQShm->getAddressForIndex(mbid);
			if ( *mbIdQ >= 0 )
				break;
		}
		return recv_priv( mbIdQ, mbid, ptr, size );
	}


        int ShmIPC::recv_priv(int* mbIdQ, int mbid, void* ptr, int size){
		int len;
		AutoMutex(_record_mutex, mbid);
		_msgQShm->getData(*mbIdQ, ptr, size, len);
		_msgQShm->freeMemory( *mbIdQ );
		
		//shift the queue so that the next message comes to the head of the queue
		int* mbIdQPrev = NULL;
		while ( *mbIdQ >= 0 ){
			mbIdQPrev = mbIdQ;
			mbIdQ += sizeof(int);
			if ( _mailboxQShm->isEndOfMemory(mbIdQ) ){
				std::string name;
				getMailBoxName(mbid, name);
				debug_lib::throw_fatal_error( "too many messages in the mailbox queue %s", name.c_str() );
			}
			*mbIdQPrev = *mbIdQ;
		}
		return len;
	}

	void ShmIPC::dumpMemory(){
		debug_lib::log( "-----------------------------------------dumping message queue-----------------------------------" );
		_msgQShm->dumpMemory(new DummyDumper());
		debug_lib::log_no_newline( "\n" );

		debug_lib::log( "-----------------------------------------dumping id data-----------------------------------------" );
		_mailboxIdShm->dumpMemory(new MailboxIdQDumper());
		debug_lib::log_no_newline( "\n" );

		debug_lib::log( "-----------------------------------------dumping mailbox queue-----------------------------------" );
		_mailboxQShm->dumpMemory(new MailboxQDumper());
		debug_lib::log_no_newline( "\n" );
	}

	ShmIPC::~ShmIPC(){
		delete _msgQShm;
		delete _mailboxQShm;
		delete _mailboxIdShm;
		delete _mailboxMetaData;
		delete _record_mutex;
		delete _global_mutex;
	}


	SharedMemoryVariableSize::SharedMemoryVariableSize(int blockSize, int maxMessageSize, int numMessages ){
		int blockChainSize = (maxMessageSize / blockSize) + 1; //+1 for the terminating -1
		int numBlocks = (maxMessageSize/blockSize) * numMessages;
		_msgQShm = new SharedMemory("SVMQ", blockSize, numBlocks, new DataInitializer() );

		//number of active messages at any time. It is a list of indexes that constitute a message
		//For example if a certain message requires allocation of 3 x 1024 blocks then the block chain contains the list of 3 ids 
		//into the msgQShm memory

		_blockChainShm = new SharedMemory( "SVBC", blockChainSize, numMessages, new IntegerMinusOneInitializer() );
		_msgSizeShm = new FixedIndexSharedMemory( "SVMS", sizeof(int), numMessages, new IntegerZeroInitializer() );
		_blockChainSize = blockChainSize;
		_numMessages = numMessages;
		_maxMessageSize = maxMessageSize;
		debug_lib::log( "shared memory variable created: block size = %d, num blocks = %d, max Message size = %d, num Messages = %d",
				_msgQShm->getBlockSize(), _msgQShm->getNumBlocks(), _maxMessageSize, _numMessages );
	}

	void SharedMemoryVariableSize::dumpMemory(ShmDataDumper* applicationDumper){
		debug_lib::log( "---------------------------------dumping message queue - messages------------------------" );
		_msgQShm->dumpMemory(new DummyDumper());
		debug_lib::log_no_newline( "\n" );

		debug_lib::log( "---------------------------------dumping message queue - block chain---------------------" );
		_blockChainShm->dumpMemory(new BlockChainDumper());
		debug_lib::log_no_newline( "\n" );

		debug_lib::log( "---------------------------------dumping message queue - msg size------------------------" );
		_msgSizeShm->dumpMemory(new MsgSizeDumper());
		debug_lib::log_no_newline( "\n" );
	}

	SharedMemoryVariableSize::~SharedMemoryVariableSize(){
		delete _msgQShm;
		delete _blockChainShm;
		delete _msgSizeShm;
	}

	void SharedMemoryVariableSize::getData ( int id, void* data, int sizeOfData, int& lenCopied ){
		int blockSize = _msgQShm->getBlockSize();
		int* msgSizePtr = (int*)_msgSizeShm->getAddressForIndex(id);
		int remainLen = *msgSizePtr;
		int lenToCopy = 0;
		lenCopied = *msgSizePtr;
		int offset = 0;

		int* blkChainPtr = (int*)_blockChainShm->getAddressForIndex(id);
		while( *blkChainPtr != -1 ){
			char* ptr = (char*)_msgQShm->getAddressForIndex( *blkChainPtr );
			if ( remainLen >= blockSize )
				lenToCopy = blockSize;
			else
				lenToCopy = remainLen;
			memcpy( (((char*)data)+offset), ptr, lenToCopy );
			remainLen -= lenToCopy;
			offset += lenToCopy;
			if ( remainLen < 0 )
				debug_lib::throw_error( "getData: logic error : remaining length is below zero" );
			blkChainPtr++;
		}
		if ( remainLen > 0 )
			debug_lib::throw_error( "getData: logic error : entire data is not copied" );
		debug_lib::log( "get Data - success, length copied = %d", lenCopied );
	}

	void SharedMemoryVariableSize::allocateAndCopy( void* data, int datalen, int& id ){
		if ( (unsigned int)datalen > (unsigned int)getMaxMessageSize() )  
			debug_lib::throw_error( "allocateAndCopy - data exceeds max message size[%d]", getMaxMessageSize() );
		int numBlocks = (datalen / _msgQShm->getBlockSize()) + 1; //for example 100 bytes will require 1 block. 1025 bytes requires 2.
		int* blkChainPtr = (int*)_blockChainShm->allocBlock(id); //return the id to the caller.
		int blockIndex;
		debug_lib::log( "allocate and Copy - copying %d len: ", datalen );
		if ( numBlocks >= 1 ){
			int blockSize = _msgQShm->getBlockSize();
			int remainLen = datalen;
			int lenToCopy;
			int offset = 0;
			int i;
			for(i = 0; i < numBlocks; i++ ){
				_msgQShm->allocBlock(blockIndex);

				//link to block chain
				*(blkChainPtr+i) = blockIndex;

				//copy data
				char* ptr = (char*) _msgQShm->getAddressForIndex(blockIndex);
				if ( remainLen >= blockSize )
					lenToCopy = blockSize;
				else
					lenToCopy = remainLen;
				memcpy( ptr, (char*)data+offset, lenToCopy );
				offset += lenToCopy;
				remainLen -= lenToCopy;
				debug_lib::log_no_newline( "%d:%d,", blockIndex, lenToCopy );
			}
			*(blkChainPtr+i) = -1;
			debug_lib::log_no_newline("\n");
			int* msgSizePtr = (int*)_msgSizeShm->getAddressForIndex(id);
			*msgSizePtr = datalen;
		}else{
			debug_lib::throw_error( "allocateAndCopy - logic error. numBlocks is negative" );
		}
	}

	void SharedMemoryVariableSize::freeMemory( int id ){
		int* blkChainPtr = (int*)_blockChainShm->getAddressForIndex(id);
		debug_lib::log_no_newline( "freeMemory indexes:" );
		while( *blkChainPtr != -1 ){
			_msgQShm->freeBlock( *blkChainPtr );
			debug_lib::log_no_newline( "%d,", *blkChainPtr );
			*blkChainPtr = -1;
			blkChainPtr++;
		}
		debug_lib::log_no_newline("\n");
		_blockChainShm->freeBlock(id);
		int* msgSizePtr = (int*)_msgSizeShm->getAddressForIndex(id);
		*msgSizePtr = 0;
	}

	void FixedIndexSharedMemory::dumpMemory(ShmDataDumper* applicationDumper){
		applicationDumper->dump( _shmAddress, _numBlocks, _blockSize );
		delete applicationDumper;
	}

	//System V IPC Mechanism
	SystemVIPC::SystemVIPC( const char* path ){
		_path = path;
	}

	int SystemVIPC::createMailBox(const char* name){
            int ret = locateMailBox (name );
            if ( ret > 0 )
                    return ret;
            int key = getKey(name);
            int queue_id = msgget( key, IPC_CREAT | 0666 );
            if ( queue_id < 0 ) {
                    debug_lib::throw_fatal_error( "createMailbox: cannot access/create queue:", name );
            }
            return queue_id;
        }

        int SystemVIPC::locateMailBox(const char* name){
            int key = getKey(name);
            int queue_id = msgget( key, 0 );
            return queue_id;
        }

        int SystemVIPC::send(const char* dest, void* ptr, int len){
            int qid = locateMailBox(dest);
            return send( qid, ptr, len );
        }

        int SystemVIPC::send(int dest_id, void* packet, int len){
            if ( (unsigned int)len > sizeof(my_msgbuf)-sizeof(long) )
                    debug_lib::throw_error( "systemv send: logic error, arg 2 to recv is > than max msg size" );
            int rc = msgsnd( dest_id, (struct msgbuf*)packet, len, 0 );
	    debug_lib::log ( "sent to dest_id %d, msgtype = %d", dest_id, ((struct my_msgbuf*)packet)->msgtype );
            if ( rc < 0 )
                    debug_lib::throw_error( "systemv send: cannot send message\n" );
            return rc;
        }

        int SystemVIPC::recv(const char* dest, void* ptr, int size){
            int qid = locateMailBox(dest);
            return recv( qid, ptr, size );
        }


        //recv can also be used to peek into the message
        int SystemVIPC::recv(int dest_id, void* ptr, int size){
            int flag = 0;
	    my_msgbuf* packet = NULL;
            packet = (my_msgbuf*) ptr;
            if( size == 0 ) //to support peek functionality
                    flag = MSG_NOERROR | IPC_NOWAIT;
            if ( packet->msgtype <= 0 )
                    debug_lib::throw_error( "systemv recv: logic error, packet.msgtype not a positive value" );

	    int rc = 0;
	    int readSize = 0;
	    if( size != 0 )
		readSize = size - sizeof(long);
	    debug_lib::log ( "waiting for packet = %ld on recv, dest_id %d, readsize= %d", packet->msgtype, dest_id, readSize );
            rc = msgrcv( dest_id, (struct msgbuf*)packet, readSize, packet->msgtype, flag );
            if ( rc < 0 )
                    debug_lib::throw_error( "systemv recv: cannot recv message\n" );
            if ( ptr )
                    memcpy( ptr, packet->mtext, rc );
            return rc;
        }

        void SystemVIPC::dumpMemory(){
            debug_lib::log( "use ipcs to view queue" );
        }

        int SystemVIPC::getKey(const char* name){
            char buf[256] = {0};
            sprintf( buf, "%s/%s", _path.c_str(), name );
            int key = ftok( buf, 'a' );
            if( key < 0 )
                    debug_lib::throw_fatal_error( "getKey: cannot create queue for name %s", name );
            return key;
        }


}
