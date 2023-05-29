#ifndef KERNELSEM_H_
#define KERNELSEM_H_

#include "Queue.h"
#include "Semaphor.h"
#include "Semlist.h"
#include "Timer.h"
class Lista;

class KernelSem {
public:
	friend class Semaphore;
	KernelSem(int init);
	virtual ~KernelSem();

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n=0);
    int val() const;
    //void remove(ID id);
    static Lista lista;
    void staviga(PCB* pcb);
    static void trazi();
private:

	int value;
	Queue* waitQueue;
};

#endif
