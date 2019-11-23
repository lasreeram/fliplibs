#include <Socket.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>

namespace sockets_lib{

	IOPollManager::IOPollManager(){
		FD_ZERO( &_readfds );
		FD_ZERO( &_writefds );
		FD_ZERO( &_exceptionfds );
		FD_ZERO( &_check_readfds );
		FD_ZERO( &_check_writefds );
		FD_ZERO( &_check_exceptionfds );
		_max_fd = -1;
		_do_read = false;
		_do_write = false;
		_do_exception = false;
		resetTimer(0, 0);
		_timeoutHandler = NULL;
	}

	void IOPollManager::removeFromReadSet(int fd){
		_readPis.erase(fd);
		FD_CLR( fd, &_readfds );
	}

	void IOPollManager::removeFromWriteSet(int fd){
		_writePis.erase(fd);
		FD_CLR( fd, &_writefds );
	}

	void IOPollManager::removeFromExceptionSet(int fd){
		_exceptionPis.erase(fd);
		FD_CLR( fd, &_exceptionfds );
	}

	void IOPollManager::addSourceForRead(int fd, IOPollHandler* handler){
		PollInfo pi;
		pi.fd = fd;
		pi.handler = handler;
		_readPis[fd] = pi;
		FD_SET( fd, &_readfds );
		setMaxFd(fd);
		_do_read = true;
	}

	void IOPollManager::addSinkForWrite(int fd, IOPollHandler* handler){
		PollInfo pi;	
		pi.fd = fd;
		pi.handler = handler;
		_writePis[fd] = pi;
		FD_SET( fd, &_writefds );
		setMaxFd(fd);
		_do_write = true;
	}

	void IOPollManager::addFdForException(int fd, IOPollHandler* handler){
		PollInfo pi;	
		pi.fd = fd;
		pi.handler = handler;
		_exceptionPis[fd] = pi;
		FD_SET( fd, &_exceptionfds );
		setMaxFd(fd);
		_do_exception = true;
	}

	void IOPollManager::setTimeoutHandler(IOPollHandler* timeoutHandler){
		_timeoutHandler = timeoutHandler;
	}

	void IOPollManager::pollAndHandleEvents(){
		int maxfd;
		struct timespec* timerdata = NULL;
		int rc;
		while(true){
			maxfd = getMaxFd() + 1;
			_check_readfds = _readfds;
			_check_writefds = _writefds;
			_check_exceptionfds = _exceptionfds;
			if ( _timerdata.tv_sec > 0 || _timerdata.tv_nsec > 0 )
				timerdata = &_timerdata;
			//pselect - the last argument is the signal to mask
			rc = pselect ( 
						maxfd, 
						(_do_read? &_check_readfds: NULL), 
						(_do_write? &_check_writefds: NULL), 
						(_do_exception? &_check_exceptionfds: NULL), 
						timerdata,
						NULL
					); 
			if ( rc < 0 )
				debug_lib::throw_error(  "select failed %d", errno );
			if ( rc == 0 ){
				if( _timeoutHandler ){
					_timeoutHandler->handle(this);
				}else
					debug_lib::throw_error(  "timeout handler not configured" );
				
				return;
			}
			callHandlers();
			//return;
		}
	}

	void IOPollManager::resetTimer(long seconds, long nanosec){
		_timerdata.tv_sec = seconds;	
		_timerdata.tv_nsec = nanosec;
	}

	int IOPollManager::getMaxFd(){
		return _max_fd;
	}

	void IOPollManager::setMaxFd(int fd){
		if ( _max_fd < fd )
			_max_fd = fd;
	}

	void IOPollManager::callHandlers(){
		std::map<int,PollInfo>::iterator iter; 
		for( iter = _readPis.begin(); _do_read && iter != _readPis.end(); iter++ ){
			if( FD_ISSET( iter->first, &_check_readfds ) ){
				iter->second.handler->handle(this);
			}
		}
		for( iter = _writePis.begin(); _do_write && iter != _writePis.end(); iter++ ){
			if( FD_ISSET( iter->first, &_check_writefds ) ){
				iter->second.handler->handle(this);
			}
		}
		for( iter = _exceptionPis.begin(); _do_exception && iter != _exceptionPis.end(); iter++ ){
			if( FD_ISSET( iter->first, &_check_exceptionfds ) ){
				iter->second.handler->handle(this);
			}
		}
	}

	IOPollManager::~IOPollManager(){
	}


//IOPollMultiTimerManager definition
	IOPollMultiTimerManager::~IOPollMultiTimerManager(){}
	IOPollMultiTimerManager::IOPollMultiTimerManager(){
		FD_ZERO( &_readfds );
		FD_ZERO( &_writefds );
		FD_ZERO( &_exceptionfds );
		FD_ZERO( &_check_readfds );
		FD_ZERO( &_check_writefds );
		FD_ZERO( &_check_exceptionfds );
		_max_fd = -1;
		_do_read = false;
		_do_write = false;
		_do_exception = false;
	}
	
	void IOPollMultiTimerManager::removeFromReadSet(int fd){
		_readPis.erase(fd);
		FD_CLR( fd, &_readfds );
	}

	void IOPollMultiTimerManager::removeFromWriteSet(int fd){
		_writePis.erase(fd);
		FD_CLR( fd, &_writefds );
	}

	void IOPollMultiTimerManager::removeFromExceptionSet(int fd){
		_exceptionPis.erase(fd);
		FD_CLR( fd, &_exceptionfds );
	}

	void IOPollMultiTimerManager::addSourceForRead(int fd, IOPollMultiTimerHandler* handler){
		PollMultiTimerInfo pi;	
		pi.fd = fd;
		pi.handler = handler;
		_readPis[fd] = pi;
		FD_SET( fd, &_readfds );
		setMaxFd(fd);
		_do_read = true;
	}

	void IOPollMultiTimerManager::addSinkForWrite(int fd, IOPollMultiTimerHandler* handler){
		PollMultiTimerInfo pi;	
		pi.fd = fd;
		pi.handler = handler;
		_writePis[fd] = pi;
		FD_SET( fd, &_writefds );
		setMaxFd(fd);
		_do_write = true;
	}

