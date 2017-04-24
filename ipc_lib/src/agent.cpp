#include <IPC.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <debug.h>
using namespace ipc_lib;

void print_help(){
	std::cout << "agent <my_actor> <dest_actor> <myname>" << std::endl;
}

int main(int argc, char** argv){
	debug_lib::init(argv[0]);

	if ( argc == 4 ){
	}else{
		debug_lib::log(  "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	char buf[1024] = {0};
	try{
		ShmIPC* ipc = new ShmIPC();
		ipc->createActor( argv[1] );
		sleep(3); //wait for 3 seconds for the other agent to start
		sprintf( buf, "%s says:hello", argv[3] );
		int rc = ipc->send( argv[2], buf, strlen(buf) );
		debug_lib::log( "after send, sent %s", buf );
		if ( rc == 0 ){
			ipc->recv( argv[1], buf, 1024 );
			std::cout << buf << std::endl;
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
