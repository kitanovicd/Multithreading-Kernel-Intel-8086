#ifndef LIST_LIST_H_
#define LIST_LIST_H_

#include "Thread.h"
#include "Def.h"
#include <stdio.h>

class PCB;

class ListElement
{
public:
	PCB *pcb;
	ListElement *next;

	ListElement(PCB *t)
	{
		pcb = t;
		next = 0;
	}
};

class List
{
public:
	ListElement *head;
	ListElement *tail;

	List();
	virtual ~List();

	void add(PCB *t);
	void remove(ID id);
	PCB *find(ID id);
};

#endif