	void IOPollMultiTimerManager::addFdForException(int fd, IOPollMultiTimerHandler* handler){
		PollMultiTimerInfo pi;	
		pi.fd = fd;
		pi.handler = handler;
		_exceptionPis[fd] = pi;
		FD_SET( fd, &_exceptionfds );
		setMaxFd(fd);
		_do_exception = true;
	}

	int IOPollMultiTimerManager::getTimerData( uint32_t timer_id, vector<char>& message ){
		map< uint32_t, vector<char> >::iterator iter;
		iter = _timer_user_data.find( timer_id );
		if ( iter == _timer_user_data.end() )
			return -1;
		message = iter->second;
		vector<char>::iterator it2;
		//for ( it2 = message.begin(); it2 != message.end(); it2++ ){
		//	printf( "%02x", *it2 );
		//}
		//printf ("\n");
		return 0;
	}

	int IOPollMultiTimerManager::setTimer( uint32_t timer_id, int millisec, IOPollMultiTimerHandler* handler ){
		vector<char> emptyStr;
		return setTimer( timer_id, millisec, handler, emptyStr );
	}

	int IOPollMultiTimerManager::setTimer( uint32_t timer_id, int millisec, IOPollMultiTimerHandler* handler, vector<char>& timerdata ){
		tevent_t tp;

		tp.handler = handler;
		if ( clock_gettime( CLOCK_REALTIME, &tp.tv ) < 0 )	
			debug_lib::throw_error( "clock get time returned error" );

		tp.id = timer_id;
		long long nanosec = (long long)tp.tv.tv_nsec + ((long long)millisec * 1000000);
		if( nanosec > BILLION_NANO_SECS ){
			long long secs_in_nanosec = nanosec/BILLION_NANO_SECS;	
			tp.tv.tv_sec += (long)(secs_in_nanosec);
			tp.tv.tv_nsec = (nanosec % (BILLION_NANO_SECS*(secs_in_nanosec)));
		}else{
			tp.tv.tv_nsec = (long)nanosec;
		}

		vector<tevent_t>::iterator iter;
		for ( iter = _timer_info.begin(); 
			iter != _timer_info.end() && (compare_times( &tp.tv, &iter->tv) > 0); 
			iter++ )
		;
		_timer_info.insert( iter, tp );

		if ( !timerdata.empty() )
			saveTimerData( timer_id, timerdata );
		

		return 0;
	}

	// not re-enterant
	void IOPollMultiTimerManager::renewTimer( uint32_t timer_id,int millisec, vector<char>& data ){
		vector< tevent_t>::iterator iter;
		for ( iter = _timer_info.begin(); iter != _timer_info.end(); iter++ ){
			if ( iter->id == timer_id )
				break;
		}

		if ( iter == _timer_info.end() )
			debug_lib::throw_error( "no timer to renew - logic error in application" );
		
		tevent_t event = *iter; //cancel removes the timer so iter cannot be used
		cancelTimer(timer_id);
		setTimer( timer_id, millisec, event.handler, data );
	}

	void IOPollMultiTimerManager::cancelTimer( uint32_t id ){
		vector< tevent_t>::iterator iter;
		for ( iter = _timer_info.begin(); iter != _timer_info.end(); iter++ ){
			if( iter->id == id ){
				iter = _timer_info.erase( iter );
				break;
			}
		}
		removeTimerData( id );
	}

	void IOPollMultiTimerManager::pollAndHandleEvents(){
		struct timespec* tvp;
		struct timespec tv;
		struct timespec now;
		vector< tevent_t>::iterator iter;
		while(true){

			if ( clock_gettime( CLOCK_REALTIME, &now ) < 0 )	
				debug_lib::throw_error( "clock_gettime failed" );
			iter = _timer_info.begin();
			while( iter != _timer_info.end() && (compare_times( &now, &iter->tv) > 0) ){
				iter->handler->timeout(iter->id, this);
				iter = _timer_info.erase(iter);
			}

			iter = _timer_info.begin();
			if ( iter != _timer_info.end() ){
				tv.tv_sec = iter->tv.tv_sec - now.tv_sec;
				tv.tv_nsec = iter->tv.tv_nsec - now.tv_nsec;
				if ( tv.tv_nsec < 0 ){
					tv.tv_sec--;
					tv.tv_nsec += BILLION_NANO_SECS;
				}
				tvp = &tv;
			}else if ( !_do_read && !_do_write && !_do_exception )
				debug_lib::throw_error( "pollAndHandleEvents called without any fds to check" );
			else
				tvp = NULL;

			int maxfd = getMaxFd() + 1;
			_check_readfds = _readfds;
			_check_writefds = _writefds;
			_check_exceptionfds = _exceptionfds;
			int rc = pselect ( 
						maxfd, 
						(_do_read? &_check_readfds: NULL), 
						(_do_write? &_check_writefds: NULL), 
						(_do_exception? &_check_exceptionfds: NULL), 
						tvp,
						NULL
					); 
			if ( rc < 0 )
				debug_lib::throw_error(  "select failed %d", errno );
			else if ( rc == 0 ) //timeout
				continue;
			callHandlers();
		}
		return;
	}

	void IOPollMultiTimerManager::removeTimerData( uint32_t timer_id ){
		std::map<uint32_t, vector<char> >::iterator iter;
		iter = _timer_user_data.find( timer_id );
		if ( iter != _timer_user_data.end() )
			_timer_user_data.erase( iter );
	}			

	void IOPollMultiTimerManager::saveTimerData( uint32_t timer_id, vector<char>& message ){
		_timer_user_data[timer_id] = message;
	}

	
	int IOPollMultiTimerManager::compare_times( struct timespec* tv1, struct timespec* tv2 ){
		int ret;
		if ( tv1->tv_sec < tv2->tv_sec )
			ret = -1;
		else if ( tv1->tv_sec == tv2->tv_sec && tv1->tv_nsec < tv2->tv_nsec )
			ret = -1;
		else if ( tv1->tv_sec == tv2->tv_sec && tv1->tv_nsec == tv2->tv_nsec )
			ret = 0;
		else
			ret = 1;
		return ret;
	}

	int IOPollMultiTimerManager::getMaxFd(){
		return _max_fd;
	}

	void IOPollMultiTimerManager::setMaxFd(int fd){
		if ( _max_fd < fd )
			_max_fd = fd;
	}

