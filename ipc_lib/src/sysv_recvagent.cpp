#include <IPC.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <debug.h>
using namespace ipc_lib;

void print_help(){
	std::cout << "sysv_recvagent <my_mailbox>" << std::endl;
}

int main(int argc, char** argv){
	debug_lib::init(argv[0]);

	if ( argc == 2 ){
	}else{
		debug_lib::log(  "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	//char buf[1024] = {0};
	try{
		SystemVIPC* ipc = new SystemVIPC("/tmp");
		ipc->createMailBox( argv[1] );
		my_msgbuf msg;
		msg.msgtype = 1;
		int rc = ipc->recv( argv[1], &msg, sizeof(msg) );
		std::cout << "return code = " << rc << " rcvd data = " << msg.mtext << std::endl;
		delete ipc;
	}catch(debug_lib::Exception& e){
		debug_lib::log(  "exit due to error: %s", e.what());
		exit(1);
	}
	return 0;
}
