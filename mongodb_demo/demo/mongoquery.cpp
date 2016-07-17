#include <cstdlib>
#include <iostream>
#include <mongo/client/dbclient.h> // for the driver
#include <mongo/client/dbclientcursor.h> // for the driver
#include <debug.h>

using namespace mongo;
void run(mongo::DBClientConnection& c) {
	debug_lib::log( "read records - where transaction = 1");

	std::auto_ptr<DBClientCursor> cursor = c.query("hackdb.transactions", MONGO_QUERY("transaction_id" << 1) );
	int i = 0;
	while (cursor->more()){
		i++;
		std::cout << cursor->next().toString() << std::endl;
	}
	debug_lib::log( "read %d records", i );

	debug_lib::log( "read records - where amount > 1 and sort by transaction id");

	cursor = c.query("hackdb.transactions", MONGO_QUERY("amount" << GT << 1 << LT << 10000000).sort("transaction_id") );
	while (cursor->more()){
		i++;
		std::cout << cursor->next().toString() << std::endl;
	}
	debug_lib::log( "read %d records", i );

	debug_lib::log( "print only transaction id for the last query");
	cursor = c.query("hackdb.transactions", MONGO_QUERY("amount" << GT << 1 << LT << 10000000).sort("transaction_id") );
	while (cursor->more()){
		BSONObj p = cursor->next();
		BSONElement tranid = p.getField("transaction_id");
		std::cout << tranid.toString(false,true) << std::endl;	
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
