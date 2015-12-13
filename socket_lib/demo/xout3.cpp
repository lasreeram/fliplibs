#include <Socket.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#define ACK 0x6
#define MAX_UNACKED_MSGS 128
#define FIRST_ACK_WAIT_TIME 3000
#define SECOND_ACK_WAIT_TIME 5000
#define ACK_MESSAGE_SIZE 5
using namespace sockets_lib;

bool gTerminateMe = false;

void signal_handler(int sig){

	if ( sig == SIGINT )
		gTerminateMe = true;
}

struct packet_t{
	uint32_t cookie;
	uint32_t retries;
	uint32_t len;
	char buf[4096];
	void network_to_host(){
		len = ntohl(len);
		cookie = ntohl(cookie);
		retries = ntohl(cookie);
		_network_order = false;
	}
	void host_to_network(){
		len = htonl(len);
		cookie = htonl(cookie);
		retries = htonl(retries);
		_network_order = true;
	}
	int getLength(){
		if ( _network_order )
			return ( (2 * sizeof(uint32_t)) + ntohl(len) );
		else
			return ( (2 * sizeof(uint32_t)) + len );
	}
	packet_t() { _network_order = false; }
	bool _network_order;
};

void print_help(){
	std::cout << "xout3 <hostname> <port>" << std::endl;
}
bool use_close = false;

uint32_t getNextMessageId(){
	static uint32_t id = 1;
	id++;
	if ( id > 999999999 )
		id = 1;
	return id;
}

class StdInReadHandler : public IOPollMultiTimerHandler {
	public:
	StdInReadHandler(TCPSocket* socket, IOPollMultiTimerHandler* timeoutHandler){
		_socket = socket; 
		_timeoutHandler = timeoutHandler;
	}

	void timeout( uint32_t timer_id, IOPollMultiTimerManager* mgr ){
	}

	void handle( IOPollMultiTimerManager* mgr ){
		if ( fgets( lout, sizeof(lout), stdin ) == NULL ){
			mgr->removeFromReadSet(0);
			_socket->close();
			sockets_lib::throw_error( "fgets returns null" );
		}else{
			packet_t message;
			uint32_t msg_id = getNextMessageId();
			message.cookie = msg_id;
			message.retries = 0;
			message.len = strlen(lout) + sizeof(uint32_t);
			char* message_ptr = reinterpret_cast<char*>(&message);
			vector<char> timer_data( message_ptr, message_ptr + message.getLength() );
			message.host_to_network();
			_socket->send_header(&message, message.getLength() );
			sockets_lib::log( "message %u sent ", ntohl(message.cookie) );
			mgr->setTimer( msg_id, FIRST_ACK_WAIT_TIME, _timeoutHandler, timer_data );
		}
		
		return;
	}
	private:
		TCPSocket* _socket;
		IOPollMultiTimerHandler* _timeoutHandler;
		char lout[2048];
}; 
class SocketReadHandler : public IOPollMultiTimerHandler {
	public:
	SocketReadHandler(TCPSocket* socket) { 
		_socket = socket; 
		offset = 0;
	}

	void timeout( unsigned int timer_id, IOPollMultiTimerManager* mgr ){
		vector<char> timerdata;
		int ret = mgr->getTimerData( timer_id, timerdata );
		if ( ret < 0 ){
			sockets_lib::log( "no timer data saved" );
			return;
		}
		packet_t message;
		memcpy( &message, &timerdata[0], timerdata.size() );
		if ( message.retries == 0 ){
			message.retries++;
			char* message_ptr = reinterpret_cast<char*>(&message);
			vector<char> new_timer_data( message_ptr, message_ptr + message.getLength() );
			sockets_lib::log( "retrying message %u, %d", timer_id, message.retries );
			message.host_to_network();
			_socket->send_header(&message, message.getLength() );
			sockets_lib::log( "sent message %u, %u", timer_id, message.getLength() );
			mgr->renewTimer( timer_id, SECOND_ACK_WAIT_TIME, new_timer_data );
		}else {
			sockets_lib::log( "dropping message %u", timer_id );
			mgr->removeTimerData( timer_id );
		}
	}

	void handle ( IOPollMultiTimerManager* mgr ){
		//the recv_no_header function throws an exception on a fatal error and returns 0 for EOF.
		rc = _socket->recv_no_header( recvbuf+offset,  ACK_MESSAGE_SIZE - offset );
		//rc < 0 readLine throws exception
		if ( rc == 0 )
			sockets_lib::throw_error( "server disconnected" );
		else {
			offset += rc;
			if ( offset < ACK_MESSAGE_SIZE )
				return; //wait for more data
		}

		offset = 0;
		if ( recvbuf[0] != ACK ){
			sockets_lib::log( "unexpected message from server:%x\n", recvbuf[0] );
			return;
		}

		uint32_t msg_id;
		memcpy( &msg_id, recvbuf+1, sizeof(uint32_t) );
		msg_id = ntohl( msg_id );
		sockets_lib::log( "ACK received for message %u", msg_id );
		mgr->cancelTimer( msg_id );
		return;
	}
	private:
		TCPSocket* _socket;
		char recvbuf[2048];
		int rc;
		int offset;
};

int main(int argc, char** argv){
	char* hname;
	char* sname;
	INIT();
	//signal( SIGINT, signal_handler);

	if ( argc == 3 ){
		hname = argv[1];
		sname = argv[2];
	} else {
		print_help();
		exit(1);
	}

	try{
		TCPClientSocket* sock = new TCPClientSocket();
		sock->connect(hname, sname);
		IOPollMultiTimerManager* pollMgr = new IOPollMultiTimerManager();
		SocketReadHandler* sockReadHandler = new SocketReadHandler(sock);
		StdInReadHandler* stdinReadHandler = new StdInReadHandler(sock, sockReadHandler);
		pollMgr->addSourceForRead( sock->getFd(), sockReadHandler );
		pollMgr->addSourceForRead( 0, stdinReadHandler );
		pollMgr->pollAndHandleEvents();
		delete pollMgr;
		delete sockReadHandler;
		delete stdinReadHandler;
		
	}catch(SocketException& e){
		sockets_lib::log(  "exit due to error in server: %s", e.what());
		exit(1);
	}
	return 0;
}
