#include <Socket.h>
#include <iostream>
using namespace sockets_lib;

void print_help(){
	std::cout << "shutdownc <hostname> <port>" << std::endl;
}
bool use_close = false;

class StdInReadHandler : public IOPollHandler {
	public:
	StdInReadHandler(TCPSocket* socket){_socket = socket;}
	void handle( IOPollManager* mgr ){
		if ( fgets( lout, sizeof(lout), stdin ) == NULL ){
			mgr->removeFromReadSet(0);
			if( use_close == true){
				debug_lib::log( "call close" );
				_socket->close();
				sleep(5);
				exit(0);
			}else{
				debug_lib::log( "call shutdown" );
				_socket->shutdown(1);
			}
		}else{
			_socket->send_header(lout, strlen(lout));
		}
		
		return;
	}
	private:
		TCPSocket* _socket;
		char lout[1024];
};

class SocketReadHandler : public IOPollHandler {
	public:
	SocketReadHandler(TCPSocket* socket) { _socket = socket; }
	void handle ( IOPollManager* mgr ){
		rc = _socket->recv_header( buf, sizeof(buf)-1 );
		if ( rc == 0 )
			debug_lib::throw_error( "server disconnected" );
		buf[rc] = '\0';
		debug_lib::log( "server responds:%s\n", buf );
		return;
	}
	private:
		TCPSocket* _socket;
		char buf[2048];
		int rc;
};

int main(int argc, char** argv){
	char* hname;
	char* sname;
	debug_lib::init(argv[0]);

	opterr = false;
	char c;
	while ( ( c = getopt( argc, argv, "c" ) )  != EOF ){
		switch(c){
			case 'c':
				use_close = true;
				break;
			case '?':
				print_help();
				exit(1);
		}
	}

	if ( (argc - optind) != 2 ){
		print_help();
		exit(1);
	}


	if ( (argc-optind) == 2 ){
		hname = argv[optind];
		sname = argv[optind+1];
	}

	try{
		TCPClientSocket* sock = new TCPClientSocket();
		sock->connect(hname, sname);
		IOPollManager* pollMgr = new IOPollManager();
		SocketReadHandler* sockReadHandler = new SocketReadHandler(sock);
		StdInReadHandler* stdinReadHandler = new StdInReadHandler(sock);
		pollMgr->addSourceForRead( sock->getFd(), sockReadHandler );
		pollMgr->addSourceForRead( 0, stdinReadHandler );
		pollMgr->pollAndHandleEvents();
		delete pollMgr;
		delete sockReadHandler;
		delete stdinReadHandler;
		
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
