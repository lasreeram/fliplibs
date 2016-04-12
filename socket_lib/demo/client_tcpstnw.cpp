#include <Socket.h>
#include <iostream>
#include "nowait_struct.h"
using namespace sockets_lib;

void print_help(){
	std::cout << "client <hostname> <port>" << std::endl;
}

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

	try{
		TCPClientSocket* sock = new TCPClientSocket();
		sock->connect(hname, sname);
		MessageFramer* framer = new FixedLenFramer(sizeof(struct nowait_struct));
		TCPClientSocketNoWait* socknw = new TCPClientSocketNoWait(sock->getFd(), framer);
		Message msg;
		struct nowait_struct stmsg;
		memset( &stmsg, 0x00, sizeof(stmsg) );
		msg.setFramer(framer);
		debug_lib::log( "calling socknw recv" );
		int rc = 0;
		bool isDone = false;
		while( !isDone ){
			rc = socknw->recv(msg);
			if( rc > 0 ){
				isDone = true;
				continue;
			}
			debug_lib::log("sleeping for 10 secs");
			sleep(10);
		}
		
		msg.moveToBuffer( (char*)&stmsg, sizeof(struct nowait_struct) );
		debug_lib::log( "received string: %s", stmsg.str );
		
		delete framer;
		delete sock;
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error: %s", e.what());
		exit(1);
	}
	return 0;
}
