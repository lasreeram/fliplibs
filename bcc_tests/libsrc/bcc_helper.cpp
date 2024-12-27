#include <cstdlib>
#include <bcc_helper.h>

void* MyBccHelper::my_malloc_function(size_t val){
	return malloc(val);
}

void MyBccHelper::my_free_function(char* ptr){
  free(ptr);
}

