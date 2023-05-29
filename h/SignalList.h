#ifndef SIGNLIST_H_
#define SIGNLIST_H_

#include "Thread.h"
#include <stdio.h>
#include <stdlib.h>

typedef void (*SignalHandler)();
typedef unsigned SignalId;

class SignalListElement
{
public:
	SignalHandler handler;
	SignalListElement *next;

	SignalListElement(SignalHandler t)
	{
		handler = t;
		next = 0;
	}
};

class SignalList
{
public:
	SignalListElement *head;
	SignalListElement *tail;

	SignalList();
	virtual ~SignalList();

	void start();
	void execute();
	void add(SignalHandler t);
	void remove(SignalId id);
	void swap(SignalHandler handler1, SignalHandler handler2);
	void copy(SignalList *List);
};

#endif
