#ifndef _OS1_BOUNDED_BUFFER_
#define _OS1_BOUNDED_BUFFER_

#include "semaphor.h"

class BoundedBuffer
{
private:
	unsigned Size;
	char *buffer;
	int head, tail;

	Semaphore mutexa, mutext;
	Semaphore spaceAvailable, itemAvailable;

public:
	BoundedBuffer(unsigned size);
	virtual ~BoundedBuffer();
	int append(char);
	char take();
	int fullCount() { return itemAvailable.val(); };
};

#endif
