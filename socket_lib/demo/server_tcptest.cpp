#include "Socket.h"
using namespace sockets_lib;

void service(TCPSocket* sock, int rcvbufsz){
	char* buf = (char*) malloc(rcvbufsz);
	if ( buf == NULL )
		sockets_lib::throw_fatal_error( "malloc:cannot allocate memory" );

	int rc = -1;
	int bytes = 0;
	int blks = 0;
	for( ;; ){
		rc = sock->recv_header( buf, rcvbufsz );
		if( rc == 0 )
			break;
		bytes += rc;
		blks += 1;
	}
	sockets_lib::log(  "%d bytes received, number of blocks = %d", bytes, blks );

	free(buf);
	return;
}

void print_help(){
	sockets_lib::log(  "server [-b<recv buf size>] <hostname> <port>" );
}

int main(int argc, char** argv){
	char* hname;
	char* sname;
	INIT();
	int rcvbufsz = 5000 * 1044;

	opterr = 0;
	char c;
	while( ( c = getopt( argc, argv, "b:" ) ) != EOF ){
		switch(c){
			case 'b':
				rcvbufsz = atoi(optarg);
				break;

			case '?':
				break;
				print_help();
		}
	}

	if ( optind+1 < argc ){
		hname = argv[optind];
		sname = argv[optind+1];
	}
	else if ( optind < argc ){
		hname = NULL;
		sname = argv[optind];
	}else{
		sockets_lib::log(  "exit: wrong arguments passed " );
		print_help();
		exit(1);
	}

	try{
		TCPServerSocket* sock = new TCPServerSocket();
		sock->setSocketOption( SO_RCVBUF, rcvbufsz );
		sock->bind(hname, sname);
		sock->listen();

		do{
			TCPSocket* acceptsock = sock->accept();
			service(acceptsock, rcvbufsz);
			//delete only closes the socket from the server side. The socket is still open on the client side.
			delete acceptsock;
		} while(0);

	}catch(sockets_lib::SocketException& e){
		sockets_lib::log(  "exit: due to error in server %s", e.what() );
		exit(1);
	}catch(...){
		sockets_lib::log(  "exit: due to unknown error in server" );
		exit(1);
	}
	return 0;
}
