#include <Socket.h>
#include <debug.h>
#include "nowait_struct.h"
using namespace sockets_lib;

void service(TCPSocket* sock){
	char buf[] = "hello world\n";
	sock->send_header( buf, strlen(buf));
	return;
}

void print_help(){
	debug_lib::log(  "server <hostname> <port>" );
}

int main(int argc, char** argv){
	char* hname;
	char* sname;
	debug_lib::init(argv[0]);

	if ( argc == 2 ){
		hname = NULL;
		sname = argv[1];
	}else if ( argc == 3 ){
		hname = argv[1];
		sname = argv[2];
	}else{
		debug_lib::log( "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	try{
		TCPServerSocket* sock = new TCPServerSocket();
		sock->bind(hname, sname);
		sock->listen();

		do{
			while( true ){
				TCPSocket* acceptsock = sock->accept();
				MessageFramer* framer = new FixedLenFramer(sizeof(nowait_struct));
				TCPAcceptSocketNoWait* acceptsocknw = 
							new TCPAcceptSocketNoWait( acceptsock->getFd(), framer);
				Message msg;
				msg.setFramer(framer);
				struct nowait_struct stmsg;
				strcpy( stmsg.str, "hello world in a structure" );
				msg.putDataRaw( (char*)&stmsg, sizeof(nowait_struct) );
				acceptsocknw->send( msg );
				//delete only closes the socket from the server side. The socket is still open on the client side.
				delete acceptsock;
				delete framer;
				delete acceptsocknw;
			}
		} while(1);

	}catch(debug_lib::Exception& e){
		debug_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}catch(...){
		debug_lib::log( "exit due to error in server: %s", "unknown error" );
		exit(1);
	}
	return 0;
}
