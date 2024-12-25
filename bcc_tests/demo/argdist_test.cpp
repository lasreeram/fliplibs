#include <iostream>
#include <unistd.h>
#include "bcc_helper.h"
#include <random>

int main(){
	MyBccHelper helper;

	/*
	  #Histogram of malloc calls by size
	  argdist-bpfcc -p <pid> -c -H 'p:c:malloc(size_t size):size_t:size#malloc size'

	  #Count of malloc calls by size
	  argdist-bpfcc -p <pid> -c -C 'p:c:malloc(size_t size):size_t:size#malloc size'

	  #Count of clock_nanosleep and print clockid argument
	  argdist-bpfcc -p 62529 -c -H 'p:c:clock_nanosleep(clockid_t clockid, int flags,const struct timespec *t, struct timespec *_Nullable remain):clockid_t:clockid#clockid'

	  #Count of clock_nanosleep and print flags
	  argdist-bpfcc -p 62529 -c -H 'p:c:clock_nanosleep(clockid_t clockid, int flags,const struct timespec *t, struct timespec *_Nullable remain):int:flags#flags'

	  #Count of clock_nanosleep and print timespec members. Note the inclusion of struct_timespec.h to BCC.
	  argdist-bpfcc -p 7823 -I '/home/aardra/flip_libs/fliplibs/bcc_tests/demo/struct_timespec.h' -c -C 'p:c:nanosleep(struct timespec *req):long:req->tv_sec'
	  
	 */
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(1, 1000); // define the range
	for (int idx = 0; idx < 1000; idx++ ){
	  int alloc_size = distr(gen);
	  char* ptr = (char*)helper.my_malloc_function(alloc_size);
	  std::cout << "allocated " << alloc_size << " bytes" << std::endl;
	  sleep(5);
	}

}
