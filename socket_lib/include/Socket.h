#ifndef __MY_SOCKET_LIB__
#define __MY_SOCKET_LIB__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "skel.h"
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <exception>
#include <signal.h>
#include <map>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <vector>
#include <debug.h>
#include <openssl/ssl.h>

using namespace std;
namespace sockets_lib {
	class Socket {
		public:
			virtual int getFd() = 0;
			virtual ~Socket(){};

	};

	enum {
		BILLION_NANO_SECS = 1000000000 
	};

	class SocketException : public exception {
		public:
			SocketException(const char* err) {
				_error = err;
			}
			//throw() here means that this function cannot not throw any exception
			const char* what() const throw() { return _error.c_str(); }
			//throw() here means that this function cannot not throw any exception
			//this is required since the std::exception is compiled with noexcept (or throw nothing clause)
			//In C++ 11 all functions are noexcept(true) by default. If you really want to throw exceptions
			//you must use throw(exception,..) indicate which types you are throwing.
			~SocketException() throw() {}

		private:
			string _error;
	};


	class IOPollManager;
	class IOPollHandler{
		public:
			virtual void handle(IOPollManager* mgr) = 0;
			virtual ~IOPollHandler(){}
	};

	class IOPollMultiTimerManager;
	class IOPollMultiTimerHandler {
		public:
			virtual void handle(IOPollMultiTimerManager* mgr) = 0;
			virtual void timeout(uint32_t timer_id, IOPollMultiTimerManager* mgr) = 0;
			virtual ~IOPollMultiTimerHandler(){};
	};

	struct PollInfo{
		int fd;
		IOPollHandler* handler;
	};

	struct PollMultiTimerInfo{
		int fd;
		IOPollMultiTimerHandler* handler;
	};

	class IOPollManager{
		public:
			IOPollManager();
			void removeFromReadSet(int fd);
			void removeFromWriteSet(int fd);
			void removeFromExceptionSet(int fd);
			void addSourceForRead(int fd, IOPollHandler* handler);
			void addSinkForWrite(int fd, IOPollHandler* handler);
			void addFdForException(int fd, IOPollHandler* handler);
			void setTimeoutHandler(IOPollHandler* timeoutHandler);
			virtual void pollAndHandleEvents();
			void resetTimer(long seconds, long nanosec);
			virtual ~IOPollManager();
		private:
			std::map<int,PollInfo> _readPis;
			std::map<int,PollInfo> _writePis;
			std::map<int,PollInfo> _exceptionPis;
			fd_set _readfds;
			fd_set _writefds;
			fd_set _exceptionfds;
			fd_set _check_readfds;
			fd_set _check_writefds;
			fd_set _check_exceptionfds;
			struct timespec _timerdata;
			int _max_fd;
			bool _do_read, _do_write, _do_exception;
			IOPollHandler* _timeoutHandler;

			int getMaxFd();
			void setMaxFd(int fd);
			void callHandlers();
	};

	class IOPollMultiTimerManager {
		public:
			virtual ~IOPollMultiTimerManager();
			IOPollMultiTimerManager();
			void removeFromReadSet(int fd);
			void removeFromWriteSet(int fd);
			void removeFromExceptionSet(int fd);
			void addSourceForRead(int fd, IOPollMultiTimerHandler* handler);
			void addSinkForWrite(int fd, IOPollMultiTimerHandler* handler);
			void addFdForException(int fd, IOPollMultiTimerHandler* handler);
			int getTimerData( uint32_t timer_id, vector<char>& message );
			int setTimer( uint32_t timer_id, int millisec, IOPollMultiTimerHandler* handler );
			int setTimer( uint32_t timer_id, int millisec, IOPollMultiTimerHandler* handler, vector<char>& timerdata );
			// not re-enterant
			void renewTimer( uint32_t timer_id,int millisec, vector<char>& data );
			void cancelTimer( uint32_t id );
			virtual void pollAndHandleEvents();
			void removeTimerData( uint32_t timer_id );

		private:
			struct tevent_t{
				struct timespec tv;
				IOPollMultiTimerHandler* handler;
				uint32_t id;
			};

			std::map< uint32_t, vector<char> > _timer_user_data;
			std::vector< tevent_t > _timer_info;
		            
			void saveTimerData( uint32_t timer_id, vector<char>& message );
			int compare_times( struct timespec* tv1, struct timespec* tv2 );

			std::map<int,PollMultiTimerInfo> _readPis;
			std::map<int,PollMultiTimerInfo> _writePis;
			std::map<int,PollMultiTimerInfo> _exceptionPis;
			fd_set _readfds;
			fd_set _writefds;
			fd_set _exceptionfds;
			fd_set _check_readfds;
			fd_set _check_writefds;
			fd_set _check_exceptionfds;
			struct timespec _timerdata;
			int _max_fd;
			bool _do_read, _do_write, _do_exception;

