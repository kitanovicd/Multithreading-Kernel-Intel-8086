#ifndef KLASA_THREAD_THREAD_H_
#define KLASA_THREAD_THREAD_H_

#include <stdio.h>
#include "PCB.h"
#include "Def.h"
#include "Timer.h"

typedef void (*SignalHandler)();
typedef unsigned SignalId;

class PCB;

class Thread
{
private:
	PCB *myPCB;

protected:
	friend class PCB;
	friend void inic();
	friend int main(int argc, char *argv[]);

	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);

	virtual void run();

public:
	void start();
	void waitToComplete();

	virtual ~Thread();

	ID getID();
	static ID getRunningId();
	static Thread *getThreadById(ID id);
	static void blockSignalGlobally(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void signal(SignalId signal);
	void blockSignal(SignalId signal);
	void unblockSignal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
};

#endif
