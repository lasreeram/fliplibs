#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <Socket.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main( int argc, char** argv){

	//signal( SIGALRM, SIG_IGN);
	int cnt = 0;
	char line[4096] = {0};
	debug_lib::init(argv[0]);
	debug_lib::logDebugInSyslog();

	//this program is to be run by inetd.
	//since the socket is mapped to stdout by inetd and passed on to this application, 
	//we have to explicitly set the line buffering for stdin. This is because standard I/O library
	//sets line bufferring automatically only if it is senses it is writing to a terminal.
	setvbuf( stdout, NULL, _IOLBF, 0 );
	memset ( line, 0x00, sizeof(line) );
	while( fgets( line, sizeof(line)-1, stdin ) != NULL ){
		debug_lib::log ( "read line %s", line );
		cout << setw(3) << ++cnt << ":" << line << endl;
		debug_lib::log ( "written line %s", line );
		memset ( line, 0x00, sizeof(line) );
	}

	return 0;

}
