#include <Socket.h>
#include <iostream>
using namespace sockets_lib;

void service(UDPSocket* sock){
	char buf[2048] = {0};

	//sets the max recv buf to 5000 data gram sizes
	sock->setSocketOption(SO_RCVBUF, 5000 * sizeof(buf) );
	int rc = -1;
	int datagrams = 0;
	for(;;)
	{
		rc = sock->recvFrom(buf, sizeof(buf) );
		if ( rc == 0 ){
			std::cout << "end of file recvd" << std::endl;
			break;
		}
		datagrams++;
	}
	std::cout << "number of data grams = " << datagrams << std::endl;
	debug_lib::log(  "number of datagrams recvd = %d", datagrams );
	return;
}

void print_help(){
	std::cout << "server <hostname> <port>" << std::endl;
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
		debug_lib::log(  "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	try{
		UDPServerSocket* sock = new UDPServerSocket();
		//reuse address allows multiple udp servers to be bound to the same socket and serve clients
		sock->setReuseAddressOption();
		sock->bind(hname, sname);
		service(sock);
	}catch(debug_lib::Exception& e){
		debug_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}catch(...){
		debug_lib::log(  "exit due to error in server: %s", "unknown error" );
		exit(1);
	}
	return 0;
}
