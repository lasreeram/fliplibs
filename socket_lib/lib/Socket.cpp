#include <Socket.h>

namespace sockets_lib{

	char* program_name;
	bool use_syslog = false;
	bool debug_off = false;

	void logDebugInSyslog(){
		use_syslog = true;
	}

	void logDebugInStdOut(){
		use_syslog = false;
	}	

	void throw_fatal_error( const char* format, ... ){
		char buf[2048] = {0};
		va_list ap;
		va_start(ap, format);
		vsprintf(buf, format, ap);
		if ( use_syslog ){
			syslog(LOG_USER|LOG_CRIT, "%s", buf );
		}else{
			if ( errno )
				printf("%s: error type = %d, errno = %d(%s), error = %s\n", 
						program_name, LOG_CRIT, errno, strerror(errno), buf );
			else
				printf("%s: error type = %d, error = %s\n", program_name, LOG_CRIT, buf );
		}
		//vsyslog(LOG_USER|err, format, ap );
		va_end(ap);
		SocketException ex(buf);
		throw ex;
	}

	void throw_error( const char* format, ... ){
		char buf[2048] = {0};
		va_list ap;
		va_start(ap, format);
		vsprintf(buf, format, ap);
		if ( use_syslog ){
			syslog(LOG_USER|LOG_ERR, "%s", buf );
		}else{
			if ( errno )
				printf("%s: error type = %d, errno = %d(%s), error = %s\n", 
							program_name, LOG_ERR, errno, strerror(errno), buf );
			else
				printf("%s: error type = %d, error = %s\n", program_name, LOG_ERR, buf );
		}
		//vsyslog(LOG_USER|err, format, ap );
		va_end(ap);
		SocketException ex(buf);
		throw ex;
	}

	void log( const char* format, ... ){
		if ( debug_off )
			return;
		char buf[2048] = {0};
		va_list ap;
		va_start(ap, format);
		vsprintf(buf, format, ap);
		if ( use_syslog )
			syslog(LOG_USER|LOG_INFO, "%s", buf );
		else
			printf("%s: %s\n", program_name, buf );
		//vsyslog(LOG_USER|err, format, ap );
		va_end(ap);
	}

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
				sockets_lib::throw_error(  "select failed %d", errno );
			if ( rc == 0 ){
				if( _timeoutHandler ){
					_timeoutHandler->handle(this);
				}else
					sockets_lib::throw_error(  "timeout handler not configured" );
				
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
			sockets_lib::throw_error( "clock get time returned error" );

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
			sockets_lib::throw_error( "no timer to renew - logic error in application" );
		
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
				sockets_lib::throw_error( "clock_gettime failed" );
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
				sockets_lib::throw_error( "pollAndHandleEvents called without any fds to check" );
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
				sockets_lib::throw_error(  "select failed %d", errno );
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
					sockets_lib::throw_error( "unknown host %s\n", hname );
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
				sockets_lib::throw_error(  "unknown service: %s\n", sname );
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
			sockets_lib::throw_error(  "udp socket create failed, error %d,%s", 
						errno, strerror(errno) );

		//peerlen must be set to sizeof peer.
		_peerlen = sizeof(_peer);
		//no reuse address set option for udp.  
	}

	UDPSocket::UDPSocket(int socket){
		_socket = socket;
		if ( !isvalidsock(_socket) )
			sockets_lib::throw_error(  "udp socket create failed, error %d,%s", 
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
			sockets_lib::throw_error(  "udp recvfrom failed, error %d,%s", 
						errno, strerror(errno) );
		}
		return rc;
	}

	//in udp client calls sendto first and recvfrom later. Server calls recvfrom first and send to later
	//in tcp either side can initiate the conversation
	int UDPSocket::sendTo( const void* buf, int buflen){
		int rc = -1;
		if ( (rc = ::sendto( _socket, buf, buflen, 0, (struct sockaddr*) &_peer, _peerlen )) < 0 )
			sockets_lib::throw_error(  "udp sendto failed, error %d,%s", 
						errno, strerror(errno) );
		return rc;
	}

	void UDPSocket::setSocketOption(int option_value, int on ){
		if ( setsockopt( _socket, SOL_SOCKET, option_value, &on, sizeof(on) ) ){
			sockets_lib::throw_error( "setsockopt failed, error %d, %s", errno, strerror(errno) );
		}	
		return;
	}

