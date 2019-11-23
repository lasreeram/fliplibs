#include <Socket.h>
#include <debug.h>
using namespace sockets_lib;

void service(TCPSocket* sock, SSLBioImpl* sslObj){
	char buf[] = "hello world\n";
	char obuf[4096];
	char ibuf[4096];
	int ilen;
	int olen = sslObj->encodeSSL( buf, strlen(buf), obuf, sizeof(obuf) );
	sock->send_header( obuf, olen );
	ilen = sock->recv_header( ibuf, sizeof(ibuf) );
	olen = sslObj->decodeSSL( ibuf, ilen, obuf, sizeof(obuf) );
	obuf[olen] = '\0';
	debug_lib::log( "message recvd = %s", obuf );
	return;
}

void print_help(){
	debug_lib::log(  "sslserver <hostname> <port>" );
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
			SSLBioImpl* sslObj = new SSLBioImpl( true, "/home/sreeram/ssl/server" );
			sslObj->doHandshake( acceptsock, true );
			service( acceptsock, sslObj );
			sslObj->shutdown();
			delete sslObj;
			//delete only closes the socket from the server side. The socket is still open on the client side.
			delete acceptsock;
		} while(1);

	}catch(debug_lib::Exception& e){
		debug_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
	}catch(...){
		debug_lib::log( "exit due to error in server: %s", "unknown error" );
		exit(1);
	}
	return 0;
}
