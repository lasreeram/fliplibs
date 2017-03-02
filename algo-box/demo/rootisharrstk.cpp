#include <debug.h>
#include <ArrayList.h>

int main(int argc, char** argv){
	debug_lib::init( argv[0]);
	
	AlgoBox::RootishArrayStack<long> a1;

	try{
		for( int i = 0; i < 10000; i++ )
			a1.add(i, 10*(i+1));
		//for( int i = 0; i < 10000; i++ )
		//	debug_lib::log( "value of index %d = %ld", i, a1.get(i));
		//debug_lib::log( "x.value of index %d = %ld", 0, a1.get(0));
		//a1.add(1,100);
		//debug_lib::log( "y.value of index %d = %ld", 0, a1.get(0));
		//debug_lib::log( "z.value of index %d = %ld", 1, a1.get(1));

		for( int i = 10000-1; i >= 0 ; i-- )
			if( i >= a1.getSize() )
				break;
			else
				a1.remove(i);
	}catch(debug_lib::Exception& e){
		debug_lib::log( "error %s", e.what() );
	}
	return 0;
}
