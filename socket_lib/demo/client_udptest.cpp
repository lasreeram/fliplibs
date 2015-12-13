#include <Socket.h>
#include <iostream>
using namespace sockets_lib;

void print_help(){
	std::cout << "client <hostname> <port> <number of datagrams to send>" << std::endl;
}

int main(int argc, char** argv){
	char* hname;
	char* sname;
	int ndatagrams = 0;
	INIT();

	if ( argc == 4 ){
		hname = argv[1];
		sname = argv[2];
		ndatagrams = atoi(argv[3]);
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
		int datagramsz = 2048;
		std::cout << "sending " << ndatagrams << std::endl;
		while ( ndatagrams-- > 0 ){
			sock->sendTo( buf, datagramsz );
		}
		std::cout << "sending eof" << std::endl;
		sock->sendTo ( "", 0 );
		delete sock;
	}catch(SocketException& e){
		sockets_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
