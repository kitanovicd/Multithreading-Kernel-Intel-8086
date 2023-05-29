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
#include "SignalList.h"
#include "Timer.h"

class Thread;
class Queue;
class SignalList;

class PCB
{
private:
	friend void interrupt timer(...);
	friend class List;

public:
	typedef void (*SignalHandler)();
	typedef unsigned SignalId;

	int bit;
	int status;
	int priorities[16];

	unsigned bp;
	unsigned sp;
	unsigned ss;
	unsigned block;
	unsigned finished;
	unsigned *mySP;

	static ID id;
	static int cnt;
	static unsigned globalBlock;
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
	SignalList *signalLists[16];

	PCB(Thread *T);
	PCB(Thread *T, StackSize stackSize, Time timeSlice);
	virtual ~PCB();

	ID getID();

	static void wrapper();
	static void blockSignalGlobally(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	void run();
	void execute();
	void swap(SignalId id, SignalHandler handler1, SignalHandler handler2);
	void signal(SignalId signal);
	void blockSignal(SignalId signal);
	void unblockSignal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId signal);

	SignalHandler destroy();
};

#endif
