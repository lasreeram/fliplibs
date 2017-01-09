#include <revafile.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
using namespace std;

void print_help(){
	cout << "usage: randlist <inputfile>\n";
}

void print_vector( vector<string>& v1 ){
	for( auto it = v1.begin(); it != v1.end(); it++ ){
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
	vector<string> v1;

	int recs = 0;
	std::string line = "";
	while( ifs.getline( buf, sizeof(buf) ) ){
		line = buf;
		v1.push_back(line);
		memset(buf, '\0', sizeof(buf) );
		recs++;
	}
	ifs.close();

	random_shuffle( v1.begin(), v1.end() );
	print_vector( v1 );
	debug_lib::log("finished - processed %d records", recs );
	return 0;
}
