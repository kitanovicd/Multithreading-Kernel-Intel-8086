#ifndef KLASA_THREAD_THREAD_H_
#define KLASA_THREAD_THREAD_H_
#include <stdio.h>
#include "PCB.h"
#include "Def.h"
#include "Timer.h"
typedef void (*SignalHandler)();
typedef unsigned SignalId;
class PCB;

class Thread {

public:

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getID();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

    void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

protected:
	friend class PCB;
	friend int main(int argc, char* argv[]);
	friend void inic();
	Thread(StackSize stackSize = defaultStackSize,Time timeSlice = defaultTimeSlice);
	virtual void run();

private:
	PCB* myPCB;
};

#endif
