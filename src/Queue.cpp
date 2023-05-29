#include "Queue.h"
#include "Def.h"
#include <stdio.h>
#include <stdlib.h>
#include "SCHEDULE.H"
Queue::Queue() {

    this->first=0;
    this->last=0;
    len=0;

}
Queue::~Queue() {
	while(!(this->first==0)){
		Elem* stari = this->first;
		this->first=this->first->next;
	    delete stari;
	    len--;
	}
}
void Queue::put(PCB* pcb){

    if (first==0){
    	first=last=new Elem(pcb);
    }
    else{
    	last=last->next=new Elem(pcb);
    }
    len++;
}
int Queue::size() const{
	return len;
}
PCB* Queue::get(){

	if (this->first == 0){ return 0; }
	Elem* temp=this->first;
	this->first=this->first->next;
	len--;
	PCB* pcb=temp->pcb;
	delete temp;
	return pcb;
}
PCB* Queue::get(ID id){
	Elem* cur=first;
	Elem* prev=0;
	while(cur!=0 && cur->pcb->getID()!=id){
		prev=cur;
		cur=cur->next;

	}
	if (cur != 0) {
		if (prev){
			prev->next=cur->next;
		}
		else{
			first=cur->next;

		}
		if (cur->next==0){
			last=prev;
		}
		PCB* pcb=cur->pcb;//Ovo je menjano stojalo je return cur->pcb;

		delete cur;
		return pcb;
	}
	else return 0;
}
