#ifndef __MY_IPC_LIB__
#define __MY_IPC_LIB__
#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <debug.h>


namespace ipc_lib{
	//
	//union semnum is defined as recommended in sys/sem.h
	//
	union semun
	{
		int val; 
		struct semid_ds *buf;
		unsigned short int *array;
		struct seminfo *__buf;
	};

	class Mutex{
		public:
			Mutex(const char* key, int snum);
			~Mutex();
			void lock(int snum=0);
			void unlock(int snum=0);
			const char* getKey(){ return _key.c_str(); }

		private:
			int _mutex;
			std::string _key;
			int _keyid;
			int _locks_held;
	};

	class AutoMutex{
		friend class Mutex;
		public:
			AutoMutex(Mutex* mutex, int snum){ 
				_mutex = mutex; 
				_snum = snum; 
				_mutex->lock(_snum); 
				_unlock_done = false;
			}
			~AutoMutex(){ 
				if( !_unlock_done ) 
					_mutex->unlock(_snum); 
			}
			void release(){ 
				_mutex->unlock(_snum); 
				_unlock_done = true; 
			}

		private:
			Mutex* _mutex;
			int _snum;
			int _unlock_done;

	};

	
	class ShmInitializer {
		public:
			virtual void initialize( void* ptr, int numBlocks, int blockSize )=0;
			virtual ~ShmInitializer(){};
	};

	class ActorMetaInitializer : public ShmInitializer{
		public:
			void initialize( void* shm, int numBlocks, int blockSize ){
				memset( shm, 0x00, (numBlocks*blockSize) );
			}
	};

	class IntegerZeroInitializer : public ShmInitializer {
		public:
			void initialize( void* shm, int numBlocks, int blockSize ){
				int* ptr = (int*) shm;
				for(unsigned int i = 0; i < ((numBlocks * blockSize)/sizeof(int)); i++ ){
					ptr[i] = 0;
				}
			}
	};


	class IntegerMinusOneInitializer : public ShmInitializer {
		public:
			void initialize( void* shm, int numBlocks, int blockSize ){
				int* ptr = (int*) shm;
				for(unsigned int i = 0; i < ((numBlocks * blockSize)/sizeof(int)); i++ ){
					ptr[i] = -1;
				}
			}
	};

	class DataInitializer : public ShmInitializer{
		public:
			void initialize( void* shm, int numBlocks, int blockSize ){
				memset( shm, 0x00, (numBlocks*blockSize) );
			}
	};

	class ActorIdInitializer : public ShmInitializer{
		public:
			void initialize( void* shm, int numBlocks, int blockSize ){
				memset( shm, 0x00, (numBlocks*blockSize) );
			}
	};


	class ShmDataDumper{
		public:
			virtual void dump( void* shm, int numBlocks, int blockSize )=0;
			virtual ~ShmDataDumper(){};
	};

	class DummyDumper : public ShmDataDumper {
		public:
			void dump( void* shm, int numBlocks, int blockSize ){
				return; //do nothing
			}
	};

	class MailboxIdQDumper : public ShmDataDumper {
		public:
			void dump( void* shm, int numBlocks, int blockSize ){
				char* ptr = (char*) shm;
				for( int i = 0; i < numBlocks; i++ ){
					ptr = (char*)shm + (i*blockSize);
					if( *ptr == '\0' )
						break;
					debug_lib::log( "actor %s has id %d", ptr, i );
				}
			}
	};

	class MsgSizeDumper : public ShmDataDumper {
		public:
			void dump( void* shm, int numBlocks, int blockSize ){
				int* ptr;
				for( int i = 0; i < numBlocks; i++ ){
					ptr = (int*) ((char*)shm + (i*blockSize));
					if( *ptr <= 0 )
						continue;
					debug_lib::log( "msgsize for msg(%d) = %d", i, *ptr );
				}
			}
	};

	class BlockChainDumper : public ShmDataDumper {
		public:
			void dump( void* shm, int numBlocks, int blockSize ){
				int* ptr;
				for( int i = 0; i < numBlocks; i++ ){
					ptr = (int*) ((char*)shm + (i*blockSize));
					if( *ptr == -1 )
						continue;
					debug_lib::log_no_newline( "block chain %d: ", i );
					while( *ptr >= 0 ){
						debug_lib::log_no_newline( "%d,", *ptr );
						ptr += 1;
					}
					debug_lib::log_no_newline( "\n" );
				}
			}
	};

	class MailboxQDumper : public ShmDataDumper {
		public:
			void dump( void* shm, int numBlocks, int blockSize ){
				int* ptr;
				for( int i = 0; i < numBlocks; i++ ){
					ptr = (int*) ((char*)shm + (i*blockSize));
					bool once = false;
					while( *ptr >= 0 ){
						if( !once ){
							debug_lib::log_no_newline( "\nmessage queue for actor %d = ", i );
							once = true;
						}
						debug_lib::log_no_newline( "%d,", *ptr );
						ptr += sizeof(int);
					}
				}
				debug_lib::log_no_newline("\n");
			}
	};


