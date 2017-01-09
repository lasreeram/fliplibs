#include <revafile.h>
#include <fstream>
#include <string>
#include <stack>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
using namespace std;

void print_help(){
	cout << "usage: matchstr <inputstr>\n";
}

bool ispush(char ch){
	if( ch == '{' )
		return true;
	if( ch == '[' )
		return true;
	if( ch == '(' )
		return true;

	return false;
}

bool isDyckExpression( char* ptr, int len ){
	if( len <= 0 )
		return true;
	
	int i = 0;
	int res = 0;
	while( i < len ){
		if( ispush(ptr[i]) )
			res++;
		else
			res--;
		i++;
	}
	if( res >= 0 )
		return isDyckExpression(ptr, len-1);

	return false;
}

char inverse( char ch ){
	if( ch == '{' )
		return '}';
	if( ch == '[' )
		return ']';
	if( ch == '(' )
		return ')';

	return '\0';
}

int main( int argc, char** argv){
	std::ifstream ifs;	
	char buf[1024] = {0};
	debug_lib::init(argv[0]);

	if( argc != 2 ){
		print_help();
		exit(1);
	}


	int i = 0;
	char* ptr = argv[1];
	if( !isDyckExpression(ptr, strlen(ptr)) ){
		debug_lib::log( "is not a dyck expression" );
		exit(1);
	}
	stack<char> matchstack;
	while( i < strlen(ptr) ){
		if( ispush(ptr[i]) ){
			debug_lib::log( "push %c", ptr[i] );
			matchstack.push(ptr[i]);
		}
		else if ( ptr[i] == inverse(matchstack.top()) ){
			debug_lib::log( "pop %c", matchstack.top() );
			matchstack.pop();
		}
		i++;
	}

	if( !matchstack.empty() ){
		debug_lib::log( "pop %c", matchstack.top() );
		debug_lib::log( "match expression not correct" );
	}else{
		debug_lib::log( "match expression is correct" );
	}
	return 0;
}
