#include <debug.h>
#include <ArrayList.h>

int main(int argc, char** argv){
	debug_lib::init( argv[0]);
	
	AlgoBox::Stack<int> s1;
	s1.push(100);
	s1.push(200);
	s1.push(300);

	for(int i=0; i < 3; i++ ){
		//debug_lib::log( "popped value %d from stack", s1.pop() );
		s1.pop();
	}

	return 0;
}