	void IOPollMultiTimerManager::callHandlers(){
		std::map<int,PollMultiTimerInfo>::iterator iter; 
		for( iter = _readPis.begin(); _do_read && iter != _readPis.end(); iter++ ){
			if( FD_ISSET( iter->first, &_check_readfds ) ){
				iter->second.handler->handle(this);
			}
		}
		for( iter = _writePis.begin(); _do_write && iter != _writePis.end(); iter++ ){
			if( FD_ISSET( iter->first, &_check_writefds ) ){
				iter->second.handler->handle(this);
			}
		}
		for( iter = _exceptionPis.begin(); _do_exception && iter != _exceptionPis.end(); iter++ ){
			if( FD_ISSET( iter->first, &_check_exceptionfds ) ){
				iter->second.handler->handle(this);
			}
		}
	}


//SocketHelper definition

	void SocketHelper::set_address( const char* hname, const char* sname, struct sockaddr_in* sap, const char* protocol){
		struct servent* sp;
		struct hostent* hp;
		char* endptr;
		short port;

		memset( sap, '\0', sizeof(struct sockaddr_in) );
		sap->sin_family = AF_INET;
		//get the host name resolved
		if ( hname != NULL ){
			//converts an ip address from number-and-dots notation into a binary form.
			if ( !inet_aton( hname, &sap->sin_addr) ){
				//converts a DNS name into an numeric address. The return value is a 
				//superseded by getnameinfo
				//gethostbyname works by checking the host file or contacting nameserver using DNS protocol.
				hp = gethostbyname(hname);
				if ( hp == NULL )
					debug_lib::throw_error( "unknown host %s\n", hname );
				sap->sin_addr = *(struct in_addr*)hp->h_addr;
			}
		}else{
			sap->sin_addr.s_addr = htonl(INADDR_ANY);
		}
		
		//get the port or service name
		//strol converts a string to a long
		port = strtol( sname, &endptr, 0);

		if ( *endptr == '\0' ){
			sap->sin_port = port;
		}else{
			//converts a service name into a port
			//returns a servent structure
			//superseded by getaddrinfo
			sp = getservbyname( sname, protocol );
			if ( sp == NULL ){
				debug_lib::throw_error(  "unknown service: %s\n", sname );
			}
			//port number returned by getservbname is already in network byte order
			sap->sin_port = sp->s_port;
		}
	}


//Socket classes definition

//UDPSocket

	UDPSocket::UDPSocket(){
		// last argument is protocol. it is used only when SOCK_RAW is used. ICMP for example
		_socket = socket( AF_INET, SOCK_DGRAM, 0 );
		if ( !isvalidsock(_socket) )
			debug_lib::throw_error(  "udp socket create failed, error %d,%s", 
						errno, strerror(errno) );

		//peerlen must be set to sizeof peer.
		_peerlen = sizeof(_peer);
		//no reuse address set option for udp.  
	}

	UDPSocket::UDPSocket(int socket){
		_socket = socket;
		if ( !isvalidsock(_socket) )
			debug_lib::throw_error(  "udp socket create failed, error %d,%s", 
						errno, strerror(errno) );
		//peerlen must be set to sizeof peer.
		_peerlen = sizeof(_peer);
	}

	struct sockaddr_in* UDPSocket::getPeerAddress(){
		return &_peer;
	}

	int UDPSocket::recvFrom(char* buf, int bufsize){
		int rc = -1;
		if ( ( rc = ::recvfrom( _socket, buf, bufsize, 0, (struct sockaddr*) &_peer, &_peerlen )) < 0 ){
			debug_lib::throw_error(  "udp recvfrom failed, error %d,%s", 
						errno, strerror(errno) );
		}
		return rc;
	}

	//in udp client calls sendto first and recvfrom later. Server calls recvfrom first and send to later
	//in tcp either side can initiate the conversation
	int UDPSocket::sendTo( const void* buf, int buflen){
		int rc = -1;
		if ( (rc = ::sendto( _socket, buf, buflen, 0, (struct sockaddr*) &_peer, _peerlen )) < 0 )
			debug_lib::throw_error(  "udp sendto failed, error %d,%s", 
						errno, strerror(errno) );
		return rc;
	}

	void UDPSocket::setSocketOption(int option_value, int on ){
		if ( setsockopt( _socket, SOL_SOCKET, option_value, &on, sizeof(on) ) ){
			debug_lib::throw_error( "setsockopt failed, error %d, %s", errno, strerror(errno) );
		}	
		return;
	}

	int UDPSocket::getFd(){ return _socket; }


//UDPServerSocket defintion
	void UDPServerSocket::bind( const char* hname, const char* sname ){
		_helper.set_address( hname, sname, &_local, "udp" );
		if ( ::bind (_socket, (struct sockaddr*) &_local, sizeof(_local) ) ){
			debug_lib::throw_error( "bind failed, error %d, %s", errno, strerror(errno) );
		}
	} 

	void UDPServerSocket::setReuseAddressOption(){
		setSocketOption(SO_REUSEADDR);
	}

//UDPClientSocket definition
	//setaddress initializes the peer structure for use in sendTo and recvFrom
	void UDPClientSocket::setAddress( const char* hname, const char* sname){
		_helper.set_address( hname, sname, &_peer, "udp" );
		_peerlen = sizeof(_peer);
	}

	//udp is connection less. 
	//This serves to set the default address to which the data grams are sent
	//Also it sets the address to be the only address from which data grams can be received.
	//The client can change its association with any server at any point in the program
	void UDPClientSocket::connect( const char* hname, const char* sname ){
		setAddress( hname, sname );
		if( ::connect( _socket, (struct sockaddr*)&_peer, sizeof(struct sockaddr) ) <  0 )
			debug_lib::throw_error( "connect failed, error %d, %s", errno, strerror(errno) );
	}

	void UDPClientSocket::connect( struct sockaddr_in* peer ){
		int len = sizeof(struct sockaddr_in);
		memcpy( &_peer, peer, len );
		_peerlen = len;
		if( ::connect( _socket, (struct sockaddr*) peer, len ) <  0 )
			debug_lib::throw_error( "connect failed, error %d, %s", errno, strerror(errno) );
	}


//TCPSocket definition

