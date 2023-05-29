#ifndef KLASA_QUEUE_QUEUE_H_
#define KLASA_QUEUE_QUEUE_H_

#include "PCB.h"
#include "Def.h"

class PCB;

class QueueElement
{
public:
	PCB *pcb;
	QueueElement *next;

	QueueElement(PCB *pcbb)
	{
		pcb = pcbb;
		next = 0;
	}
};

class Queue
{
private:
	int len;
	QueueElement *head;
	QueueElement *tail;

public:
	Queue();
	virtual ~Queue();

	int size() const;
	void put(PCB *pcb);

	PCB *get();
	PCB *get(ID id);
};

#endif
