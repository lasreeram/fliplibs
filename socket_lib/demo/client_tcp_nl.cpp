#include <Socket.h>
#include <iostream>
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

	char buf[2048] = "hello world";
	try{
		TCPClientSocket* sock = new TCPClientSocket();
		sock->connect(hname, sname);
		sock->writeLine( buf, strlen(buf) );
		//std::cout << buf << std::endl;
		//strcpy( buf, "got it" );
		//sock->send( buf, sizeof(buf) );
		delete sock;
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