	TCPSocket::TCPSocket(){
		// last argument is protocol. it is used only when SOCK_RAW is used. ICMP for example
		_socket = socket( AF_INET, SOCK_STREAM, 0 );
		if ( !isvalidsock(_socket) )
			debug_lib::throw_error(  "socket create failed, error %d,%s", errno, strerror(errno) );
		setSocketOption(SO_REUSEADDR);
		//signal(SIGPIPE, SIG_IGN);
	}

	TCPSocket::TCPSocket(SOCKET s){
		_socket = s;
		if ( !isvalidsock(_socket) )
			debug_lib::throw_error(  "socket create failed, error %d,%s", errno, strerror(errno) );
	}

	void TCPSocket::setSocketOption(int option_value, int on ){
		if ( setsockopt( _socket, SOL_SOCKET, option_value, &on, sizeof(on) ) ){
			debug_lib::throw_error(  "setsockopt failed, error %d, %s", errno, strerror(errno) );
		}	
		return;
	}

	int TCPSocket::recv( void* buf, size_t size){
		int flags = 0;
		int rc = 0;
		while( true ){
			if ( ( rc = ::recv( _socket, buf, size, flags )) < 0 ){
				if ( errno == EINTR )	
					continue;
				debug_lib::throw_error(  "recv failed, error %d,%s", 
						errno, strerror(errno) );
			}
			break;
		}
		return rc;
	}


	int TCPSocket::recv_header( void* buf, size_t len ){
		int rc = -1;
		u_int32_t reclen;
		//recv prefixed length
		rc = recvn( (char*)&reclen, sizeof(u_int32_t) );
		if ( rc != sizeof(u_int32_t) ){
			if ( rc < 0 )
				return -1; //calling function can continue.
			return 0; //EOF (also called FIN) is received
			//if an EOF(rc==0) is recved the program must exit
			//sockets_lib::error(1,  "recv failed, error %d, %s", 
			//		errno, strerror(errno) );
		}

		//convert from network byte order to host byte order
		reclen = ntohl(reclen);	
		if ( reclen > len ){
			//not enough room must discard the buffer
			while( reclen > 0 ){
				rc = recvn( buf, len );
				if ( (u_int32_t) rc != len ){
					if ( rc < 0 )
						return -1; //calling function can continue
					//if an EOF(rc==0) is recved the program must exit
					debug_lib::throw_error( "recv failed, while discarding message. error %d, %s", 
						errno, strerror(errno) );
				}
				reclen -= len;
				if ( reclen < len )
					len = reclen;
			}
			set_errno( EMSGSIZE );
			debug_lib::throw_error(  "logic error, space not enough, message discarded.%d, %s", 
					errno, strerror(errno) );
		}

		//recv the actual record now
		rc = recvn( buf, reclen );
		if ( (u_int32_t) rc != reclen ){
			if ( rc < 0 )
				return -1; //calling function can continue
			return 0; // EOF (also called FIN is received)
		}
		return rc;
	}
		
	int TCPSocket::recvn(void* buf, int nbytes){
		int rc = -1;
		int flags = 0;
		int cnt = nbytes;
		char* bp = (char*)buf;
		while( cnt > 0 ){
		//recv is to recieve data from the socket. The recv return any data that is available immediately
		//instead of waiting for all data. If no data is available the call blocks, unless the socket is 
		//non-blocking.
		// flags on the recv
		// MSG_DONTWAIT - enable non-blocking behaviour
		// MSG_ERRQUEUE - this flag specifies that queued messages must be recvd from socket error queue.
			if ( ( rc = ::recv( _socket, bp, cnt, flags )) < 0 ){
				if ( errno == EINTR )	
					continue;
				debug_lib::throw_error(  "recvn failed, error %d,%s", 
						errno, strerror(errno) );
			}
			if ( rc == 0 )
				return (nbytes - cnt); //return short bytes
			
			cnt -= rc;
			bp += rc;
		}
		return rc;
	}

	int TCPSocket::readLine(char* bufptr, int len){
		char* bufx = bufptr;
		static char* bp;
		static int cnt = 0;
		static char b[4096];
		char c;
	
		while ( len-- > 0 ){
			if ( cnt-- <= 0 ){
				cnt = ::recv( _socket, b, sizeof(b), 0 );
				if ( cnt < 0 )
					debug_lib::throw_error( "error in read line" );
				if ( cnt == 0 )
					return 0;
				bp = b;
			}

			c = *bp++;
			*bufptr = c;
			//sockets_lib::log( "pointer %p", bufx );
			bufptr++;
			if ( c == '\n' ){
				while ( *bp == '\n' ){
					bp++;
					cnt--;
				}
				*bufptr = '\0';
				cnt--;
				//cnt = 0;
				return (bufptr - bufx);
			}

		}
		set_errno( EMSGSIZE );
		debug_lib::throw_error( "not enough space in buffer" );
		return -1;
	}

	int TCPSocket::writeLine( const char* buf, int size_of_buf){
		int rc = send_no_header(buf, size_of_buf);
		if ( strchr( buf, '\n' ) == NULL )
			rc += send_no_header("\n", 1);
		return rc;
	}

	int TCPSocket::send_no_header( const void* buf, size_t len ){
		//flags is set to NOSIGNAL to disable SIGPIPE specifically
		//? this might work only on Linux
		int flags = MSG_NOSIGNAL;
		int rc = -1;
		if ( (rc = ::send(_socket, buf, len, flags)) <= 0 )
			debug_lib::throw_error(  "send returned an error %d, %s", errno, strerror(errno) );
		return rc;
	}

