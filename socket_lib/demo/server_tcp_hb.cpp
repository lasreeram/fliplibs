#include <Socket.h>
#include "heartbeat.h"
using namespace sockets_lib;

//Server with heart beat message
void print_help(){
	sockets_lib::log(  "server <hostname> <port>" );
}
int _missed_heart_beats = 0;

class IOReadHandler : public IOPollHandler {
        public:
        IOReadHandler(TCPSocket* socket) { _socket = socket; }
        void handle ( IOPollManager* mgr ){
		sockets_lib::log ( "read a message" );
		msg_t msg = {0};
		int rc = _socket->recvn(&msg, sizeof(msg_t));
		if ( rc == 0 )
			sockets_lib::throw_error( "EOF received" );
		
		_missed_heart_beats = 0;
		mgr->resetTimer( T1+T2, 0 );
		int msgtype = ntohl(msg.type);
		switch(msgtype){
			case MSG_HEARTBEAT:
				_socket->send_no_header( (char*)&msg, sizeof(msg_t) );
				break;
			default:
				sockets_lib::throw_error( "unknown message type received %d", 
					msgtype );
				break;
		}
		sockets_lib::log ( "read handle done" );
                return;
        }
        private:
                TCPSocket* _socket;
};

class IOTimeoutHandler : public IOPollHandler {
        public:
        IOTimeoutHandler(TCPSocket* socket) { 
		_socket = socket; 
		_missed_heart_beats = 0;
	}
        void handle ( IOPollManager* mgr ){
		sockets_lib::log ( "timeout happened" );
		_missed_heart_beats++;
		if ( _missed_heart_beats > 3 )
			sockets_lib::throw_error( "connection is dead. No heart beat" );
		sockets_lib::log ( "missed heart beat" );
		mgr->resetTimer( T2, 0 );
		sockets_lib::log ( "timeout handle done" );
                return;
        }
        private:
                TCPSocket* _socket;
};

void service(TCPSocket* sock){
        IOPollManager* pollMgr = new IOPollManager();
        IOReadHandler* readHandler = new IOReadHandler(sock);
        IOTimeoutHandler* timeoutHandler = new IOTimeoutHandler(sock);

	pollMgr->addSourceForRead( sock->getFd(), readHandler );
	pollMgr->resetTimer( T1+T2, 0 );
	pollMgr->setTimeoutHandler( timeoutHandler );
	pollMgr->pollAndHandleEvents();
	delete readHandler;
	delete timeoutHandler;
	delete pollMgr;
	
	return;
}

int main(int argc, char** argv){
	char* hname;
	char* sname;
	INIT();

	if ( argc == 2 ){
		hname = NULL;
		sname = argv[1];
	}else if ( argc == 3 ){
		hname = argv[1];
		sname = argv[2];
	}else{
		sockets_lib::log( "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	try{
		TCPServerSocket* sock = new TCPServerSocket();
		sock->bind(hname, sname);
		sock->listen();

		do{
			TCPSocket* acceptsock = sock->accept();
			service(acceptsock);
			//delete only closes the socket from the server side. The socket is still open on the client side.
			delete acceptsock;
		} while(0);

	}catch(sockets_lib::SocketException& e){
		sockets_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}catch(...){
		sockets_lib::log( "exit due to error in server: %s", "unknown error" );
		exit(1);
	}
	return 0;
}
