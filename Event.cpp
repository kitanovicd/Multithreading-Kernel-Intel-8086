#include "Event.h"
#include "Def.h"
#include "PCB.h"
#include "KEvent.h"

Event::Event(IVTNo ivtNo) {
    myImpl=new KEvent(PCB::running,ivtNo);
}

Event::~Event() {
	delete myImpl;
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}
