#include <debug.h>
#include <Array.h>

int main(int argc, char** argv){
	debug_lib::init( argv[0]);
	
	AlgoBox::FastArrayStack<long> a1;
	for( int i = 0; i < 100000; i++ )
		a1.add(i, i*10);

	try{
	for( int i = 100000-1; i >= 0; i-- )
		if( i >= a1.getSize() )
			break;
		else
			a1.remove(i);
	}catch(debug_lib::Exception& e){
		debug_lib::log( "error = %s", e.what() );
	}

	return 0;
}
