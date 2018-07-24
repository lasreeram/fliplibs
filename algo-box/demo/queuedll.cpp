#include <debug.h>
#include <LinkedList.h>

int main(int argc, char** argv){
	debug_lib::init( argv[0]);
	
	AlgoBox::DoublyLinkedList<int> q1;
	//q1.addToQueue(100);
	//q1.addToQueue(200);
	//q1.addToQueue(300);
	//q1.addToQueue(400);

	for(int i=0; i < 4; i++ ){
		//debug_lib::log( "value %d from queue", q1.getNext() );
	}

	return 0;
}
