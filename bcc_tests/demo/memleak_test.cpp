#include <iostream>
#include <unistd.h>
#include <bcc_helper.h>
#include <random>

int main(){
	MyBccHelper helper;

	/*
	  # track the pid for leaks
	  memleak-bpfcc -p $(pidof memleak_test)

	  # track the pid for leaks and accumulate allocations due to same stack
	  memleak-bpfcc -p $(pidof memleak_test) -a

	  # track the pid for leaks, trace entry and exits in to alloc functions
	  memleak-bpfcc -p $(pidof memleak_test) -t

	  # track the pid for leaks older than 2 secs 
	  memleak-bpfcc -p $(pidof memleak_test) -o 2000

	  # track every 5th allocation
	  memleak-bpfcc -p $(pidof memleak_test) -s 5
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
