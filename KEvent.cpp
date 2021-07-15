#include "KEvent.h"
#include <dos.h>
#include "Def.h"
#include "PCB.h"
#include "Timer.h"
#include "IVTEntry.h"
#include "SCHEDULE.H"
KEvent* IVTEntry::events[256];
routine IVTEntry::routines[256];

KEvent::KEvent(PCB* pcbb,IVTNo ivtNo) {
    HARD_LOCK;
	pcb=pcbb;
    ivtno=ivtNo;
    IVTEntry::events[ivtno]=this;
    value=0;
    override();
    HARD_UNLOCK;
}

KEvent::~KEvent() {
    HARD_LOCK;
    IVTEntry::routines[ivtno]=0;
#ifndef BCC_BLOCK_IGNORE
    setvect(ivtno,oldroutine);
#endif
    HARD_UNLOCK;
}

void KEvent::override(){

#ifndef BCC_BLOCK_IGNORE
	oldroutine=getvect(ivtno);
	setvect(ivtno,IVTEntry::routines[ivtno]);
#endif
}

void KEvent::wait(){
    HARD_LOCK;
    if(PCB::running == pcb){
       if (value==1) value=0;
       else{
          PCB::running->status=UNRUNNABLE;
          HARD_UNLOCK;
          dispatch();
          HARD_LOCK;
       }
    }
    HARD_UNLOCK;
}
void KEvent::signal(){
	HARD_LOCK;
	if (pcb==0) value=1;
	else{
       pcb->status=RUNNABLE;
       Scheduler::put(pcb);
       pcb=0;
       HARD_UNLOCK;
       dispatch();
       HARD_LOCK;
	}
    HARD_UNLOCK;
}

