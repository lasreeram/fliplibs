#include <Socket.h>
#include <iostream>
#define ACK_TIME 10
#define ACK 0x6
using namespace sockets_lib;

void print_help(){
	std::cout << "xout <hostname> <port>" << std::endl;
}
bool use_close = false;

class IOTimeoutHandler : public IOPollHandler {
	public:
	IOTimeoutHandler(TCPSocket* socket) { _socket = socket; }
        void handle ( IOPollManager* mgr ){
		debug_lib::log( "timeout occurred" );
                debug_lib::throw_error( "terminating" );
                return;
        }
	private:
		TCPSocket* _socket;
};

class StdInReadHandler : public IOPollHandler {
	public:
	StdInReadHandler(TCPSocket* socket){_socket = socket;}
	void handle( IOPollManager* mgr ){
		if ( fgets( lout, sizeof(lout), stdin ) == NULL ){
			mgr->removeFromReadSet(0);
			_socket->close();
			debug_lib::throw_error( "fgets returns null" );
		}else{
			debug_lib::log( "sending %s", lout );
			_socket->writeLine(lout, strlen(lout));
			mgr->removeFromReadSet(0); //until ACK is recvd
			mgr->resetTimer(ACK_TIME, 0);
		}
		
		return;
	}
	private:
		TCPSocket* _socket;
		char lout[1024];
};

class SocketReadHandler : public IOPollHandler {
	public:
	SocketReadHandler(TCPSocket* socket, IOPollHandler* stdinHandler) { 
		_socket = socket; 
		_stdinHandler = stdinHandler;
	}
	void handle ( IOPollManager* mgr ){
		//Note that this read is just to receive any error on the socket
		//that is why the else throws an un-expected message error.
		//the readLine function throws an exception on a fatal error and returns 0 for EOF.
		rc = _socket->readLine( buf, sizeof(buf)-1 );
		//rc < 0 readLine throws exception
		if ( rc == 0 )
			debug_lib::throw_error( "server disconnected" );
		else if ( rc == 2 && buf[0] == ACK ){
			debug_lib::log( "ack received" );
			mgr->addSourceForRead( 0, _stdinHandler);
			mgr->resetTimer(0, 0);
		}
		else{ //rc > 0
			buf[rc] = '\0';
			debug_lib::throw_error( "unexpected message from server:%x, %x\n", buf[0], buf[1] );
		}
		return;
	}
	private:
		TCPSocket* _socket;
		IOPollHandler* _stdinHandler;
		char buf[2048];
		int rc;
};

int main(int argc, char** argv){
	char* hname;
	char* sname;
	debug_lib::init(argv[0]);



	if ( argc == 3 ){
		hname = argv[1];
		sname = argv[2];
	} else {
		print_help();
		exit(1);
	}

	try{
		TCPClientSocket* sock = new TCPClientSocket();
		sock->connect(hname, sname);
		IOPollManager* pollMgr = new IOPollManager();
		StdInReadHandler* stdinReadHandler = new StdInReadHandler(sock);
		SocketReadHandler* sockReadHandler = new SocketReadHandler(sock, stdinReadHandler);
		pollMgr->addSourceForRead( sock->getFd(), sockReadHandler );
		pollMgr->addSourceForRead( 0, stdinReadHandler );
		IOTimeoutHandler* timeoutHandler = new IOTimeoutHandler(sock);
		pollMgr->setTimeoutHandler(timeoutHandler);
		pollMgr->pollAndHandleEvents();
		delete pollMgr;
		delete sockReadHandler;
		delete stdinReadHandler;
		
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
