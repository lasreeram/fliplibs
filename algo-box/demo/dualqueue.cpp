#include <debug.h>
#include <Array.h>

int main(int argc, char** argv){
	debug_lib::init( argv[0]);
	
	AlgoBox::DualArrayDeQueue<int> q1;
	q1.addFirst(100);
	q1.addFirst(200);
	q1.addFirst(300);
	q1.addFirst(400);
	q1.addFirst(500);
	q1.addFirst(600);
	q1.addLast(1300);
	q1.addLast(1400);
	q1.addLast(1500);
	q1.addLast(1600);
	q1.addLast(1700);
	q1.addLast(1800);

	debug_lib::log( "first value is %d", q1.getFirst() );
	debug_lib::log( "last value is %d", q1.getLast() );
	debug_lib::log( "remove first value is %d", q1.removeFirst() );
	debug_lib::log( "first value is %d", q1.getFirst() );

	return 0;
}
