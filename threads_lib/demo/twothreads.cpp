#include <Threads.h>
#include <debug.h>
#include <unistd.h>
#include <iostream>

using namespace pthreads_lib;
class Worker : public ThreadFunctor {
	public:
		int run(){
			debug_lib::log( "executed successfully" );
			for(int i = 0; i < 20; i++){
				debug_lib::log( "doing work" );
				sleep(2);
			}
		}
};

int main(int argc, char** argv){
	debug_lib::init((char*)"IamMain", true);

	Worker* worker = new Worker();
	Thread* workerThread1 = new Thread(worker);
	workerThread1->setName("worker1");

	Thread* mainThread = new Thread(pthread_self());
	mainThread->setName("iAmMain");

	Thread* workerThread2 = new Thread(worker);
	workerThread2->setName("worker2");

	mainThread->join(workerThread1->getMyId());
	mainThread->join(workerThread2->getMyId());


	delete workerThread1;
	delete workerThread2;
	delete mainThread;
	debug_lib::log( "workers are done" );

	return 0;
}
