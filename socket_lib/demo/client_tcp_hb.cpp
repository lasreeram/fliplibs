#include <Socket.h>
#include <iostream>
#include "heartbeat.h"
using namespace sockets_lib;

// Heart beat client
void print_help(){
	std::cout << "client <hostname> <port>" << std::endl;
}

int _missed_heart_beats = 0;

class IOReadHandler : public IOPollHandler {
	public:
	IOReadHandler(TCPSocket* socket) { _socket = socket; }
	void handle ( IOPollManager* mgr ){
		msg_t msg = {0};
		debug_lib::log( "read a heartbeat" );
		int rc = _socket->recvn( (char*)&msg, sizeof(msg_t) );
		if ( rc == 0 )
			debug_lib::throw_error( "EOF received on socket" );
		_missed_heart_beats = 0;
		mgr->resetTimer( T1, 0 );
		debug_lib::log( "read handler done" );
		return;
	}
	private:
		TCPSocket* _socket;
};

class IOTimeoutHandler : public IOPollHandler {
	public:
	IOTimeoutHandler(TCPSocket* socket) { _socket = socket; }
        void handle ( IOPollManager* mgr ){
		debug_lib::log( "timeout occurred" );
                _missed_heart_beats++;
                if ( _missed_heart_beats > 3 )
                        debug_lib::throw_error( "connection is dead. No heart beat" );
                debug_lib::log ( "sending heart beat message" );
                msg_t msg = {0};
                msg.type = htonl( MSG_HEARTBEAT );
                _socket->send_no_header( (char*)&msg, sizeof(msg) );
                mgr->resetTimer( T2, 0 );
		debug_lib::log( "timeout handle done" );
                return;
        }
	private:
		TCPSocket* _socket;
};

int main(int argc, char** argv){
	char* hname;
	char* sname;
	debug_lib::init(argv[0]);

	if ( argc == 3 ){
		hname = argv[1];
		sname = argv[2];
	}else{
		debug_lib::log(  "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	TCPClientSocket* sock = new TCPClientSocket();
	IOPollManager* pollMgr = new IOPollManager();
	try{
		sock->connect(hname, sname);
		IOReadHandler* readHandler = new IOReadHandler(sock);
		IOTimeoutHandler* timeoutHandler = new IOTimeoutHandler(sock);
		pollMgr->addSourceForRead( sock->getFd(), readHandler );
		pollMgr->resetTimer( T1, 0 );
		pollMgr->setTimeoutHandler( timeoutHandler );
		pollMgr->pollAndHandleEvents();
		delete readHandler;
		delete timeoutHandler;
		delete sock;
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
