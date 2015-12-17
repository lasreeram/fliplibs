#include <Socket.h>
using namespace sockets_lib;

void service(TCPSocket* sock){
	int rc = 0;
	char buf[2048] = {0};
	for( ; ; ){
		rc = sock->readLine( buf, sizeof(buf) - 1);
		buf[rc] = '\0';
		if ( rc == 0 )
			debug_lib::throw_error( "EOF received on read" );
		write( 1, buf, rc );
		rc = sock->writeLine( "\006\n", 2 ); //write back the ack
	}
	return;
}

void print_help(){
	debug_lib::log(  "keep <hostname> <port>" );
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
		debug_lib::log( "exit: wrong arguments passed %d\n", argc );
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
			delete acceptsock;
		} while(1);

	}catch(sockets_lib::debug_lib::Exception& e){
		debug_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}catch(...){
		debug_lib::log( "exit due to error in server: %s", "unknown error" );
		exit(1);
	}
	return 0;
}
