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
	INIT();

	pidsz = sprintf(buf, "%d: ", getpid() );
	
	try{
		UDPSocket* socket = new UDPSocket(0);
		rc = socket->recvFrom( buf+pidsz, sizeof(buf)-pidsz );
		if ( rc == 0 ){
			sockets_lib::throw_error( "end of file on recv" );
		}
		socket->sendTo( buf, rc+pidsz );
		delete socket;
		exit(0);
	}catch(SocketException& e){
            sockets_lib::log( "exit due to error in server: %s", e.what() );
            exit(1);
        }catch(...){
            sockets_lib::log( "exit due to error in server: %s", "unknown error" );
            exit(1);
	}
	
	return 0;
}
