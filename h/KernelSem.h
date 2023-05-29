#ifndef KERNELSEM_H_
#define KERNELSEM_H_

#include "Queue.h"
#include "Semaphor.h"
#include "SemaphoreList.h"
#include "Timer.h"

class List;

class KernelSem
{
private:
	int value;
	Queue *waitQueue;

public:
	friend class Semaphore;

	static List list;

	KernelSem(int init);
	static void search();

	int val() const;
	void put(PCB *pcb);

	virtual ~KernelSem();
	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n = 0);
};

#endif
