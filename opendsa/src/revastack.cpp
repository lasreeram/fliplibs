#include <revafile.h>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
using namespace std;

void print_help(){
	cout << "usage: revastack <inputstr>\n";
}

void print_stack( stack<char> s ){
	int i = 0;
	while( !s.empty() ){
		i++;
		debug_lib::log( "element %d = %c", i, s.top() );
		s.pop();
	}
}

int main( int argc, char** argv){
	debug_lib::init(argv[0]);

	if( argc != 2 ){
		print_help();
		exit(1);
	}


	stack<char> s1;
	int i = 0;
	char* ptr = argv[1];
	for( i = 0; i < strlen(ptr); i++ ){
		s1.push(ptr[i]);
	}
	debug_lib::log( "before reversing" );
	print_stack( s1 );

	queue<char> q1;
	while( !s1.empty() ){
		q1.push(s1.top());
		s1.pop();
	}

	while ( !q1.empty() ){
		s1.push(q1.front());
		q1.pop();
	}

	debug_lib::log( "after reversing" );
	print_stack( s1 );

	return 0;
}
