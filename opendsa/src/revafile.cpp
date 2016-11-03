#include <revafile.h>
#include <fstream>
#include <string>
#include <stack>
using namespace std;

int main( int argc, char** argv){

std::ifstream ifs;	
char buf[1024] = {0};
debug_lib::init(argv[0]);

ifs.open( argv[1], std::ifstream::in );

int recs = 0;
std::stack<std::string> revFile;
std::string line = "";
while( ifs.getline( buf, sizeof(buf) ) ){
	line = buf;
	//debug_lib::log( "%s", buf );
	revFile.push(line);
	memset(buf, '\0', sizeof(buf) );
	recs++;
}

while( !revFile.empty() ){
	debug_lib::log( "%s", revFile.top().c_str() );
	revFile.pop();
}

debug_lib::log("finished - processed %d records", recs );
return 0;
}
