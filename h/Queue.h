#ifndef KLASA_QUEUE_QUEUE_H_
#define KLASA_QUEUE_QUEUE_H_

#include "PCB.h"
#include "Def.h"
class PCB;

class Elem{
public:
    PCB* pcb;
    Elem* next;
    Elem(PCB* pcbb){
    	pcb=pcbb;
    	next=0;
    }
};


class Queue {
public:
	Queue();
	virtual ~Queue();

	void put(PCB* pcb);
	PCB* get();
	int size() const;
	PCB* get(ID id);
private:
	int len;
	Elem* first;
	Elem* last;
};
#endif
