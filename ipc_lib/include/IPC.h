#ifndef __MY_IPC_LIB__
#define __MY_IPC_LIB__


namespace ipc_lib{

	class Mutex{
		public:
			Mutex(const char* key, int snum);
			~Mutex();
			lock(int snum=0);
			unlock(int snum=0);
			const char* getKey(){ _key.c_str(); }

		private:
			int _mutex;
			std::string _key;
			int _locks_held;
	};

	class AutoMutex{
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

	
	class ShmInitializer{
		public:
			void initialize( void* ptr, int numBlocks, int blockSize )=0;
	};

	class MailBoxQInitializer :: ShmInitializer{
		public:
			void initializer( void* shm, int numBlocks, int blockSize ){
				int* ptr = (int*) shm;
				for(int i = 0; i < ((numBlocks * blockSize)/sizeof(int)); i++ ){
					ptr[i] = -1;
				}
			}
	};

	class DataInitializer :: ShmInitializer{
		public:
			void initializer( void* shm, int numBlocks, int blockSize ){
				memset( shm, 0x00, (numBlocks*blockSize) );
			}
	};

	class MailBoxIdInitializer :: ShmInitializer{
		public:
			void initializer( void* shm, int numBlocks, int blockSize ){
				memset( shm, 0x00, (numBlocks*blockSize) );
			}
	};

	class MailBoxMetaInitializer :: ShmInitializer{
		public:
			void initializer( void* shm, int numBlocks, int blockSize ){
				memset( shm, 0x00, (numBlocks*blockSize) );
			}
	};



	//A system V shared memory implementation
	class SharedMemory{
		public:
			SharedMemory(const char* key, int blocksize, int numBlocks, ShmInitializer* initObj);
			~SharedMemory();
			void attach();
			void* allocBlock(int& index);
			void freeBlock(void* b);
			void freeBlock(int index);
			void dumpMemory();
			int getBlockSize(){ return _blockSize; }
			int getNumBlocks(){ return _numBlocks; }

		private:
			int _semid;
			std::string _key;
			int _blockSize;
			int _numBlocks;
			int _createSize;
			void* _shmAddress;
			int* _free_list_head;
			int* _indexTable;
			bool checkIndexForAccess(int index);
			bool checkIndex(int index);
			int isEndOfMemory(void* ptr);
	};


	//IPC Using system V shared memory. The buffers are in shm and the 
	//index of the buffer to use is communicated via TCPIP
	class IPC{
		public:
			void send(const char* dest, void* ptr, int len)=0;
			void send(int dest_id, void* ptr, int len)=0;
			int recv(const char* dest, void* ptr, int size)=0;
			int recv(int dest_id, void* ptr, int size)=0;
			virtual ~IPC(){};
		private:
			Mutex _global_mutex;
			Mutex _record_mutex;
	};

	class ShmIPC : public IPC{
		public:
			void send(int mbid, void* ptr, int len);
			int recv(int mbid, void* ptr, int size);
			int recv_no_wait(int mbid, void* ptr, int size);
			void send(const char* mbname, void* ptr, int len);
			int recv(const char* mbname, void* ptr, int size);
			int recv_no_wait(const char* mbname, void* ptr, int size);
		private:
			SharedMemeory* _dataShm;
			SharedMemeory* _mailboxQShm;
			SharedMemeory* _mailboxIdShm;
			int recv_priv(int* mbIdQAddr, void* ptr, int size);
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
