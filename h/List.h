#ifndef LIST_LIST_H_
#define LIST_LIST_H_

#include "Thread.h"
#include "Def.h"
#include <stdio.h>

class PCB;

class El
{
public:
	PCB *pcb;
	El *next;

	El(PCB *t)
	{
		pcb = t;
		next = 0;
	}
};

class List
{
public:
	El *first;
	El *last;

	List();
	virtual ~List();

	void add(PCB *t);
	void remove(ID id);
	PCB *find(ID id);
};

#endif