			int getMaxFd();
			void setMaxFd(int fd);
			void callHandlers();

	};

	class SocketHelper
	{
		public:
			void set_address( const char* hname, const char* sname, struct sockaddr_in* sap, const char* protocol);
	};

	class UDPSocket : public Socket {
		public:
			UDPSocket();
			UDPSocket(int socket);
			struct sockaddr_in* getPeerAddress();
			int recvFrom(char* buf, int bufsize);
			//in udp client calls sendto first and recvfrom later. Server calls recvfrom first and send to later
			//in tcp either side can initiate the conversation
			int sendTo( const void* buf, int buflen);
			void setSocketOption(int option_value, int on = 0 );
			int getFd();

		protected:
			SOCKET _socket;
			struct sockaddr_in _local;
			struct sockaddr_in _peer;
			SocketHelper _helper;
			socklen_t _peerlen;
	};


	class UDPServerSocket : public UDPSocket {
		public:
			void bind( const char* hname, const char* sname );
			void setReuseAddressOption();
	};

	class UDPClientSocket : public UDPSocket {
		public:
			//setaddress initializes the peer structure for use in sendTo and recvFrom
			void setAddress( const char* hname, const char* sname);
			//udp is connection less. 
			//This serves to set the default address to which the data grams are sent
			//Also it sets the address to be the only address from which data grams can be received.
			//The client can change its association with any server at any point in the program
			void connect( const char* hname, const char* sname );
			void connect( struct sockaddr_in* peer );
	};

	class TCPSocket : public Socket {
		public:
			TCPSocket();
			TCPSocket(SOCKET s);
			void setSocketOption(int option_value, int on = 0 );
			int recv_no_header( void* buf, size_t size);
			int recv_header( void* buf, size_t len );
			int recvn(void* buf, int nbytes);
			int readLine(char* bufptr, int len);
			int writeLine( const char* buf, int size_of_buf);
			int send_no_header( const void* buf, size_t len );
			int send_header( const void* buf, size_t len );
			void shutdown(int type);
			void close();
			virtual bool isServer()=0;
			virtual bool isClient()=0;
			int getFd();
			virtual ~TCPSocket();

		protected:
			SOCKET _socket;



	};

	class TCPAcceptSocket : public TCPSocket {
			public:
			TCPAcceptSocket(SOCKET s) : TCPSocket(s) {}
			virtual bool isServer(){ return true; }
			virtual bool isClient(){ return false; }
	};

	class TCPServerSocket : public TCPSocket {
			public:
			virtual bool isServer(){ return true; }
			virtual bool isClient(){ return false; }
			void bind( const char* hname, const char* sname );
			void listen(int backlog=5);
			TCPSocket* accept();

			private:
			SocketHelper _helper;
			struct sockaddr_in _local;
			struct sockaddr_in _peer;
			unsigned int _peerlen;
	};

	class TCPClientSocket : public TCPSocket {
			public:
			virtual bool isServer(){ return false; }
			virtual bool isClient(){ return true; }
			void connect(const char* hname, const char* sname);

			private:
			SocketHelper _helper;
			struct sockaddr_in _peer;
	};


	class SSLSocket{
		public:
			SSLSocket(){
			}
			virtual ~SSLSocket(){
			}
	};

	class SSLHelper{
		public:
			static void initializeCtxt( bool isServer, std::string filePath, SSL_CTX** ssl_ctxt );
			static const char* getSSLError();
	};

	class SSLClientSocket : public SSLSocket {
		public:
			SSLClientSocket(TCPClientSocket* clSocket, const char* certPath);
			SSLClientSocket(SOCKET socket, const char* certPath);
			int write( const char* buf, int len );
			int read( const char* buf, int size );
			void shutdown();

		private:
			void initialize(SOCKET socket, const char* certPath);
			static SSL_CTX* _ssl_ctxt;
			static bool _initialized;
			SSL* _ssl;
	};

	class SSLServerSocket : public SSLSocket {
		public:
			SSLServerSocket(TCPAcceptSocket* srvSocket, const char* certPath);
			SSLServerSocket(SOCKET socket, const char* certPath);
			int write( const char* buf, int len );
			int read( const char* buf, int size );
			void shutdown();

		private:
			void initialize(SOCKET socket, const char* certPath);
			static SSL_CTX* _ssl_ctxt;
			static bool _initialized;
			SSL* _ssl;
	};

}
#endif
