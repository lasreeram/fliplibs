#ifndef __MY_IPC_LIB__
#define __MY_IPC_LIB__


namespace ipc_lib{

	class Mutex{
		public:
			Mutex();
			~Mutex();
			lock();
			unlock();
			const char* getKey(){ _key.c_str(); }

		private:
			int _mutex;
			std::string _key;
	};

	//A system V shared memory implementation
	class SharedMemory{
		public:
			SharedMemory(const char* key);
			~SharedMemory();

			void* alloc(int size);
			void free(void* ptr);

		private:
			int _semid;
			std::string _key;
	};


	//IPC Using system V shared memory. The buffers are in shm and the 
	//index of the buffer to use is communicated via TCPIP
	class IPC{
		public:
			void send(void* ptr, int len)=0;
			int recv(void* ptr, int size)=0;
		private:
			SharedMemeory _shm;
			Mutex _mutex;
	};

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

	class ShmIPC : public IPC{
		public:
			void send(void* ptr, int len);
			int recv(void* ptr, int size);
		private:
			SharedMemeory _shm2;
	};

	class UnixDomainSocketIPC : public IPC{
		public:
			void send(void* ptr, int len);
			int recv(void* ptr, int size);
	};

}

#endif
