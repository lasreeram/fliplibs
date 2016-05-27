#include <Threads.h>
#include <debug.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
using namespace pthreads_lib;
pthreads_lib::Mutex* gMutex = NULL;
int balance = 100;
int gIndex = 0;

class WithdrawWorker : public ThreadFunctor {
	public:
		int run(){
			debug_lib::log( "executed withdraw successfully" );
			for(int i = 0; i < 5; i++){
				debug_lib::log( "doing work" );
				//gMutex->lock();
				balance--;
				//gMutex->unlock();
				sleep(2);
			}
			return 0;
		}
};
class DepositWorker : public ThreadFunctor {
	public:
		int run(){
			debug_lib::log( "executed deposit successfully" );
			for(int i = 0; i < 5; i++){
				debug_lib::log( "doing work" );
				//gMutex->lock();
				balance++;
				//gMutex->unlock();
				sleep(2);
			}
			return 0;
		}
};

int main(int argc, char** argv){
	debug_lib::init((char*)"IamMain", true);
	int numberOfThreads;
	if( argc < 2 ){
		debug_lib::log( "usage: manythreads <number of threads to run>" );
		exit(1);
	}else{
		numberOfThreads = atoi(argv[1]);
		debug_lib::log( "running %d threads", numberOfThreads );
	}
	DepositWorker* depositor = new DepositWorker();
	WithdrawWorker* withdrawer = new WithdrawWorker();

	gMutex = new Mutex(NULL);
	Thread* mainThread = new Thread(pthread_self());
	mainThread->setName("iAmMain");
	Thread** thptr = (Thread**) malloc( numberOfThreads * sizeof(Thread*) );
	char buf[64] = {0};
	for(int i = 0; i < numberOfThreads; i++ ){
		if( i % 2 == 0 )
			thptr[i] = new Thread(depositor);
		else
			thptr[i] = new Thread(withdrawer);
		sprintf( buf, "worker-%d", i );
		std::string str = buf;
		thptr[i]->setName(str);
	}

	for(int i = 0; i < numberOfThreads; i++ ){
		mainThread->join(thptr[i]->getMyId());	
		delete thptr[i];
	}

	debug_lib::log( "final balance = %d", balance );
	free(thptr);
	delete mainThread;
	delete depositor;
	delete withdrawer;

	return 0;
}
