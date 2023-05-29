#ifndef KERNELSEM_H_
#define KERNELSEM_H_

#include "Queue.h"
#include "Semaphor.h"
#include "Semlist.h"
#include "Timer.h"

class Lista;

class KernelSem
{
private:
	int value;
	Queue *waitQueue;

public:
	friend class Semaphore;

	static Lista lista;

	KernelSem(int init);

	static void trazi();
	int val() const;
	void staviga(PCB *pcb);

	virtual ~KernelSem();
	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n = 0);
};

#endif
