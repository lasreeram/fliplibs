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
		sockets_lib::log( "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	char buf[2048] = {0};
	try{
		UDPClientSocket* sock = new UDPClientSocket();
		sock->connect(hname, sname); //if this is not done. set address must be called
		//setAddress(hname, sname); //either connect or setAddress. Connect in udp ties the client to the given address.
		strcpy(buf, "hello");
		printf("sending %s\n", buf );
		sock->sendTo( buf, sizeof(buf) );
		sock->recvFrom( buf, sizeof(buf) );
		std::cout << buf << std::endl;
		delete sock;
	}catch(SocketException& e){
		sockets_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
