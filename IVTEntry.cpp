#include "IVTEntry.h"
#include "KEvent.h"
IVTEntry::IVTEntry(routine function,IVTNo number) {
	routines[number]=function;
}

void IVTEntry::signal(IVTNo number){
	(events[number])->signal();
}

void IVTEntry::getold(routine rout,IVTNo number){
	rout=events[number]->oldroutine;
}