	int UDPSocket::getFd(){ return _socket; }


//UDPServerSocket defintion
	void UDPServerSocket::bind( const char* hname, const char* sname ){
		_helper.set_address( hname, sname, &_local, "udp" );
		if ( ::bind (_socket, (struct sockaddr*) &_local, sizeof(_local) ) ){
			sockets_lib::throw_error( "bind failed, error %d, %s", errno, strerror(errno) );
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
			sockets_lib::throw_error( "connect failed, error %d, %s", errno, strerror(errno) );
	}

	void UDPClientSocket::connect( struct sockaddr_in* peer ){
		int len = sizeof(struct sockaddr_in);
		memcpy( &_peer, peer, len );
		_peerlen = len;
		if( ::connect( _socket, (struct sockaddr*) peer, len ) <  0 )
			sockets_lib::throw_error( "connect failed, error %d, %s", errno, strerror(errno) );
	}


//TCPSocket definition

	TCPSocket::TCPSocket(){
		// last argument is protocol. it is used only when SOCK_RAW is used. ICMP for example
		_socket = socket( AF_INET, SOCK_STREAM, 0 );
		if ( !isvalidsock(_socket) )
			sockets_lib::throw_error(  "socket create failed, error %d,%s", errno, strerror(errno) );
		setSocketOption(SO_REUSEADDR);
		//signal(SIGPIPE, SIG_IGN);
	}

	TCPSocket::TCPSocket(SOCKET s){
		_socket = s;
		if ( !isvalidsock(_socket) )
			sockets_lib::throw_error(  "socket create failed, error %d,%s", errno, strerror(errno) );
	}

	void TCPSocket::setSocketOption(int option_value, int on ){
		if ( setsockopt( _socket, SOL_SOCKET, option_value, &on, sizeof(on) ) ){
			sockets_lib::throw_error(  "setsockopt failed, error %d, %s", errno, strerror(errno) );
		}	
		return;
	}

	int TCPSocket::recv_no_header( void* buf, size_t size){
		int flags = 0;
		int rc = 0;
		while( true ){
			if ( ( rc = ::recv( _socket, buf, size, flags )) < 0 ){
				if ( errno == EINTR )	
					continue;
				sockets_lib::throw_error(  "recvn failed, error %d,%s", 
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
					sockets_lib::throw_error( "recv failed, while discarding message. error %d, %s", 
						errno, strerror(errno) );
				}
				reclen -= len;
				if ( reclen < len )
					len = reclen;
			}
			set_errno( EMSGSIZE );
			sockets_lib::throw_error(  "logic error, space not enough, message discarded.%d, %s", 
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
				sockets_lib::throw_error(  "recvn failed, error %d,%s", 
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
					sockets_lib::throw_error( "error in read line" );
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
		sockets_lib::throw_error( "not enough space in buffer" );
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
			sockets_lib::throw_error(  "send returned an error %d, %s", errno, strerror(errno) );
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
			sockets_lib::throw_error(  "send returned an error %d, %s", errno, strerror(errno) );
		}
		
		if ( (rc = ::send(_socket, buf, len, flags)) <= 0 )
			sockets_lib::throw_error(  "send returned an error %d, %s", errno, strerror(errno) );
		*/

		u_int32_t reclen = htonl(len);
		iovec dataitems[2] = {0};
		dataitems[0].iov_base = &reclen;
		dataitems[0].iov_len = sizeof(u_int32_t);
		dataitems[1].iov_base = (void*)buf;
		dataitems[1].iov_len = len;
		rc = ::writev( _socket, dataitems, 2);
		if ( rc <= 0 )
			sockets_lib::throw_error(  "send returned an error %d, %s", errno, strerror(errno) );

		//if ( sigprocmask(SIG_UNBLOCK, &pipemask, NULL ) == -1 ){
		//	sockets_lib::error( 1,  "signal SIGPIPE is blocked" );
		//}
		return rc;
	}
	
	void TCPSocket::shutdown(int type){
		if ( ::shutdown( _socket, type ) )
			sockets_lib::throw_error( "shutdown failed " );
	}

	void TCPSocket::close(){
		if ( ::close(_socket) )
			sockets_lib::throw_error( "close failed" );
	}

	int TCPSocket::getFd(){ return _socket; }

	TCPSocket::~TCPSocket(){
		if ( ::close(_socket) )
			sockets_lib::log( "close failed" );
	}

//TCPServerSocket defintiion

	void TCPServerSocket::bind( const char* hname, const char* sname ){
		_helper.set_address( hname, sname, &_local, "tcp" );
		if ( ::bind (_socket, (struct sockaddr*) &_local, sizeof(_local) ) ){
			sockets_lib::throw_error( "bind failed, error %d, %s", errno, strerror(errno) );
		}
	} 

	void TCPServerSocket::listen(int backlog){
		if ( ::listen(_socket, backlog) ){
			sockets_lib::throw_error( "listen failed, errno %d, %s", errno, strerror(errno) );
		}
	}

	TCPSocket* TCPServerSocket::accept(){
		SOCKET s1 = ::accept(_socket, (struct sockaddr*) &_peer, &_peerlen );
		sockets_lib::log ( "inside accept socket %d", getpid() );
		TCPSocket* accept_socket = new TCPAcceptSocket(s1);
		return accept_socket;
	}


//TCPClientSocket definition
	void TCPClientSocket::connect(const char* hname, const char* sname){
		_helper.set_address( hname, sname, &_peer, "tcp" );
		if ( ::connect( _socket, (struct sockaddr*) &_peer, sizeof(_peer) ) )
			sockets_lib::throw_error(  "connect failed, errno %d, %s", errno, strerror(errno) );
	}


}
