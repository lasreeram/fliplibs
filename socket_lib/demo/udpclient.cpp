#include <Socket.h>
#include <iostream>
using namespace sockets_lib;

void print_help(){
	std::cout << "udpclient <hostname> <port>" << std::endl;
}

int main(int argc, char** argv){
	char* hname;
	char* sname;
	int rc = -1;
	debug_lib::init(argv[0]);

	if ( argc == 3 ){
		hname = argv[1];
		sname = argv[2];
	}else{
		debug_lib::log( "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	char buf[2048] = {0};
	try{
		UDPClientSocket* sock = new UDPClientSocket();
		//sets the peer address from host name and service name
		sock->setAddress( hname, sname );
		while( fgets( buf, sizeof(buf), stdin ) != NULL ){
			sock->sendTo(buf, strlen(buf) );
			//peer address might change in recvfrom
			rc = sock->recvFrom( buf, sizeof(buf) - 1 );
			if ( rc == 0 )
				debug_lib::throw_error( "EOF received" );
			buf[rc] = '\0';
			fputs( buf, stdout );
		}
		delete sock;
	}catch(debug_lib::Exception& e){
		debug_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
