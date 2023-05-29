#ifndef SEMLIST_H_
#define SEMLIST_H_

#include "Semaphor.h"
#include "PCB.h"
#include "KerSem.h"

class SemaphoreListElement
{
public:
	Time timeToWait;
	PCB *pcb;
	SemaphoreListElement *next;
	KernelSem *sem;

	SemaphoreListElement(PCB *t, Time t1, KernelSem *semm)
	{
		pcb = t;
		next = 0;
		timeToWait = t1;
		sem = semm;
	}
};

class SemaphoreList
{
private:
	SemaphoreListElement *head;
	SemaphoreListElement *tail;

public:
	friend class KernelSem;

	SemaphoreList();
	virtual ~SemaphoreList();

	void remove(ID id);
	void add(PCB *t, Time t1, KernelSem *semm);
	PCB *find(ID id);
};

#endif