	int TCPSocket::send_header( const void* buf, size_t len ){
		//flags is set to NOSIGNAL to disable SIGPIPE specifically
		//? this might work only on Linux
		int rc = -1;
		//sigset_t pipemask;
		//if ( sigemptyset(&pipemask) == -1 || 
			//sigaddset(&pipemask, SIGPIPE ) == -1 ){
			//sockets_lib::error( 1,  "signal initialization failed" );
		//}
		//if ( sigprocmask(SIG_BLOCK, &pipemask, NULL ) == -1 ){
			//sockets_lib::error( 1,  "signal SIGPIPE is blocked" );
		//}
		/* change to writev
		u_int32_t reclen = htonl(len);
		int flags = MSG_NOSIGNAL;
		if ( ( rc = ::send(_socket, &reclen, sizeof(u_int32_t), flags )) < 0 ){
			debug_lib::throw_error(  "send returned an error %d, %s", errno, strerror(errno) );
		}
		
		if ( (rc = ::send(_socket, buf, len, flags)) <= 0 )
			debug_lib::throw_error(  "send returned an error %d, %s", errno, strerror(errno) );
		*/

		u_int32_t reclen = htonl(len);
		iovec dataitems[2] = {0};
		dataitems[0].iov_base = &reclen;
		dataitems[0].iov_len = sizeof(u_int32_t);
		dataitems[1].iov_base = (void*)buf;
		dataitems[1].iov_len = len;
		rc = ::writev( _socket, dataitems, 2);
		if ( rc <= 0 )
			debug_lib::throw_error(  "send returned an error %d, %s", errno, strerror(errno) );

		//if ( sigprocmask(SIG_UNBLOCK, &pipemask, NULL ) == -1 ){
		//	sockets_lib::error( 1,  "signal SIGPIPE is blocked" );
		//}
		return rc;
	}
	
	//shutdown is synchronize the close with the end point. This will return an EOF for all processes that read fromt he socket and a SIGPIPE for a processes that write
	void TCPSocket::shutdown(int type){
		if ( ::shutdown( _socket, type ) )
			debug_lib::throw_error( "shutdown failed " );
	}

	//close is one side close of the socket the other side is still open
	void TCPSocket::close(){
		if ( ::close(_socket) )
			debug_lib::throw_error( "close failed" );
	}

	int TCPSocket::getFd(){ return _socket; }

	TCPSocket::~TCPSocket(){
		if ( ::close(_socket) )
			debug_lib::log( "close failed" );
	}

//TCPServerSocket defintiion

	void TCPServerSocket::bind( const char* hname, const char* sname ){
		_helper.set_address( hname, sname, &_local, "tcp" );
		if ( ::bind (_socket, (struct sockaddr*) &_local, sizeof(_local) ) ){
			debug_lib::throw_error( "bind failed, error %d, %s", errno, strerror(errno) );
		}
	} 

	void TCPServerSocket::listen(int backlog){
		if ( ::listen(_socket, backlog) ){
			debug_lib::throw_error( "listen failed, errno %d, %s", errno, strerror(errno) );
		}
	}

	TCPSocket* TCPServerSocket::accept(){
		_peerlen = sizeof(_peer);
		SOCKET s1 = ::accept(_socket, (struct sockaddr*) &_peer, &_peerlen );
		debug_lib::log ( "inside accept socket %d", getpid() );
		TCPSocket* accept_socket = new TCPAcceptSocket(s1);
		return accept_socket;
	}


//TCPClientSocket definition
	void TCPClientSocket::connect(const char* hname, const char* sname){
		_helper.set_address( hname, sname, &_peer, "tcp" );
		if ( ::connect( _socket, (struct sockaddr*) &_peer, sizeof(_peer) ) )
			debug_lib::throw_error(  "connect failed, errno %d, %s", errno, strerror(errno) );
	}


//SSL code
	void ssl_msg_callback( int write_p, int version, int content_type, const void* buf, size_t len, SSL* ssl, void* arg){
		debug_lib::log( "ssl_msg_callback: write_p = %d, version = %d, content_type = %d, size of msg = %d",
					write_p, version, content_type, len );
		debug_lib::lograw( (const unsigned char*) buf, len );
	}

	const char* SSLHelper::getSSLError(void)
	{ 
		BIO *bio = BIO_new (BIO_s_mem());
		ERR_print_errors(bio);
		char *buf = NULL;
		BIO_get_mem_data (bio, &buf);
		static char *ret = (char *) calloc (1,  1024);
		if (ret && buf )
			strcpy(ret, buf);
		BIO_free (bio);
		return ret;
        }

	void SSLHelper::initializeCtxt( bool isServer, std::string filePath, SSL_CTX** ssl_ctxt ){
		std::string certFile = filePath + "/mycert.pem";
		std::string privKeyFile = filePath + "/mykey.pem";
		std::string caFilePath = filePath + "/cacerts/";
		SSL_library_init(); //this is same as OpenSSL_add_all_algorithms which is a define of this function
		SSL_load_error_strings();
		if( isServer )
		    *ssl_ctxt = SSL_CTX_new( TLSv1_2_server_method() );	
		else
		    *ssl_ctxt = SSL_CTX_new( TLSv1_2_client_method() );	

		if( *ssl_ctxt == NULL ){
			debug_lib::throw_fatal_error( "cannot created ssl context %s", getSSLError() );
		}
		if ( SSL_CTX_use_certificate_file(*ssl_ctxt, certFile.c_str(), SSL_FILETYPE_PEM ) <= 0 ){
		    //error handling for ssl
		    debug_lib::throw_error( "error in use certificate file %s", certFile.c_str() );
		}
		if ( SSL_CTX_use_PrivateKey_file(*ssl_ctxt, privKeyFile.c_str(), SSL_FILETYPE_PEM ) <= 0 ){
		    //error handling for ssl
		    debug_lib::throw_error( "error in use private key file %s", privKeyFile.c_str() );
		}

		//In the caFilePath you should setup all certificates one in each file and use the c_rehash utility to
		//generate the links
		if ( SSL_CTX_load_verify_locations( *ssl_ctxt, NULL, caFilePath.c_str() ) <= 0 ){
		    //error handling for ssl
		    debug_lib::throw_error( "error loading verify locations" );
		}
		SSL_CTX_set_msg_callback( *ssl_ctxt, ssl_msg_callback );
		debug_lib::log( "initialize context complete" );
    	}


	SSL_CTX* SSLClientSocket::_ssl_ctxt = NULL;
	bool SSLClientSocket::_initialized = false;
	SSLClientSocket::SSLClientSocket(TCPClientSocket* clSocket, const char* certPath){
		debug_lib::log( "file descriptor = %d", clSocket->getFd() );
	        initialize( clSocket->getFd(), certPath );
        }

	SSLClientSocket::SSLClientSocket(SOCKET socket, const char* certPath){
	        initialize( socket, certPath );
        }

        int SSLClientSocket::write( const char* buf, int len ){
	        int err = SSL_write( _ssl, buf, len );
	        if ( err <= 0 ){
		        debug_lib::throw_error( "write failed due to err = %d, detailed error= %s", 
						err, SSLHelper::getSSLError() );
	        }
	        return err;
        }

