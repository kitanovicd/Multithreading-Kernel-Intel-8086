#include "PCB.h"
#include "Timer.h"
#include "Def.h"
#include <dos.h>
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "List.h"
#include "SCHEDULE.H"
PCB* PCB::running;
PCB* PCB::idle;
ID PCB::broj=0;
List PCB::list;
unsigned PCB::globallblock=0;
int PCB::cnt=0;
SignalHandler PCB::unisti(){
	LOCK;
	while(myQueue->size() > 0){
	    PCB* temp=myQueue->get();
	    temp->status=RUNNABLE;
	    Scheduler::put(temp);
	}
	status=STOP;
	if (parent) parent->signal(1);
	//signal(2);
	UNLOCK;
	dispatch();
}
PCB::PCB(Thread* T):parent(0),pcbStackSize(0),pcbTimeSlice(0),myQueue(0),mySP(0),zavrsio(0),bp(0),ss(0),sp(0),bit(0){
	//HARD_LOCK;
	myThread=T;
   	threadID=broj;
   	period=0.0;
   	status=RUNNABLE;

   	for (int i=0; i<16; i++) {
   		niz[i]=0;
   	    prior[i]=-1;
   	}
   	block=0x0000;
   	parent=0;
   	list.add(this);
   	//HARD_UNLOCK;
}

PCB::PCB(Thread* T,StackSize stackSize,Time timeSlice){
	//Konstruktor nije zavrsen
	HARD_LOCK;
	myThread=T;
	parent=PCB::running;
	pcbStackSize=stackSize/2;
	pcbTimeSlice=timeSlice;
	myQueue = new Queue();
    mySP=new unsigned[pcbStackSize];
	period=0.0;
	threadID=++broj;
	zavrsio=0;
	status=NEW;
	bp=0;
	for (int i=0; i<16; i++){
	    if (PCB::running->niz[i]==0) niz[i]=0;
	    else {
	    	niz[i]=new signlist();
	    	niz[i]->copy(PCB::running->niz[i]);
	    }
	    prior[i]=PCB::running->prior[i];
	 }
	 block=PCB::running->block;


#ifndef BCC_BLOCK_IGNORE


	mySP[pcbStackSize -1]=CPU_FLAGS_INIT_VALUE;
	mySP[pcbStackSize -2]=FP_SEG(&(PCB::wrapper));
	mySP[pcbStackSize -3]=FP_OFF(&(PCB::wrapper));
	ss=FP_SEG(mySP + pcbStackSize - 12);
	sp=FP_OFF(mySP + pcbStackSize - 12);
#endif
	list.add(this);
	HARD_UNLOCK;
}

PCB::~PCB() {

	PCB::list.remove(getID());
	if (mySP!=0) delete mySP;
}
ID PCB::getID(){
	return threadID;
}
void PCB::wrapper(){
   running->run();
}
void PCB::run(){
	myThread->run();
	HARD_LOCK;

	while(myQueue->size() > 0){
		PCB* temp=myQueue->get();
        temp->status=RUNNABLE;
		Scheduler::put(temp);
	}
	status=STOP;
	if (parent) parent->signal(1);
	if (((1U << 2) & block) == 0){
		if (((1U << 2) & globallblock) == 0){
	        if (niz[2] != 0) niz[2]->izvrsi();
		}
	}
	HARD_UNLOCK;
    dispatch();
}
void PCB::signal(SignalId signal){
    LOCK;
    if ((signal>=0) && (signal<16)) {
        int i=0;
    	while(i<16 && prior[i]!=-1){
        	i++;
        }
    	if (i<16) prior[i]=signal;
    }
    UNLOCK;
}
void PCB::registerHandler(SignalId signal, SignalHandler handler){
	LOCK;
	if (signal>=0 && signal<16){
	   if (niz[signal]==0) niz[signal]=new signlist();
	   niz[signal]->add(handler);
	}
	UNLOCK;;
}
void PCB::unregisterAllHandlers(SignalId id){
	LOCK;
	if (id>=0 && id<16){
	    delete niz[id];
        niz[id]=0;
	}
	UNLOCK;
}
void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	LOCK;
	if (id>=0 && id<16) if (niz[id] != 0) niz[id]->swap(hand1,hand2);
    UNLOCK;
}
void PCB::blockSignal(SignalId signal){
    LOCK;
	if (signal>=0 && signal<16){
		block |= (1<<signal);
	}
	UNLOCK;
}
void PCB::blockSignalGlobally(SignalId signal){
	LOCK;
	if (signal>=0 && signal<16){
	    globallblock=globallblock | (1<<signal);
	}
	UNLOCK;
}
void PCB::unblockSignal(SignalId signal){
	LOCK;
	if (signal>=0 && signal<16){
    	unsigned maska=65534<<signal;
        block=block & maska;
    }
	UNLOCK;
}
void PCB::unblockSignalGlobally(SignalId signal){
	LOCK;
	if (signal>=0 && signal<16){
	    unsigned maska=65535;
        maska=maska & (65534<<signal);
	    globallblock=globallblock & maska;
	}
	UNLOCK;
}
void PCB::izvrsi(){
	LOCK;

	int i=0;
	while(i<16){
		if (prior[i] != -1){
		if (((1U << prior[i]) & block) == 0){
			if (((1U << prior[i]) & globallblock) == 0){
				if (prior[i]==0) unisti();
				else {niz[prior[i]]->izvrsi();}
                prior[i]=-1;
			}
		}
	    }
		i++;
	}
	i=0;
	int j=0;
	while(i<16){
		if (prior[i] != -1){
			if (!(i==j)){
				prior[j]=prior[i];
			}
			int k=prior[j];
			prior[i]=-1;
			if (i==j) prior[i]=k;
			j++;
		}
		i++;
	}
	UNLOCK;
}
