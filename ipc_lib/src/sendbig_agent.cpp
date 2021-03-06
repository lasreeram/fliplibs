#include <IPC.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <debug.h>
using namespace ipc_lib;

void print_help(){
	std::cout << "sendbig_agent <dest_actor> <len of data to send>" << std::endl;
}

int main(int argc, char** argv){
	debug_lib::init(argv[0]);

	if ( argc == 3 ){
	}else{
		debug_lib::log(  "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	char buf[8192] = {0};
	int len = atoi(argv[2]);
	if ( len > 8192 )
		debug_lib::throw_error( "length %d must be less than 8192 for this test" );

	
	memset( buf, 'a', len );

	try{
		ShmIPC* ipc = new ShmIPC();
		int rc = ipc->send( argv[1], buf, len );
		if ( rc == 0 ){
			debug_lib::log( "after send, sent %s", buf );
		}else{
			debug_lib::log( "error %d, in send", rc );
		}
		delete ipc;
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error: %s", e.what());
		exit(1);
	}
	return 0;
}
