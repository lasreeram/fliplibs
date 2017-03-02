#include <debug.h>
#include <ArrayList.h>

int main(int argc, char** argv){
	debug_lib::init( argv[0]);
	
	AlgoBox::Array<int> a1(10);
	debug_lib::log( "size of a1 is %d", a1.getLength());
	a1[0] = 100;
	debug_lib::log( "size of a1 is %d", a1[0] );


	return 0;
}
