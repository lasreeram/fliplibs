#include <cstdlib>
#include <iostream>
#include <mongo/client/dbclient.h> // for the driver
#include <mongo/client/dbclientcursor.h> // for the driver
#include <debug.h>

using namespace mongo;
void run(mongo::DBClientConnection& c) {
	c.connect("localhost");
	srand( time(NULL) );
	int tranid = rand() % 1000000;
	int merchid = rand() % 1000000;
	int termid = rand() % 1000000;
	int amt = rand() % 1000000;
	BSONObj p = BSONObjBuilder().genOID(
				).append("transaction_id",tranid
				).append("merchant_id",merchid
				).append("terminal_id", termid
				).append("amount", amt).obj();
	c.insert( "hackdb.transactions", p );
	debug_lib::log( "record inserted %s", p.toString().c_str() );
}

int main(int argc, char** argv) {
	debug_lib::init(argv[0]);
	mongo::client::initialize();
	mongo::DBClientConnection c;
	try {
		run(c);
	} catch( const mongo::DBException &e ) {
	}
	//mongo::client::shutdown();
	return EXIT_SUCCESS;
}
