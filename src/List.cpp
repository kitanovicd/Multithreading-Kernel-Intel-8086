#include "List.h"
#include "Thread.h"
#include "Def.h"


List::List() {
	first=last=0;
}

List::~List() {

	while(first){
		El* stari=first;
		first=first->next;
		delete stari;
	}
}
void List::add(PCB* t){

	if (last){
		last=last->next=new El(t);
	}
	else first=last=new El(t);


}

PCB* List::find(ID id){
	El* cur=first;
	while(cur && cur->pcb->getID() != id){
		cur=cur->next;
	}
	if (cur){
		return cur->pcb;
	}
	return 0;
}
void List::remove(ID id){

	El* cur=first;
	El* prev=0;
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
        	delete cur;
        }
        else{
        	first=cur->next;
        	if (first==0) last=0;
        	delete cur;
        }
    }
}
