#include "Socket.h"
#include <iostream>
using namespace sockets_lib;

void print_help(){
	std::cout << "client [-s<send size>] [-b<send buf size>] [-c<count of blocks>] <hostname> <port>" << std::endl;
}

int main(int argc, char** argv){
	char* hname;
	char* sname;
	int blks = 5000;
	int sndbufsz = 32 * 1024;
	int sndsz = 1440;
	INIT();

	opterr = 0;
	char c;
	while( ( c = getopt( argc, argv, "s:b:c:" ) ) != EOF ){
		switch(c){
			case 's':
				sndsz = atoi(optarg);
				break;
			case 'b':
				sndbufsz = atoi(optarg);
				break;
			case 'c':
				blks = atoi(optarg);
				break;
			case '?':
				std::cout << "arguments not passed correctly" << std::endl;
				print_help();
				exit(1);
				break;
		}

	}
	if ( optind+1 < argc ){
		hname = argv[optind];
		sname = argv[optind+1];
	}else{
		sockets_lib::log( "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	char* buf = NULL;
	buf = (char*) malloc( sndsz ); 
	if ( buf == NULL )
		sockets_lib::throw_fatal_error( "cannot allocate memory. malloc failed" );

	try{
		TCPClientSocket* sock = new TCPClientSocket();
		sock->setSocketOption( SO_SNDBUF, sndbufsz );
		sock->connect(hname, sname);
		strcpy(buf, "1");
		std::cout << "start value of blks = " << blks << std::endl;
		while ( blks-- > 0 ){
			sock->send_header( buf, sndsz );
		}
		std::cout << "end value of blks = " << blks << std::endl;
		delete sock;
	}catch(SocketException& e){
		sockets_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
