#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

typedef unsigned int Time;

class KernelSem;

class Semaphore
{
private:
	KernelSem *myImpl;

public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	int val() const;

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n = 0);
};

#endif