        int SSLClientSocket::read( const char* buf, int size ){
	        int err = SSL_read( _ssl, (void*)buf, size );
	        if ( err < 0 ){
		        debug_lib::throw_error( "read failed due to err = %d", err );
	        }
	        if ( err == 0 ){
		        debug_lib::log( "peer has shutdown" );
		        return 0;
	        }
		debug_lib::log( "read is done %s", SSLHelper::getSSLError() );
	        return err;
        }

        void SSLClientSocket::shutdown(){
	        int err = SSL_shutdown(_ssl);
	        if ( err != 0 )
		        debug_lib::throw_error( "error during ssl shutdown" );
        }

        void SSLClientSocket::initialize(SOCKET socket, const char* certPath){
	        if ( !_initialized ){
		        SSLHelper::initializeCtxt( false, certPath, &_ssl_ctxt );
	        }
	        _initialized = true;
	        _ssl = SSL_new(_ssl_ctxt);
	        SSL_set_fd(_ssl, socket);
	        SSL_connect(_ssl);
		debug_lib::log( "ssl client initialize complete" );
        }


	//SSL server
	SSL_CTX* SSLServerSocket::_ssl_ctxt = NULL;
	bool SSLServerSocket::_initialized = false;
	SSLServerSocket::SSLServerSocket(TCPAcceptSocket* srvSocket, const char* certPath){
	        initialize( srvSocket->getFd(), certPath );
        }

        SSLServerSocket::SSLServerSocket(SOCKET socket, const char* certPath){
	        initialize( socket, certPath );
        }

        int SSLServerSocket::write( const char* buf, int len ){
	        int err = SSL_write( _ssl, buf, len );
	        if ( err <= 0 ){
		        debug_lib::throw_error( "write failed due to err = %d, detailed error = %s", 
					err, SSLHelper::getSSLError() );
	        }
		debug_lib::log( "write is done %s", SSLHelper::getSSLError() );
	        return err;
        }

        int SSLServerSocket::read( const char* buf, int size ){
	        int ret = SSL_read( _ssl, (void*)buf, size );
	        if ( ret == 0 ){
		        debug_lib::log( "peer has shutdown" );
		        return 0;
	        }else if ( ret < 0 ){
		        debug_lib::throw_error( "read failed due to err = %d", ret );
	        }
	        return ret;
        }

        void SSLServerSocket::shutdown(){
	        int err = SSL_shutdown(_ssl);
	        if ( err != 0 )
		        debug_lib::throw_error( "error during ssl shutdown" );
		debug_lib::log( "ssl shutdown performed" );
        }

        void SSLServerSocket::initialize(SOCKET socket, const char* certPath){
	        if( !_initialized ){
		        SSLHelper::initializeCtxt( true, certPath, &_ssl_ctxt);
	        }
	        _initialized = true;
	        _ssl = SSL_new(_ssl_ctxt);
	        SSL_set_fd(_ssl, socket);
	        SSL_accept(_ssl);
		debug_lib::log( "ssl server initialize complete" );
        }

	//SSL Bio implementation
	SSL_CTX* SSLBioImpl::_ssl_ctxt = NULL;
	bool SSLBioImpl::_initialized = false;
	SSLBioImpl::SSLBioImpl(bool isServer, const char* certPath){
	        initialize( isServer, certPath );
        }

	int SSLBioImpl::encodeSSL( const char* ibuf, int ilen, char* obuf, int osize ){
		debug_lib::log ( "inside encode = %d", ilen );
		sslWrite( ibuf, ilen );
		int ret = bioRead( _output_bio, obuf, osize );
		debug_lib::log( "encode ssl done" );
		return ret;
	}

	int SSLBioImpl::decodeSSL( const char* ibuf, int ilen, char* obuf, int osize ){
		debug_lib::log ( "inside decode = %d", ilen );
		int ret = bioWrite( _input_bio, ibuf, ilen );
		ret = sslRead( obuf, osize );	
		debug_lib::log( "decode ssl done" );
		return ret;
	}

        int SSLBioImpl::sslWrite( const char* buf, int len ){
	        int err = SSL_write( _ssl, buf, len );
	        if ( err < 0 ){
		        debug_lib::throw_error( "ssl write failed due to err = %d, detailed error= %s", 
						err, SSLHelper::getSSLError() );
	        }else if ( err == 0 ){
		        debug_lib::throw_error( "ssl write failed and detailed error= %s", 
						err, SSLHelper::getSSLError() );
		}
		debug_lib::log ( "finished ssl write" );
	        return err;
        }

        int SSLBioImpl::bioWrite( BIO* bio, const char* buf, int len ){
		debug_lib::log ( "inside biowrite = %d", len );
	        int err = BIO_write( bio, buf, len );
	        if ( err < 0 ){
		        debug_lib::throw_error( "bio write failed due to err = %d, detailed error= %s", 
						err, SSLHelper::getSSLError() );
	        }else if ( err == 0 ){
		        debug_lib::throw_error( "bio write failed and detailed error= %s", 
						err, SSLHelper::getSSLError() );
		}
	        return err;
        }

        int SSLBioImpl::sslRead( const char* buf, int size ){
		debug_lib::log ( "inside sslread = %d", size );
	        int err = SSL_read( _ssl, (void*)buf, size );
	        if ( err < 0 ){
		        debug_lib::throw_error( "ssl read failed due to err = %d, %s", err, SSLHelper::getSSLError() );
	        }
	        if ( err == 0 ){
		        debug_lib::log( "peer has shutdown" );
		        return 0;
	        }
		debug_lib::log( "read is done %s", SSLHelper::getSSLError() );
	        return err;
        }

        int SSLBioImpl::bioRead( BIO* bio, const char* buf, int size ){
		debug_lib::log ( "inside bioread= %d", size );
	        int err = BIO_read( bio, (void*)buf, size );
	        if ( err < 0 ){
		        debug_lib::throw_error( "bio read failed due to err = %d", err );
	        }
	        if ( err == 0 ){
		        debug_lib::log( "peer has shutdown" );
		        return 0;
	        }
		debug_lib::log( "read is done %s", SSLHelper::getSSLError() );
	        return err;
        }

