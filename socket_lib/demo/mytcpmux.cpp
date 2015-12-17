#include <Socket.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAXARGS 10
#define MAXLINE 2048
#define NSERVTAB 1024
#define CONFIG "/home/hduser/books/sockprog/lib/mytcpmux.conf" 
typedef struct 
{
	int flag;
	char* service;
	char* path;
	char* args[ MAXARGS + 1 ];
}servtab_t;

servtab_t service_table[ NSERVTAB + 1 ];

using namespace sockets_lib;
TCPServerSocket* listensocket = NULL; 

static void parsetab( servtab_t* stp ){
	FILE* fp;
	fp = fopen( CONFIG, "r" );

	if ( fp == NULL )
		debug_lib::throw_error( "unable to open config file %s", CONFIG );

	int lineno = 0;
	stp = service_table;
	char* saveptr = NULL;
	char line[MAXLINE] = {0};
	char* cp;
	debug_lib::log( "start parsing config file" );
	while( fgets( line, sizeof(line), fp ) != NULL ){
		lineno++;
		if ( line[ strlen(line) - 1 ] != '\n' ){
			debug_lib::throw_error( "line %d is too long", lineno );
		}
		line[ strlen(line) ] = '\0';

		debug_lib::log( "checking if exceeded table entries" );
		if ( stp >= service_table + NSERVTAB ) 
			debug_lib::throw_error( "too many entries in config" );
		
		debug_lib::log( "checking for comments" );
		cp = strchr( line, '#' );
		if ( cp != NULL )
			*cp = '\0';

		debug_lib::log( "parsing line %s into tokens", line );
		cp = strtok_r( line, " \t\n", &saveptr);
		if ( cp == NULL )
			continue;

		debug_lib::log( "looking for + character" );
		if ( *cp == '+' ){
			stp->flag = true;
			cp++;
			if ( *cp == '\0' ||  strchr( " \t\n", *cp ) != NULL )
				debug_lib::throw_error( "white space after + at %d", lineno );
		}

		stp->service = strdup( cp );
		if ( stp->service == NULL )
			debug_lib::throw_error( "out of memory" );

		cp = strtok_r( NULL, " \t\n", &saveptr );

		if ( cp == NULL )
			debug_lib::throw_error( "line missing path name, service = %s, line = %d", 
							stp->service, lineno );

		stp->path = strdup( cp );
		if ( stp->path == NULL )
			debug_lib::throw_error( "out of memory" );

		int i;
		for ( i = 0; i < MAXARGS; i++ ){
			cp = strtok_r( NULL, " \t\n", &saveptr );
			if ( cp == NULL )
				break;
			stp->args[i] = strdup(cp);
			if ( stp->args[i] == NULL )
				debug_lib::throw_error( "out of memory" );
				
		}

		if ( i >= MAXARGS && strtok_r (NULL, " \t\n", &saveptr) != NULL )
			debug_lib::throw_error( "too many arguments at line %d", lineno );

		stp->args[i] = NULL;
		stp++;
	}
	debug_lib::log( "finished parsing config file" );

	stp->service = NULL;
	fclose(fp);

}

void start_server( TCPSocket* socket ){

	char line[MAXLINE];
	servtab_t *stp = NULL;
	static bool _stp_parsed = false;
	int rc;
	static char err1[] = "-unable to read service name\n";
	static char err2[] = "-unknown service\n";
	static char err3[] = "-unable to start service\n";
	static char ok[] = "+OK";

	if ( !_stp_parsed ){
		parsetab(stp);
		_stp_parsed = true;
	}

	rc = fork();
	if ( rc < 0 ){
		write( socket->getFd(), err3, sizeof(err3) );
	}

	if ( rc != 0 ) //parent goes back to listening again
		return;

	listensocket->close(); //child can close the listening socket
	
	debug_lib::log( "waiting to read a line from accept socket" );
	alarm(10); //this is required so that the child does not wait for ever on read
	rc = socket->readLine( line, sizeof(line)-1 );
	if ( rc == 0 ){
		write( socket->getFd(), err1, sizeof(err1) );
		exit(1);
	}
	alarm(0); //ommitting this caused a SIGALRM in the child process.
	line[rc-1] = '\0';

	debug_lib::log( "matching the service from table" );
	for ( stp = service_table; stp->service; stp++ ){
		debug_lib::log( "matching the service %s, line = %s", stp->service, line );
		if ( strcmp( line, stp->service ) == 0 ){
			debug_lib::log( "matching %s, %s", line, stp->service );
			break;
		}else{
			debug_lib::log( "not matching %s, %s", line, stp->service );
		}
	}

	debug_lib::log( "checking service data" );
	if ( !stp->service ){
		debug_lib::log( "cannot find service" );
		write( socket->getFd(), err2, sizeof(err1) );
		exit(1);
	}
		
	if ( stp->flag ){
		socket->writeLine( ok, strlen(ok) );
	}

	debug_lib::log( "executing new process path=%s, args=%s", stp->path, stp->args[0] );
	//duplicate all the file descriptors to std fds (this is what inetd - internet superserver also does)
	dup2( socket->getFd(), 0 );
	dup2( socket->getFd(), 1 );
	dup2( socket->getFd(), 2 );
	//now that we have dup2 the socket, no use for it.
	socket->close();

	rc = execv( stp->path, stp->args );
	if ( rc < 0 )
		debug_lib::throw_error( "error executing process at %s, with args %s, %d, %s", 
				stp->path, stp->args[0], errno, strerror(errno) );
	write( 1, err3, sizeof(err3) );
}

void reaper( int sig ){

	debug_lib::log( "caught signal sig = %d", sig );
	int waitstatus;
	while ( waitpid( -1, &waitstatus, WNOHANG ) > 0 ){
		;
	}
}

int main( int argc, char** argv ){

	char* hostname;
	char* service;
	debug_lib::init(argv[0]);
	logDebugInSyslog();

	if ( argc == 3 ){
		hostname = argv[1];
		service = argv[2];
	}else{
		debug_lib::log( "usage: %s [ interface [ port ] ]\n", argv[1] );
		//mytcpmux 127.0.0.1 8888
		exit(1);
	}

	try{
		listensocket = new TCPServerSocket();
		listensocket->bind( hostname, service );
		listensocket->listen();

		daemon( 0, 0 );
		signal ( SIGCHLD, reaper );
		for( ;; ){
			debug_lib::log( "listening for connection" );
			TCPSocket* acceptsocket = listensocket->accept();			
			debug_lib::log( "created an accept socket" );
			start_server(acceptsocket);
			//acceptsocket->close();
		}

        }catch(sockets_lib::debug_lib::Exception& e){
		debug_lib::log( "exit due to error in server: %s", e.what());
		exit(1);
        }catch(...){
		debug_lib::log( "exit due to error in server: %s", "unknown error" );
		exit(1);
	}


	return 0;
}
