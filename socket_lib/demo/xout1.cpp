#include "Socket.h"
#include <iostream>
using namespace sockets_lib;

void print_help(){
	std::cout << "xout <hostname> <port>" << std::endl;
}
bool use_close = false;

class StdInReadHandler : public IOPollHandler {
	public:
	StdInReadHandler(TCPSocket* socket){_socket = socket;}
	void handle( IOPollManager* mgr ){
		if ( fgets( lout, sizeof(lout), stdin ) == NULL ){
			mgr->removeFromReadSet(0);
			_socket->close();
		}else{
			_socket->writeLine(lout, strlen(lout));
		}
		
		return;
	}
	private:
		TCPSocket* _socket;
		char lout[1024];
};

class SocketReadHandler : public IOPollHandler {
	public:
	SocketReadHandler(TCPSocket* socket) { _socket = socket; }
	void handle ( IOPollManager* mgr ){
		//Note that this read is just to receive any error on the socket
		//that is why the else throws an un-expected message error.
		//the readLine function throws an exception on a fatal error and returns 0 for EOF.
		rc = _socket->readLine( buf, sizeof(buf)-1 );
		//rc < 0 readLine throws exception
		if ( rc == 0 )
			sockets_lib::throw_error( "server disconnected" );
		else{ //rc > 0
			buf[rc] = '\0';
			sockets_lib::throw_error( "unexpected message from server:%s\n", buf );
		}
		return;
	}
	private:
		TCPSocket* _socket;
		char buf[2048];
		int rc;
};

int main(int argc, char** argv){
	char* hname;
	char* sname;
	INIT();



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
		SocketReadHandler* sockReadHandler = new SocketReadHandler(sock);
		StdInReadHandler* stdinReadHandler = new StdInReadHandler(sock);
		pollMgr->addSourceForRead( sock->getFd(), sockReadHandler );
		pollMgr->addSourceForRead( 0, stdinReadHandler );
		pollMgr->pollAndHandleEvents();
		delete pollMgr;
		delete sockReadHandler;
		delete stdinReadHandler;
		
	}catch(SocketException& e){
		sockets_lib::log(  "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
