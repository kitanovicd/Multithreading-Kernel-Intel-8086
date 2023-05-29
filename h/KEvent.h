#ifndef KEVENT_H_
#define KEVENT_H_
#include "Event.h"
#include "PCB.h"
#include "IVTEntry.h"

class IVTEntry;

class KEvent
{
private:
   int value;
   routine oldroutine;
   IVTNo ivtno;
   PCB *pcb;

public:
   KEvent(PCB *pcbb, IVTNo ivtNo);
   ~KEvent();
   void wait();
   void signal();
   void override();
};

#endif /* KEVENT_H_ */
