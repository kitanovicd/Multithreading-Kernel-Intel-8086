#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "Event.h"
typedef void interrupt (*routine)(...);

class KEvent;

class IVTEntry {
public:
	static KEvent* events[256];
	static routine routines[256];
	IVTEntry(routine function,IVTNo number);
	void signal(IVTNo number);
	void getold(routine rout,IVTNo number);
	//virtual ~IVTEntry();
};

#define PREPAREENTRY(_ivtNo, _callOld)\
void interrupt intr##_ivtNo(...);\
IVTEntry newEntry##_ivtNo(intr##_ivtNo,_ivtNo);\
void interrupt intr##_ivtNo(...) {\
	routine old;\
	newEntry##_ivtNo.getold(old, _ivtNo);\
	newEntry##_ivtNo.signal(_ivtNo);\
	if (_callOld) {\
		old();\
	}\
}

#endif
