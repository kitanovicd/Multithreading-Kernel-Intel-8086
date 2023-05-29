#include <stdlib.h>
#include "Semaphor.h"
#include "KerSem.h"
#include "Timer.h"
Semaphore::Semaphore(int init) {
	HARD_LOCK;
	myImpl=new KernelSem(init);
    HARD_UNLOCK;
}

Semaphore::~Semaphore() {
	delete myImpl;
}

int Semaphore::val() const{
	return myImpl->val();
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}
