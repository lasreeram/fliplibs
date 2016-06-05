#include <cstdlib>
#include <iostream>
#include <mongo/client/dbclient.h> // for the driver
#include <mongo/client/dbclientcursor.h> // for the driver
#include <debug.h>

using namespace mongo;
void run(mongo::DBClientConnection& c) {
	std::auto_ptr<DBClientCursor> cursor = c.query("hackdb.transactions", BSONObj());
	int i = 0;
	while (cursor->more()){
		i++;
		std::cout << cursor->next().toString() << std::endl;
	}
	debug_lib::log( "read %d records", i );
}

int main(int argc, char** argv) {
	debug_lib::init( argv[0] );
	mongo::client::initialize();
	mongo::DBClientConnection c;
	c.connect("localhost");
	try {
		run(c);
	} catch( const mongo::DBException &e ) {
		debug_lib::log( "caught exception %s", e.what() );
	}
	return EXIT_SUCCESS;
}
