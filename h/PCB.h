#ifndef _KLASA_PCB_PCB_H_
#define _KLASA_PCB_PCB_H_
#include "Queue.h"
#include "Def.h"
#include "List.h"
#include "Thread.h"
#include "SignList.h"
#include "Timer.h"
class Thread;
class Queue;
class signlist;
#define LOCK PCB::cnt++;;
#define UNLOCK PCB::cnt--;
class PCB {
private:
	friend void interrupt timer(...);
	friend class List;
public:
	static PCB* running;
	static PCB* idle;
	static ID broj;
	static List list;
	unsigned bp;
    unsigned sp;
    unsigned ss;
	unsigned zavrsio;
	typedef void (*SignalHandler)();
	typedef unsigned SignalId;
    static int cnt;

	ID threadID;
	Time period;
	Time pcbTimeSlice;
	StackSize pcbStackSize;
	unsigned* mySP;
	Thread* myThread;
    PCB* parent;
	Queue* myQueue;
	int status;
    int bit;
    PCB(Thread* T);
	PCB(Thread* T,StackSize stackSize,Time timeSlice);
	virtual ~PCB();

	ID getID();

	static void wrapper();
	void run();


	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
	void izvrsi();
	SignalHandler unisti();
	signlist* niz[16];
    int prior[16];

	unsigned block;
	static unsigned globallblock;
};

#endif
