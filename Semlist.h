#ifndef SEMLIST_H_
#define SEMLIST_H_
#include "Semaphor.h"
#include "PCB.h"
#include "KerSem.h"


class Element{
public:
	Time timeToWait;
    PCB* pcb;
    Element* next;
    KernelSem* sem;
    Element(PCB* t,Time t1,KernelSem* semm){
    	pcb=t;
    	next=0;
    	timeToWait=t1;
    	sem=semm;
    }
};

class Lista {
public:
	friend class KernelSem;
	Lista();
	virtual ~Lista();

	void add(PCB* t,Time t1,KernelSem* semm);
	PCB* find(ID id);
	void remove(ID id);

private:
	Element* first;
	Element* last;
};
#endif
