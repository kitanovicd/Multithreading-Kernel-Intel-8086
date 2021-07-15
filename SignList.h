#ifndef SIGNLIST_H_
#define SIGNLIST_H_
#include "Thread.h"
#include <stdio.h>
#include <stdlib.h>
typedef void (*SignalHandler)();
typedef unsigned SignalId;


class Elementt{
public:
    SignalHandler handler;
    Elementt* next;
    Elementt(SignalHandler t){
    	handler=t;
    	next=0;
    }
};

class signlist {
public:
	signlist();
	virtual ~signlist();

	void add(SignalHandler t);
	void remove(SignalId id);
	void start();
	void swap(SignalHandler hand1,SignalHandler hand2);
	void copy(signlist* lista);
	void izvrsi();
public:
	Elementt* first;
	Elementt* last;
};

#endif
