#include <revafile.h>
#include <fstream>
#include <string>
#include <stack>
#include <iostream>
#include <stdlib.h>
using namespace std;

void print_stack(std::stack<std::string>& stack1){
	while( !stack1.empty() ){
		debug_lib::log( "%s", stack1.top().c_str() );
		stack1.pop();
	}
}

void print_help(){
	cout << "usage: fifty <inputfile> <num-at-which-to-print>\n";
}

int main( int argc, char** argv){
	std::ifstream ifs;	
	char buf[1024] = {0};
	debug_lib::init(argv[0]);

	if( argc != 3 ){
		print_help();
		exit(1);
	}

	ifs.open( argv[1], std::ifstream::in );

	int recs = 0;
	int printAt = atoi(argv[2]);
	std::stack<std::string> stack1;
	std::string line = "";
	while( ifs.getline( buf, sizeof(buf) ) ){
		line = buf;
		//debug_lib::log( "%s", buf );
		stack1.push(line);
		memset(buf, '\0', sizeof(buf) );
		recs++;
		if( (recs%printAt) == 0){
			print_stack(stack1);			
		}
	}

	print_stack(stack1);
	debug_lib::log("finished - processed %d records", recs );
	return 0;
}
