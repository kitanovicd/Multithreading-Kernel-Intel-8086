/*
 * Semlist.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#include "Semlist.h"
#include "Def.h"
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

List::List() {
	first=last=0;
}

List::~List() {

	while(first){
		Element* stari=first;
		first=first->next;
		delete stari;
	}

}
void List::add(PCB* t,Time t1,KernelSem* semm){


    Element* cur=first;
    Element* pret=0;
    while(cur && (cur->timeToWait<=t1)){

    	t1=t1-cur->timeToWait;
    	pret=cur;
    	cur=cur->next;
    }
    Element* novi=new Element(t,t1,semm);
    novi->next=cur;
    if (pret){
    	pret->next=novi;
    }
    else{
    	first=novi;
    }
    if (cur) {cur->timeToWait-=t1;

    }
    else last=novi;


}

PCB* List::find(ID id){
	//HARD_LOCK;
	Element* cur=first;
	while(cur && cur->pcb->getID() != id){
		cur=cur->next;
	}
	if (cur){
		//HARD_UNLOCK;
		return cur->pcb;
	}
	//HARD_UNLOCK;
	return 0;
}
void List::remove(ID id){

	Element* cur=first;
	Element* prev=0;
	int i=0;
	while(cur && cur->pcb->getID() != id){

		prev=cur;
		cur=cur->next;
	}

	if (cur){

        if (prev){
        	prev->next=cur->next;
        	if (cur->next==0){
        		last=prev;
        	}
        	//cur->sem->remove(id);
        	delete cur;//Ovo je bilo zakomentarisano
        }
        else{
        	first=cur->next;
        	//cur->sem->remove(id);
        	delete cur;//Ovo je bilo zakomentarisano
        }
    }

}

