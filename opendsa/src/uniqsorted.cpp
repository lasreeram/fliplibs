#include <revafile.h>
#include <fstream>
#include <string>
#include <set>
#include <iostream>
#include <stdlib.h>
using namespace std;

void print_help(){
	cout << "usage: uniqsorted <inputfile>\n";
}

void print_set1( set<string>& set1 ){
	for( auto it = set1.begin(); it != set1.end(); it++ ){
		debug_lib::log( "%s", it->c_str() );
	}
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
	std::set<std::string> set1;
	std::string line = "";
	while( ifs.getline( buf, sizeof(buf) ) ){
		line = buf;
		if( set1.find( line ) == set1.end() ){
			set1.insert(line);
		}
		memset(buf, '\0', sizeof(buf) );
		recs++;
	}

	print_set1( set1 );
	debug_lib::log("finished - inserted %d records", set1.size() );
	debug_lib::log("finished - processed %d records", recs );
	return 0;
}
