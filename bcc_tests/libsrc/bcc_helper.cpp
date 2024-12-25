#include <cstdlib>
#include <bcc_helper.h>

void* MyBccHelper::my_malloc_function(size_t val){
	return malloc(val);
}

