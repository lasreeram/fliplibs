#include <Socket.h>
#include "skel.h"
using namespace sockets_lib;

// first version of udpecho run by inetd
// In this version this server does the work and exits
// inetd does not accept any new datagrams until this server terminates
// in the next version udpecho2, the server forks and the parent terminates immediately
// this allows the child to service the client and inetd to accept new datagrams
int main(int argc, char** argv){
	int rc;
	int pidsz;
	char buf[120];

	pidsz = sprintf(buf, "%d: ", getpid() );
	INIT();
	sockets_lib::logDebugInSyslog();
	
	try{
		UDPSocket* socket = new UDPSocket(0);
		rc = socket->recvFrom( buf+pidsz, sizeof(buf)-pidsz );
		if ( rc == 0 ){
			sockets_lib::throw_error( "end of file on recv" );
		}

		UDPClientSocket* clientsocket = new UDPClientSocket();
		//connect to the address on a different socket
		clientsocket->connect( socket->getPeerAddress() );

		if ( fork() != 0 ){
			exit(0);
		}
		sockets_lib::log( "child continues" );

		//child process
		while( strncmp( buf + pidsz, "done", 4 ) != 0 ){
			clientsocket->sendTo( buf, rc+pidsz );
			sockets_lib::log( "%s", buf );
			pidsz = sprintf(buf, "%d: ", getpid() );
			alarm(30);
			rc = clientsocket->recvFrom( buf+pidsz, sizeof(buf) - pidsz );
			alarm(0);
			buf[rc+pidsz] = '\0';
			sockets_lib::log( "%s", buf+pidsz );
		}
		delete clientsocket;
		exit(0);
	}catch(SocketException& e){
		alarm(0);
		sockets_lib::log( "exit due to error in server: %s", e.what() );
		exit(1);
        }catch(...){
		alarm(0);
		sockets_lib::log( "exit due to error in server: %s", "unknown error" );
		exit(1);
	}
	
	return 0;
}
