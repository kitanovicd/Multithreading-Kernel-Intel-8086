#ifndef KEVENT_H_
#define KEVENT_H_
#include "Event.h"
#include "PCB.h"
#include "IVTEntry.h"
class IVTEntry;
class KEvent {
public:

   KEvent (PCB* pcbb,IVTNo ivtNo);
   ~KEvent ();
   void wait ();
   void signal();
   void override();
   routine oldroutine;
private:

   IVTNo ivtno;
   PCB* pcb;
   int value;
};

#endif /* KEVENT_H_ */