	//A system V shared memory implementation
	class SharedMemory{
		public:
			SharedMemory(const char* key, int blocksize, int numBlocks, ShmInitializer* initObj);
			virtual ~SharedMemory();
			virtual void* allocBlock(int& index);
			virtual void freeBlock(void* b);
			virtual void freeBlock(int index);
			virtual void dumpMemory(ShmDataDumper* applicationDumper);
			int getBlockSize(){ return _blockSize; }
			int getNumBlocks(){ return _numBlocks; }
			int getIndexForAddress(void* addr);
			void* getAddressForIndex(int index);
			bool isValidAddress(void* ptr);
			int isEndOfMemory(void* ptr);

		protected:
			int _semid;
			std::string _key;
			int _keyid;
			int _blockSize;
			int _numBlocks;
			int _createSize;
			void* _shmAddress;
			int* _free_list_head;
			int* _indexTable;
			bool checkIndexForAccess(int index);
			bool checkIndex(int index);
			int attach();
			void init();
	};

	class FixedIndexSharedMemory : public SharedMemory{
		public:
			FixedIndexSharedMemory(const char* key, int blocksize, int numBlocks, ShmInitializer* initObj)
				: SharedMemory( key, blocksize, numBlocks, initObj ) 
			{
			}
			virtual void dumpMemory(ShmDataDumper* applicationDumper);
			virtual void* allocBlock(int& index){
				debug_lib::throw_error( "logic error: no implementation. should not be called" );
				return NULL;
			}
			virtual void freeBlock(void* b){
				debug_lib::throw_error( "logic error: no implementation. should not be called" );
			}
			virtual void freeBlock(int index){
				debug_lib::throw_error( "logic error: no implementation. should not be called" );
			}
	};

	class SharedMemoryVariableSize{
		public:
			SharedMemoryVariableSize(int blockSize, int maxMsgSize, int numMessages);
			~SharedMemoryVariableSize();
			void allocateAndCopy( void* data, int datalen, int& id );
			void getData( int id, void* data, int size, int& lenCopied );
			int getMaxMessageSize(){ return _maxMessageSize; }
			int getMaxMessages(){ return _numMessages; }
			void freeMemory( int id );
			void dumpMemory(ShmDataDumper* applicationDumper);

		private:
			SharedMemory* _msgQShm;
			SharedMemory* _msgSizeShm;
			SharedMemory* _blockChainShm;
			int _blockChainSize;
			int _numMessages;
			int _maxMessageSize;
	};

	//IPC Using system V shared memory. The buffers are in shm and the 
	//index of the buffer to use is communicated via TCPIP
	class IPC{
		public:
			virtual int createActor(const char* name) = 0;
			virtual int locateActor(const char* name) = 0;
			virtual int send(const char* dest, void* ptr, int len)=0;
			virtual int send(int dest_id, void* ptr, int len)=0;
			virtual int recv(const char* dest, void* ptr, int size)=0;
			virtual int recv(int dest_id, void* ptr, int size)=0;
			virtual void dumpMemory()=0;
			virtual ~IPC(){};
		protected:
			Mutex* _global_mutex;
			Mutex* _record_mutex;
	};

	class ShmIPC : public IPC{
		public:
			int createActor(const char* name);
			int locateActor(const char* name);
			void getActorName(int id, std::string& name);
			int send(const char* dest, void* ptr, int len);
			int send(int mbid, void* ptr, int len);
			int recv(int mbid, void* ptr, int size);
			int recv_no_wait(int mbid, void* ptr, int size);
			int recv(const char* mbname, void* ptr, int size);
			int recv_no_wait(const char* mbname, void* ptr, int size);
			void dumpMemory();
			~ShmIPC();
			ShmIPC();
		private:
			SharedMemoryVariableSize* _msgQShm;
			SharedMemory* _actorQShm;
			SharedMemory* _actorIdShm;
			SharedMemory* _actorMetaData;
			int recv_priv(int* mbIdQAddr, int mbid, void* ptr, int size);
	};

	#define MAX_SEND_SIZE 4056	

	//use for send and recv
	struct my_msgbuf{
		long msgtype;
		char mtext[MAX_SEND_SIZE];
	};

	//important ideas
	// IPC key : A unique identifier to identify the entire System V queue (where queues based on user keys are created)
	//           For each client-server pair, each direction a separate queue must be created

	//MAXMSG size is 4056 for system v queues

	//How does the kernal internally store the message
	//As a singly linked list along with the size of the message
	class SystemVIPC : public IPC{
		public:
			SystemVIPC( const char* basePath );
			~SystemVIPC(){
			}
			int createActor(const char* name);
			int locateActor(const char* name);
			int send(const char* dest, void* ptr, int len);
			int send(int dest_id, void* packet, int len);
			int recv(const char* dest, void* ptr, int size);
			//recv can also be used to peek into the message
			int recv(int dest_id, void* ptr, int size);
			void dumpMemory();

		private:
			int getKey(const char* name);
			std::string _path;
	};


/*
	class TCPSocketIPC : public IPC{
		public:
			SocketIPC(TCPSocket socket){ _socket = socket; }
			~SocketIPC();
			void send(void* ptr, int len);
			int recv(void* ptr, int size);

		public:
			TCPSocket _socket;
	};

	class PipeIPC : public IPC{
		public:
			void send(void* ptr, int len);
			int recv(void* ptr, int size);
	};


	class UnixDomainSocketIPC : public IPC{
		public:
			void send(void* ptr, int len);
			int recv(void* ptr, int size);
	};
*/

}

#endif
