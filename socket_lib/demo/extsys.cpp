#include <Socket.h>
using namespace sockets_lib;
#define COOKIE_SIZE sizeof(uint32_t)

void service(TCPSocket* sock){
	int rc = 0;
	char buf[4096] = {0};
	for( ; ; ){
		rc = sock->recv_header( buf, sizeof(buf) - 1);
		buf[rc] = '\0';
		if ( rc == 0 )
			sockets_lib::throw_error( "EOF received on read" );
		int randp = rand() % 100;
		if ( randp < 33 ){
			sockets_lib::log( "do not ack message" );
			continue;
		}
		int msg_id;
		memcpy( &msg_id, buf, COOKIE_SIZE );
		sockets_lib::log( "ack message %u", ntohl(msg_id) );
		memmove( buf+1, buf, COOKIE_SIZE );
		buf[0] = '\006';
		sock->send_no_header( buf, 1 + COOKIE_SIZE );
	}
	return;
}

void print_help(){
	sockets_lib::log(  "extsys <hostname> <port>" );
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
			delete acceptsock;
		} while(1);

	}catch(sockets_lib::SocketException& e){
		sockets_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}catch(...){
		sockets_lib::log( "exit due to error in server: %s", "unknown error" );
		exit(1);
	}
	return 0;
}
