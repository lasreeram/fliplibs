#include <Socket.h>
using namespace sockets_lib;

void service(TCPSocket* sock){
	char buf[2048] = {0};
	int rc = sock->readLine(buf, sizeof(buf) );
	if ( rc == 0 )
		debug_lib::throw_error( "end of file received" );
	debug_lib::log( "%s", buf );
	return;
}

void print_help(){
	debug_lib::log(  "server <hostname> <port>" );
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
			//delete only closes the socket from the server side. The socket is still open on the client side.
			delete acceptsock;
		} while(0);

	}catch(debug_lib::Exception& e){
		debug_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}catch(...){
		debug_lib::log( "exit due to error in server: %s", "unknown error" );
		exit(1);
	}
	return 0;
}