        void SSLBioImpl::shutdown(){
	        int err = SSL_shutdown(_ssl);
	        if ( err != 0 )
		        debug_lib::throw_error( "error during ssl shutdown" );
        }

	void SSLBioImpl::doHandshake( TCPSocket* socket, bool isServer ){
		int len, pending;
		char buf[16384] = {0};
		while(!SSL_is_init_finished(_ssl)) {
			if ( isServer ){
				//wait for client hello. Client always sends the first msg in handshake
				len = socket->recv_header( buf, sizeof(buf) );
				debug_lib::log( "recv on socket %d", len );
				if ( len > 0 ){
					debug_lib::log( "bio write" );
					//write the client hello to the memory bio to be processed by openssl
					len = bioWrite( _input_bio, buf, len );
				}
				//ssl handshake based on the current state of the handshake
				SSL_do_handshake(_ssl);
				debug_lib::log( "do ssl handshake" );
				
				pending = BIO_ctrl_pending( _output_bio );
				if ( pending > 0 ){
					debug_lib::log( "bio read pending" );
					//read encrypted message put out by ssl_handshake in the output bio
					len = bioRead( _output_bio, buf, sizeof(buf) );
					if ( len > 0 ){
						debug_lib::log( "send len = %d on socket", len );
						//send the encrypted message on the socket
						socket->send_header( buf, len );
					}
				}
			}else{ //client
				//client initiates the handshake
				SSL_do_handshake(_ssl);
				debug_lib::log( "do handshake" );
				//client recvs the final message of the handshake - the 'Server Finished'. So this check is required
				if ( SSL_is_init_finished(_ssl)) {
					debug_lib::log( "done handshake" );
					continue;
				}
				pending = BIO_ctrl_pending( _output_bio );
				if ( pending > 0 ){
					debug_lib::log( "pending bio read" );
					//read encrypted message put out by ssl_handshake in the output bio
					len = bioRead( _output_bio, buf, sizeof(buf) );
					//send the encrypted message on the socket
					if ( len > 0 ){
						debug_lib::log( "send on socket" );
						socket->send_header( buf, len );
					}
				}

				//wait for response on the socket
				len = socket->recv_header( buf, sizeof(buf) );
				debug_lib::log( "recv on socket, len = %d", len );
				if ( len > 0 ){
					debug_lib::log( "do bio write" );
					//write encrypted message on to input bio
					bioWrite( _input_bio, buf, len );
				}
			}
		}
		debug_lib::log( "ssl hand shake is done for %s", isServer? "server": "client" );
	}

        void SSLBioImpl::initialize(bool isServer, const char* certPath ){
	        if ( !_initialized ){
		        SSLHelper::initializeCtxt( isServer, certPath, &_ssl_ctxt );
	        }
	        _initialized = true;
	        _ssl = SSL_new(_ssl_ctxt);
		_input_bio = BIO_new(BIO_s_mem());
		_output_bio = BIO_new(BIO_s_mem());
		SSL_set_bio( _ssl, _input_bio, _output_bio );
		if( isServer ){
			SSL_set_accept_state(_ssl);
		}
		else{
			SSL_set_connect_state(_ssl);
		}
		debug_lib::log( "ssl bio initialize complete" );
        }

	SSLBioImpl::~SSLBioImpl(){
		//ERR_remove_state(0);
		//ENGINE_cleanup();
		//CONF_modules_unload(1);
		//ERR_free_strings();
		//EVP_cleanup();
		//sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
		//CRYPTO_cleanup_all_ex_data();
	}


	//non blocking sockets
	TCPSocketNoWait::TCPSocketNoWait(SOCKET s, MessageFramer* framer) {
		fcntl(s, F_SETFL, O_NONBLOCK);  // set to non-blocking
		_current_recv_msg = new Message(framer);
		_current_send_msg = new Message(framer);
		_current_recv_msg->reset();
		_current_send_msg->reset();
		_socket = s;
	}

	TCPSocketNoWait::~TCPSocketNoWait(){
		if ( _current_send_msg ) delete _current_send_msg;
		if ( _current_recv_msg ) delete _current_recv_msg;
	}

	//returns -1 for disconnect
	//returns 0 for no data
	//returns n for data
	int TCPSocketNoWait::recv( Message& msg ){
		//this needs to be cleaned up. What is the limit of the buffer size.
		static char recvbuf[32768];
		int recvbufsize = sizeof(recvbuf);
		memset( recvbuf, 0x00, recvbufsize );
		int flags = 0;
		int rc = 0;
		while( true ){ //while loop is only to check for EINTR
			if ( ( rc = ::recv( _socket, recvbuf, recvbufsize, flags )) < 0 ){
				if ( errno == EAGAIN || errno == EWOULDBLOCK ){
					break;
				}
				if ( errno == EINTR )	
					continue;
				debug_lib::throw_error(  "recvn failed, error %d,%s", 
						errno, strerror(errno) );
			}else
				break;
		}
		debug_lib::log( "nowait recv: return code of rc = %d", rc );

		if( rc == 0 ){
			debug_lib::log( "nowait recv: socket disconnected" );	
			return -1;
		}

		if ( rc > 0 ){
			int len_used; //Need to fix this
			_current_recv_msg->putData( recvbuf, rc, len_used );
			debug_lib::log( "received rc %d bytes, len used %d", rc, len_used );
			while( _current_recv_msg->isReady() ){
				_recv_q.push(*_current_recv_msg);
				if ( len_used < rc ){
					_current_recv_msg->reset();
					rc = _current_recv_msg->putData( recvbuf+len_used, rc-len_used, len_used);
				}else
					break;
			}
		}

		rc = 0;
		if( !_recv_q.empty() ){
			msg = _recv_q.front();
			rc = msg.size();
			_recv_q.pop();
		}
		return rc;
	}

