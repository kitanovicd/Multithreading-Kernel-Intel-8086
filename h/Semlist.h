#ifndef SEMLIST_H_
#define SEMLIST_H_

#include "Semaphor.h"
#include "PCB.h"
#include "KerSem.h"

class Element
{
public:
	Time timeToWait;
	PCB *pcb;
	Element *next;
	KernelSem *sem;

	Element(PCB *t, Time t1, KernelSem *semm)
	{
		pcb = t;
		next = 0;
		timeToWait = t1;
		sem = semm;
	}
};

class List
{
private:
	Element *first;
	Element *last;

public:
	friend class KernelSem;

	List();
	virtual ~List();

	void remove(ID id);
	void add(PCB *t, Time t1, KernelSem *semm);
	PCB *find(ID id);
};

#endif
