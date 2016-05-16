#include <Threads.h>
#include <debug.h>
#include <unistd.h>
#include <iostream>

void* dowork(void* ctxt){
	debug_lib::log( "worker 1 executed successfully" );
	return NULL;
}

using namespace pthreads_lib;
int main(int argc, char** argv){
	debug_lib::init( "IamMain", true );
	Thread* worker1 = new Thread(dowork, NULL, NULL);
	debug_lib::log( "calling setname on worker 1" );
	worker1->setName("worker1");

	Thread* mainThread = new Thread(pthread_self());
	mainThread->setName("iAmMain");

	debug_lib::log("main thread id = %ld, worker1 thread id = %ld", 
				mainThread->getMyId(), worker1->getMyId() );
	debug_lib::log(	"main thread name = %s, worker1 thread name = %s", 
				mainThread->getName().c_str(), worker1->getName().c_str() );

	mainThread->join(worker1->getMyId(), NULL);
	delete worker1;
	delete mainThread;

	return 0;
}
