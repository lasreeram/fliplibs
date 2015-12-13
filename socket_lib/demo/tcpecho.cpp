#include <Socket.h>
#include <iostream>
using namespace sockets_lib;
int nap_time = 5;

void service(TCPSocket* sock){
	int rc = 0;
	char buf[2048];
	while(true){
		rc = sock->recv_header( buf, sizeof(buf)-1 );
		if ( rc == 0 )
			sockets_lib::throw_error( "client disconnected" );
		buf[rc] = '\0';
		sockets_lib::log( "%s\n", buf );
		if ( nap_time )
			sleep(nap_time);
		sock->send_header( buf, strlen(buf) );
	}
	return;
}

void print_help(){
	sockets_lib::log(  "tcpecho [-n<naptime] <hostname> <port>" );
}

int main(int argc, char** argv){
	char* hname;
	char* sname;
	INIT();

	char ch;
	opterr = 0;
	while ( ( ch = getopt( argc, argv, "n:" ) )  != EOF ){
		switch(ch){
			case 'n': nap_time = atoi(optarg);
				  break;
			case '?':
				  nap_time = 5;
				  break;
		}
	}

	int remain_args = (argc - optind);
	if ( remain_args == 1 ){
		hname = NULL;
		sname = argv[optind];
	}else if ( remain_args == 2 ){
		hname = argv[optind];
		sname = argv[optind+1];
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
			//delete only closes the socket from the server side. The socket is still open on the client side.
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
