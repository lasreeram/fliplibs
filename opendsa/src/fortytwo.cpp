#include <revafile.h>
#include <fstream>
#include <string>
#include <deque>
#include <iostream>
#include <stdlib.h>
using namespace std;

void print_help(){
	cout << "usage: fortytwo <inputfile>\n";
}

int main( int argc, char** argv){
	std::ifstream ifs;	
	char buf[1024] = {0};
	debug_lib::init(argv[0]);

	if( argc != 2 ){
		print_help();
		exit(1);
	}

	ifs.open( argv[1], std::ifstream::in );

	int recs = 0;
	std::deque<std::string> dek;
	std::string line = "";
	while( ifs.getline( buf, sizeof(buf) ) ){
		line = buf;
		dek.push_front(line);
		if( dek.size() > 43 ){
			dek.pop_back();
		}
		if ( line.length() == 0 ){
			debug_lib::log( "%s", dek.back().c_str() );
		}
		memset(buf, '\0', sizeof(buf) );
		recs++;
	}

	debug_lib::log("finished - processed %d records", recs );
	return 0;
}
