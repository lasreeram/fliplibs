#include <revafile.h>
#include <fstream>
#include <string>
#include <unordered_set>
#include <iostream>
#include <stdlib.h>
using namespace std;

void print_help(){
	cout << "usage: uniq <inputfile>\n";
}

void print_uset( unordered_set<string>& uset ){
	for( auto it = uset.begin(); it != uset.end(); it++ ){
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
	std::unordered_set<std::string> uset;
	std::string line = "";
	while( ifs.getline( buf, sizeof(buf) ) ){
		line = buf;
		if( uset.find( line ) == uset.end() ){
			uset.insert(line);
		}
		memset(buf, '\0', sizeof(buf) );
		recs++;
	}

	print_uset( uset );
	debug_lib::log("finished - inserted %d records", uset.size() );
	debug_lib::log("finished - processed %d records", recs );
	return 0;
}
