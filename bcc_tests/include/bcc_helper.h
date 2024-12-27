
#ifndef MYBCC_HELPER_LIBRARY_H
#define MYBCC_HELPER_LIBRARY_H
#include <cstddef>

class MyBccHelper{
	public:	
		void* my_malloc_function(size_t val);
		void my_free_function(char* ptr);

};
#endif
