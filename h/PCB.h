#ifndef _KLASA_PCB_PCB_H_
#define _KLASA_PCB_PCB_H_
#define LOCK    \
	PCB::cnt++; \
	;
#define UNLOCK PCB::cnt--;

#include "Queue.h"
#include "Def.h"
#include "List.h"
#include "Thread.h"
#include "SignList.h"
#include "Timer.h"

class Thread;
class Queue;
class signlist;

class PCB
{
private:
	friend void interrupt timer(...);
	friend class List;

public:
	typedef void (*SignalHandler)();
	typedef unsigned SignalId;

	int status;
	int bit;
	int prior[16];

	unsigned bp;
	unsigned sp;
	unsigned ss;
	unsigned block;
	unsigned zavrsio;
	unsigned *mySP;

	static int cnt;
	static unsigned globallblock;
	static ID broj;
	static PCB *running;
	static PCB *idle;
	static List list;

	ID threadID;
	Time period;
	Time pcbTimeSlice;
	StackSize pcbStackSize;
	Thread *myThread;
	PCB *parent;
	Queue *myQueue;
	signlist *niz[16];

	PCB(Thread *T);
	PCB(Thread *T, StackSize stackSize, Time timeSlice);
	virtual ~PCB();

	ID getID();

	static void wrapper();
	static void blockSignalGlobally(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	void run();
	void izvrsi();
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void signal(SignalId signal);
	void blockSignal(SignalId signal);
	void unblockSignal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);

	SignalHandler unisti();
};

#endif
