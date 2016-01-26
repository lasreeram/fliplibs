#ifndef __MY_IPC_LIB__
#define __MY_IPC_LIB__
#include <string>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>


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

	class MailBoxMetaInitializer : public ShmInitializer{
		public:
			void initialize( void* shm, int numBlocks, int blockSize ){
				memset( shm, 0x00, (numBlocks*blockSize) );
			}
	};

	class MailBoxQInitializer : public ShmInitializer {
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

	class MailBoxIdInitializer : public ShmInitializer{
		public:
			void initialize( void* shm, int numBlocks, int blockSize ){
				memset( shm, 0x00, (numBlocks*blockSize) );
			}
	};



	//A system V shared memory implementation
	class SharedMemory{
		public:
			SharedMemory(const char* key, int blocksize, int numBlocks, ShmInitializer* initObj);
			~SharedMemory();
			void attach();
			void init();
			void* allocBlock(int& index);
			void freeBlock(void* b);
			void freeBlock(int index);
			void dumpMemory();
			int getBlockSize(){ return _blockSize; }
			int getNumBlocks(){ return _numBlocks; }
			int getIndexForAddress(void* addr);
			void* getAddressForIndex(int index);
			bool isValidAddress(void* ptr);
			int isEndOfMemory(void* ptr);

		private:
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
	};


	//IPC Using system V shared memory. The buffers are in shm and the 
	//index of the buffer to use is communicated via TCPIP
	class IPC{
		public:
			virtual int createMailBox(const char* name) = 0;
			virtual int locateMailBox(const char* name) = 0;
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
			int createMailBox(const char* name);
			int locateMailBox(const char* name);
			void getMailBoxName(int id, std::string& name);
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
			SharedMemory* _msgQShm;
			SharedMemory* _mailboxQShm;
			SharedMemory* _mailboxIdShm;
			SharedMemory* _mailboxMetaData;
			int recv_priv(int* mbIdQAddr, int mbid, void* ptr, int size);
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
