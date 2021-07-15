#include "KerSem.h"
#include "Semaphor.h"
#include "Queue.h"
#include "SCHEDULE.H"
#include "Timer.h"
#include "Semlist.h"
#include "PCB.h"
#include "Def.h"
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
Lista KernelSem::lista;
KernelSem::KernelSem(int init) {
	value=init;
	waitQueue=new Queue();
}

KernelSem::~KernelSem(){
	HARD_LOCK;
    delete waitQueue;
    HARD_UNLOCK;
}

int KernelSem::val() const{
	return value;
}

int KernelSem::signal(int n){
    HARD_LOCK;

	int i=0;
	if (n<0){
		HARD_UNLOCK;
		return n;
	}
	else{
		if (n==0){
		   if (value + 1 <=0){
              PCB* temp=waitQueue->get();
		      lista.remove(temp->getID());

		      if (temp) { temp->bit=1;}
              if (temp) temp->status=RUNNABLE;
              i++;
              Scheduler::put(temp);
              n--;
              value++;
		   }else ++value;
        }else{
		   value+=n;
           while(n>0 || waitQueue->size()!=0){
               PCB* t=waitQueue->get();
               lista.remove(t->getID());
               t->status=RUNNABLE;
               i++;
               n--;
               Scheduler::put(t);
           }
        }
	}
	HARD_UNLOCK;
    return i;
}
int KernelSem::wait(Time maxTimeToWait){
    HARD_LOCK;
	value--;
    if (value<0){
        PCB::running->status=UNRUNNABLE;
    	waitQueue->put(PCB::running);
        if (maxTimeToWait>0) lista.add(PCB::running,maxTimeToWait,this);

        HARD_UNLOCK;
        dispatch();
        HARD_LOCK;
        if (maxTimeToWait==0) {
        	HARD_UNLOCK;
        	return 1;
        }
        else{
        	if (PCB::running->bit == 1){
        		PCB::running->bit=0;
        		HARD_UNLOCK;
        		return 1;
        	}
        	else {
        		HARD_UNLOCK;
        		return 0;
        	}
        }
    }
	HARD_UNLOCK;
	return 1;
}

void KernelSem::staviga(PCB *pcbb) {
    PCB *pcb = waitQueue->get(pcbb->getID());
    ++value;
    pcb->status = RUNNABLE;
    Scheduler::put(pcb);
}
void KernelSem::trazi(){
    HARD_LOCK;
    if ((lista.first)){
    Element* tek;
    --(lista.first->timeToWait);
    while (lista.first && lista.first->pcb && lista.first->timeToWait == 0) {
        tek = lista.first;
        lista.first = lista.first->next;
        tek->sem->staviga(tek->pcb);
        delete tek;
    }
    }
    HARD_UNLOCK;
}
