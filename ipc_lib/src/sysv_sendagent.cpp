#include <IPC.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <debug.h>
using namespace ipc_lib;

void print_help(){
	std::cout << "sysv_sendagent <dest_mailbox> <string to send>" << std::endl;
}

int main(int argc, char** argv){
	debug_lib::init(argv[0]);

	if ( argc == 3 ){
	}else{
		debug_lib::log(  "exit: wrong arguments passed %d\n", argc );
		print_help();
		exit(1);
	}

	char buf[1024] = {0};
	strcpy( buf, argv[2] );
	try{
		SystemVIPC* ipc = new SystemVIPC("/tmp");
		my_msgbuf msg = {0};

		msg.msgtype = 1;
		strcpy( msg.mtext, buf );
		int rc = ipc->send( argv[1], &msg, strlen(msg.mtext)+1 );
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
