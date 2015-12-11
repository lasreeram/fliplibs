#include "Socket.h"
#include <iostream>
using namespace sockets_lib;

void print_help(){
	std::cout << "client <hostname> <port>" << std::endl;
}

int main(int argc, char** argv){
	char* hname;
	char* sname;
	INIT();

	if ( argc == 3 ){
		hname = argv[1];
		sname = argv[2];
	}else{
		sockets_lib::log(  "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	char buf[2048] = {0};
	try{
		TCPClientSocket* sock = new TCPClientSocket();
		sock->connect(hname, sname);
		sock->recv_header( buf, sizeof(buf) );
		std::cout << buf << std::endl;
		//strcpy( buf, "got it" );
		//sock->send( buf, sizeof(buf) );
		delete sock;
	}catch(SocketException& e){
		sockets_lib::log(  "exit due to error: %s", e.what());
		exit(1);
	}
	return 0;
}
