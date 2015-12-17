#ifndef __MY_IPC_LIB__
#define __MY_IPC_LIB__


namespace ipc_lib{

	class Mutex{
		public:
			Mutex();
			~Mutex();
			lock();
			release();
	};

	//A system V shared memory implementation
	class SharedMemory{
		public:
			SharedMemory(const char* name);
			~SharedMemory();

			init();
			void* alloc(int size);
			void free(void* ptr);

		private:
			Mutex _mutex;
	};

	class IPC{
		public:
			void send(void* ptr, int len)=0;
			int recv(void* ptr, int size)=0;
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
	};

	class UnixDomainSocketIPC : public IPC{
		public:
			void send(void* ptr, int len);
			int recv(void* ptr, int size);
	};

}

#endif
