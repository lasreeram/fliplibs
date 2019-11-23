#include <Socket.h>
#include <iostream>
using namespace sockets_lib;

void print_help(){
	std::cout << "sslclient <hostname> <port>" << std::endl;
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
	char obuf[2048] = {0};
	try{
		TCPClientSocket* sock = new TCPClientSocket();
		sock->connect(hname, sname);
		SSLBioImpl* sslObj = new SSLBioImpl( false, "/home/sreeram/ssl/client" );
		sslObj->doHandshake( sock, false );
		int olen = sock->recv_header( buf, sizeof(buf) );
		sslObj->decodeSSL( buf, olen, obuf, sizeof(obuf) );
		std::cout << obuf << std::endl;
		strcpy( buf, "hi there" );
		olen = sslObj->encodeSSL( buf, strlen(buf), obuf, sizeof(obuf) );
		sock->send_header( obuf, olen );
		sslObj->shutdown();
		delete sslObj;
		delete sock;
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error: %s", e.what());
		exit(1);
	}
	return 0;
}
