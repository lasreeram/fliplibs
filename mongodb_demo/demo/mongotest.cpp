#include <cstdlib>
#include <iostream>
#include <mongo/client/dbclient.h> // for the driver
#include <mongo/client/dbclientcursor.h> // for the driver
#include <debug.h>

using namespace mongo;
void run(mongo::DBClientConnection& c) {
	std::auto_ptr<DBClientCursor> cursor = c.query("hackdb.transactions", BSONObj());
	while (cursor->more())
		std::cout << cursor->next().toString() << std::endl;
}

int main() {
	mongo::client::initialize();
	mongo::DBClientConnection c;
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
	debug_lib::log( "inserting json object %s", p.toString().c_str() );
	
	c.insert( "hackdb.transactions", p );
	try {
		run(c);
		std::cout << "connected ok" << std::endl;
	} catch( const mongo::DBException &e ) {
		std::cout << "caught " << e.what() << std::endl;
	}
	//mongo::client::shutdown();
	return EXIT_SUCCESS;
}