	int TCPSocketNoWait::send( Message& msg ){
		static char sendbuf[32768];
		int sendbufsize = sizeof(sendbuf);
		int rc = -1;
		int flags = 0;
		int len;
		if ( !msg.isReady() ){
			debug_lib::throw_error( "no wait send: message not ready to be sent" );
		}
		debug_lib::log( "sending message msg.raw %d, msg.size %d", msg.isRaw(), msg.size() );

		_send_q.push(msg);
		bool canSendMore = true;
		Message& qmsg = _send_q.front();
		while(canSendMore){
			len = qmsg.moveToBuffer( sendbuf, sendbufsize );	
			debug_lib::log( "message is raw %d, len is %d", qmsg.isRaw(), len );
			if ( (rc = ::send(_socket, sendbuf, len, flags)) < 0 ){
				if ( errno == EAGAIN || errno == EWOULDBLOCK ){
					canSendMore = false;
					continue;
				}else
					debug_lib::throw_error(  "no wait send returned error %d", errno );
			}
			debug_lib::log( "message sent, len %d, return %d", len, rc );
			debug_lib::log( "pop message" );
			_send_q.pop();
			if( _send_q.empty() )
				canSendMore = false;
			else
				qmsg = _send_q.front();
		}
		return 0;
	}

	//Message
         unsigned int Message::putDataRaw(const char* buf, size_t len){
		static char obuf[16384] = {0};
		size_t olen = sizeof(obuf)-1;
		
		size_t copylen;
		const char* startaddr;
		if( _framer->form( buf, len, (char*) obuf, olen ) ){
			startaddr = obuf;
			copylen = olen;
		}
		else{
			startaddr = buf;
			copylen = len;
		}

		for( unsigned int i = 0; i < copylen; i++ ){
			_msg.push_back( *(startaddr+i) );
		}
                _raw = true;
                _ready = true;
		return len;
  	 }

         unsigned int Message::putData(const char* buf, size_t len, int& ret_len_parsed){
                 if ( _ready ){
                         debug_lib::throw_error( "logic error, cannot add any more data" );
                 }
                 unsigned int len_to_copy = 0;
                 unsigned int offset_to_use = 0;
                 unsigned int len_parsed = 0;
                 bool endOfMessage = false;
                 _framer->frame( buf, len, len_to_copy, offset_to_use, len_parsed, endOfMessage);
		 debug_lib::log( "framer passed len %d , returned len_to_copy %d, offset_to_use %d, len_parsed %d, endOfMessage %d",
					len, len_to_copy, offset_to_use, len_parsed, endOfMessage );
                 if( len_to_copy > 0 ){
                         const char* startaddr = buf + offset_to_use;
                         for( unsigned int i = 0; i < len_to_copy; i++ ){
                                 _msg.push_back( *(startaddr+i) );
                         }
                 }
                 if( endOfMessage )
                         setReady();
                 ret_len_parsed = len_parsed;
                 return len_parsed;
         }

         int Message::moveToBuffer( char* buf, size_t size ){
                 unsigned int rc = _msg.size();
                 if( rc > size )
                         debug_lib::throw_error( "logic error: size of buffer less than message size" );
                 for(unsigned int i = 0; i < _msg.size(); i++ ){
                         buf[i] = _msg[i];
                 }
                 //to be fixed! std::copy( buf, _msg.begin(), _msg.end() );
                 return rc;
         }

         bool Message::isReady(){ return _ready; }
         bool Message::isRaw(){ return _raw; }
         unsigned int Message::size(){ return _msg.size(); }

         void Message::reset(){
                 _ready = false;
                 _raw = false;
                 _msg.clear();
         }
         void Message::setReady(bool ready){ _ready = ready; }


	//message framers
	void NewLineFramer::frame( const char* buf, size_t len, unsigned int& len_to_copy, 
			unsigned int& offset_to_use, unsigned int& len_parsed, bool& endOfMessage ){
		unsigned int i;
		offset_to_use = 0;
		for( i = 0; i < len; i++ ){
			if( buf[i] == '\n' ){
				debug_lib::log( "NewLineFramer encountered new line" );
				len_to_copy = i;
				len_parsed = i+1;
				endOfMessage = true;
				return;
			}
		}
		len_to_copy = len;
		len_parsed = len;
		endOfMessage = false;
		return;
	}
	
	bool NewLineFramer::form( const char* buf, size_t len, char* obuf, size_t& olen ){
		return false;
	}

	bool FixedLenFramer::form( const char* buf, size_t len, char* obuf, size_t& olen ){
		return false;
	}

        void FixedLenFramer::frame( const char* buf, size_t len, unsigned int& len_to_copy,
                        unsigned int& offset_to_use, unsigned int& len_parsed, bool& endOfMessage ){
                offset_to_use = 0;
                unsigned int i;
                for( i = 0; i < len; i++ ){
                        _running_len++;
                        if( _running_len == _len ){
                                _running_len = 0;
                                len_to_copy = i+1;
                                len_parsed = i+1;
                                endOfMessage = true;
                                return;
                        }
                }
                len_to_copy = len;
                len_parsed = len;
                endOfMessage = false;
                return;
        }


	bool Byte2HeaderFramer::form( const char* buf, size_t len, char* obuf, size_t& olen ){
		if( olen < len + 2 )
			debug_lib::throw_error( "Byte2HeaderFramer::form: logic error: length in buffer not sufficient" ); 
		memset( obuf, 0x00, olen );
		char* ptr = obuf;
		ptr += sprintf( obuf, "%02ld", len );
		memcpy( ptr, buf, len );
		olen = len + 2;
		return true;
	}

        void Byte2HeaderFramer::frame( const char* buf, size_t len, unsigned int& len_to_copy,
                        unsigned int& offset_to_use, unsigned int& len_parsed, bool& endOfMessage ){
                unsigned int i;
                offset_to_use = 0;
                if( _header_len < 2 ){
                        for( i = 0; i < 2; i++ ){
                                _header[_header_len] = buf[i];
                                _header_len++;
                                offset_to_use++;
                                if( _header_len == 2 ){
                                        _header_len = 2;
                                        _header[2] = '\0';
                                        _len = atoi(_header);
					debug_lib::log( "header found %s", _header );
                                        _running_len = 0;
					i = i+1;
                                        break;
                                }
                        }
                }

                if ( _len == 0 ){
                        len_to_copy = 0;
                        len_parsed = i+1;
                        endOfMessage = false;
                        return;
                }

                for( ; i < len; i++ ){
                        _running_len++;
                        if( _running_len == _len ){
                                len_to_copy = _len;
                                len_parsed = i+1;
                                endOfMessage = true;
                                return;
                        }
                }

                len_to_copy = len;      
                len_parsed = len;       
                endOfMessage = false;   
                return; 
        } 



}
