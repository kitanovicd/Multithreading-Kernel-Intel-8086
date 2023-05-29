#include "Thread.h"
#include "stdio.h"
#include "PCB.h"
#include "SCHEDULER.H"
#include "Timer.h"
#include "Def.h"
#include "List.h"
#include <stdlib.h>

class SCHEDULER;
void Thread::run() {}

Thread::Thread(StackSize stackSize, Time timeSlice)
{
    myPCB = new PCB(this, stackSize, timeSlice);
}

Thread::~Thread()
{
    waitToComplete();
    HARD_LOCK;
    delete myPCB;
    HARD_UNLOCK;
}

void Thread::start()
{
    HARD_LOCK;
    myPCB->status = RUNNABLE;
    Scheduler::put(myPCB);
    HARD_UNLOCK;
}

void Thread::waitToComplete()
{
    HARD_LOCK;

    if (myPCB == PCB::idle || myPCB->status == STOP)
    {
        HARD_UNLOCK;
        return;
    }

    PCB::running->status = UNRUNNABLE;
    myPCB->myQueue->put(PCB::running);

    HARD_UNLOCK;
    dispatch();
}

ID Thread::getID()
{
    return myPCB->getID();
}

ID Thread::getRunningId()
{
    return PCB::running->getID();
}

Thread *Thread::getThreadById(ID id)
{
    return (PCB::list.find(id))->myThread;
}

void Thread::signal(SignalId signal)
{
    myPCB->signal(signal);
}
void Thread::registerHandler(SignalId signal, SignalHandler handler)
{
    myPCB->registerHandler(signal, handler);
}
void Thread::unregisterAllHandlers(SignalId id)
{
    myPCB->unregisterAllHandlers(id);
}
void Thread::swap(SignalId id, SignalHandler handler1, SignalHandler handler2)
{
    myPCB->swap(id, handler1, handler2);
}
void Thread::blockSignal(SignalId signal)
{
    myPCB->blockSignal(signal);
}
void Thread::blockSignalGlobally(SignalId signal)
{
    PCB::blockSignalGlobally(signal);
}
void Thread::unblockSignal(SignalId signal)
{
    myPCB->unblockSignal(signal);
}
void Thread::unblockSignalGlobally(SignalId signal)
{
    PCB::unblockSignalGlobally(signal);
}
