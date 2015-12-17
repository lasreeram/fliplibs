#include <Socket.h>
#include <string.h>
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

	char buf[2048] = {0};
	int rc = 0;
	try{
		TCPClientSocket* sock = new TCPClientSocket();
		sock->connect(hname, sname);

		while( fgets(buf, sizeof(buf), stdin ) != NULL ){
			sock->writeLine( buf, strlen(buf) );
			memset(buf, '\0', sizeof(buf) );
			rc = sock->readLine( buf, sizeof(buf) - 1);
			if ( rc == 0 )
				debug_lib::throw_error( "end of file received on socket" );
			std::cout << buf;
		}
		delete sock;
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error: %s", e.what());
		exit(1);
	}
	return 0;
}
