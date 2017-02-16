#include <debug.h>
#include <Array.h>

int main(int argc, char** argv){
	debug_lib::init( argv[0]);
	
	AlgoBox::ArrayQueue<int> q1;
	q1.add(100);
	q1.add(200);
	q1.add(300);
	q1.add(400);

	for(int i=0; i < 4; i++ ){
		debug_lib::log( "value %d from queue", q1.remove() );
	}

	return 0;
}
