#include <Threads.h>
#include <debug.h>
#include <unistd.h>
#include <iostream>

using namespace pthreads_lib;
class Worker : public ThreadFunctor {
	public:
		int run(){
			debug_lib::log( "worker 1 executed successfully" );
			return 0;
		}

};

int main(int argc, char** argv){
	debug_lib::init( (char*)"IamMain", true );
	Worker* worker = new Worker();
	Thread* workerThread1 = new Thread(worker);
	debug_lib::log( "calling setname on worker 1" );
	workerThread1->setName("worker1");

	Thread* mainThread = new Thread(pthread_self());
	mainThread->setName("iAmMain");

	debug_lib::log("main thread id = %ld, worker1 thread id = %ld", 
				mainThread->getMyId(), workerThread1->getMyId() );
	debug_lib::log(	"main thread name = %s, worker1 thread name = %s", 
				mainThread->getName().c_str(), workerThread1->getName().c_str() );

	mainThread->join(workerThread1->getMyId());
	delete workerThread1;
	delete worker;
	delete mainThread;
	debug_lib::log ( "workers are done" );

	return 0;
}
