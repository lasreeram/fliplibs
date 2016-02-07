#include <IPC.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <debug.h>
using namespace ipc_lib;

void print_help(){
	std::cout << "createmb <mbname> " << std::endl;
}

int main(int argc, char** argv){
	debug_lib::init(argv[0]);

	if ( argc == 2 ){
	}else{
		debug_lib::log(  "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	try{
		ShmIPC* ipc = new ShmIPC();
		ipc->createMailBox( argv[1] );
		debug_lib::log( "mailbox %s created", argv[1] );
		delete ipc;
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error: %s", e.what());
		exit(1);
	}
	return 0;
}
