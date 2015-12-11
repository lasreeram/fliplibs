#include <iostream>
#include <string.h>
struct tevent_t
{
	int a;
	int b;
}

int main()
{
	char* ptr;
	memcpy( ptr, "hello", 5 );
	std::cout << ptr << std::endl;
	std::vector< int, tevent_t > vec;
	std::vector< int, tevent_t >::iterator iter;
	for ( iter = vec.begin(); iter != vec.end(); iter++ ){
		iter = 
	}
	
	return 0;
}
