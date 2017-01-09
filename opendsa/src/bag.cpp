#include <revafile.h>
#include <fstream>
#include <string>
#include <unordered_set>
#include <iostream>
#include <stdlib.h>
using namespace std;

void print_help(){
	cout << "usage: bag <inputstr>\n";
}

void print_bag(unorderd_set<unordered_set<char>*>& bag ){
	for( auto it = bag.begin(); it != bag.end(); it++ ){
		for( auto it2 = *it->begin(); it2 != *it->end(); it2++ ){
			debug_lib::log( "item = %c", *it2 );
		}
	}
}

int main( int argc, char** argv){
	debug_lib::init(argv[0]);

	if( argc != 2 ){
		print_help();
		exit(1);
	}

	unordered_set<unorderd_set<char>*> bag;
	int i = 0;
	char* ptr = argv[1];
	for( i = 0; i < strlen(ptr); i++ ){
		bool inserted = false;
		for( auto it = bag.begin(); it != bag.end(); it++ ){
			if( *it->find(ptr[i]) == *it->end() ){
				*it->insert(ptr[i]);
				inserted = true;
			}
		}
		if ( inserted == false ){
			u = new unordered_set<char>();
			u.insert(ptr[i]);
			bag.insert(u);
		}
	}

	print_bag( bag );
	for( auto it = bag.begin(); it != bag.end(); it++ ){
		delete *it;
	}
	
	return 0;
}
