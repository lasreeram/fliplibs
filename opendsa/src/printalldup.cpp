#include <revafile.h>
#include <fstream>
#include <string>
#include <set>
#include <iostream>
#include <stdlib.h>
using namespace std;

void print_help(){
	cout << "usage: printalldup <inputfile>\n";
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
	int printed = 0;
	std::set<std::string> set1;
	std::string line = "";
	while( ifs.getline( buf, sizeof(buf) ) ){
		line = buf;
		if( set1.find( line ) == set1.end() ){
			set1.insert(line);
		}else{
			debug_lib::log( "%s", line.c_str() );
			printed++;
		}
		memset(buf, '\0', sizeof(buf) );
		recs++;
	}

	debug_lib::log("printed %d records", printed );
	debug_lib::log("finished - inserted %d records", set1.size() );
	debug_lib::log("finished - processed %d records", recs );
	return 0;
}
