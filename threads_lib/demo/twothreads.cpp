#include <Threads.h>
#include <debug.h>
#include <unistd.h>
#include <iostream>

void* dowork(void* ctxt){
	debug_lib::log( "executed successfully" );
	return NULL;
}

using namespace pthreads_lib;
int main(int argc, char** argv){
	debug_lib::init(argv[0], true);

	Thread* worker1 = new Thread(dowork, NULL, NULL);
	worker1->setName("worker1");

	Thread* mainThread = new Thread(pthread_self());
	mainThread->setName("iAmMain");

	Thread* worker2 = new Thread(dowork, NULL, NULL);
	worker2->setName("worker2");

	mainThread->join(worker1->getMyId(), NULL);
	mainThread->join(worker2->getMyId(), NULL);


	delete worker1;
	delete worker2;
	delete mainThread;

	return 0;
}
