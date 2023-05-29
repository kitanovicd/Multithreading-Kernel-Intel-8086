#include "PCB.h"
#include "Timer.h"
#include "Def.h"
#include <dos.h>
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "List.h"
#include "SCHEDULER.H"

int PCB::cnt = 0;
unsigned PCB::globalBlock = 0;

ID PCB::id = 0;
PCB *PCB::running;
PCB *PCB::idle;
List PCB::list;

SignalHandler PCB::destroy()
{
	LOCK;
	while (myQueue->size() > 0)
	{
		PCB *temp = myQueue->get();
		temp->status = RUNNABLE;
		Scheduler::put(temp);
	}

	status = STOP;

	if (parent)
		parent->signal(1);

	UNLOCK;
	dispatch();
}

PCB::PCB(Thread *T) : parent(0), pcbStackSize(0), pcbTimeSlice(0), myQueue(0), mySP(0), finished(0), bp(0), ss(0), sp(0), bit(0)
{
	HARD_LOCK;
	myThread = T;
	threadID = id;
	period = 0.0;
	status = RUNNABLE;

	for (int i = 0; i < 16; i++)
	{
		signalLists[i] = 0;
		priorities[i] = -1;
	}

	block = 0x0000;
	parent = 0;
	list.add(this);
	HARD_UNLOCK;
}

PCB::PCB(Thread *T, StackSize stackSize, Time timeSlice)
{
	HARD_LOCK;

	bp = 0;
	status = NEW;
	period = 0.0;
	finished = 0;
	threadID = ++id;
	pcbTimeSlice = timeSlice;
	pcbStackSize = stackSize / 2;

	myThread = T;
	parent = PCB::running;

	myQueue = new Queue();
	mySP = new unsigned[pcbStackSize];

	for (int i = 0; i < 16; i++)
	{
		if (PCB::running->signalLists[i] == 0)
			signalLists[i] = 0;
		else
		{
			signalLists[i] = new SignalList();
			signalLists[i]->copy(PCB::running->signalLists[i]);
		}

		priorities[i] = PCB::running->priorities[i];
	}

	block = PCB::running->block;

#ifndef BCC_BLOCK_IGNORE

	mySP[pcbStackSize - 1] = CPU_FLAGS_INIT_VALUE;
	mySP[pcbStackSize - 2] = FP_SEG(&(PCB::wrapper));
	mySP[pcbStackSize - 3] = FP_OFF(&(PCB::wrapper));
	ss = FP_SEG(mySP + pcbStackSize - 12);
	sp = FP_OFF(mySP + pcbStackSize - 12);
#endif
	list.add(this);
	HARD_UNLOCK;
}

PCB::~PCB()
{

	PCB::list.remove(getID());

	if (mySP != 0)
		delete mySP;
}

ID PCB::getID()
{
	return threadID;
}

void PCB::wrapper()
{
	running->run();
}

void PCB::run()
{
	myThread->run();
	HARD_LOCK;

	while (myQueue->size() > 0)
	{
		PCB *temp = myQueue->get();
		temp->status = RUNNABLE;
		Scheduler::put(temp);
	}

	status = STOP;

	if (parent)
		parent->signal(1);

	if (((1U << 2) & block) == 0)
	{
		if (((1U << 2) & globalBlock) == 0)
		{
			if (signalLists[2] != 0)
				signalLists[2]->execute();
		}
	}

	HARD_UNLOCK;
	dispatch();
}

void PCB::signal(SignalId signal)
{
	LOCK;
	if ((signal >= 0) && (signal < 16))
	{
		int i = 0;
		while (i < 16 && priorities[i] != -1)
		{
			i++;
		}
		if (i < 16)
			priorities[i] = signal;
	}
	UNLOCK;
}

void PCB::registerHandler(SignalId signal, SignalHandler handler)
{
	LOCK;
	if (signal >= 0 && signal < 16)
	{
		if (signalLists[signal] == 0)
			signalLists[signal] = new SignalList();
		signalLists[signal]->add(handler);
	}
	UNLOCK;
	;
}

void PCB::unregisterAllHandlers(SignalId id)
{
	LOCK;
	if (id >= 0 && id < 16)
	{
		delete signalLists[id];
		signalLists[id] = 0;
	}
	UNLOCK;
}

void PCB::swap(SignalId id, SignalHandler handler1, SignalHandler handler2)
{
	LOCK;
	if (id >= 0 && id < 16)
		if (signalLists[id] != 0)
			signalLists[id]->swap(handler1, handler2);
	UNLOCK;
}

void PCB::blockSignal(SignalId signal)
{
	LOCK;
	if (signal >= 0 && signal < 16)
	{
		block |= (1 << signal);
	}
	UNLOCK;
}

void PCB::blockSignalGlobally(SignalId signal)
{
	LOCK;
	if (signal >= 0 && signal < 16)
	{
		globalBlock = globalBlock | (1 << signal);
	}
	UNLOCK;
}

void PCB::unblockSignal(SignalId signal)
{
	LOCK;
	if (signal >= 0 && signal < 16)
	{
		unsigned mask = 65534 << signal;
		block = block & mask;
	}
	UNLOCK;
}

void PCB::unblockSignalGlobally(SignalId signal)
{
	LOCK;
	if (signal >= 0 && signal < 16)
	{
		unsigned mask = 65535;
		mask = mask & (65534 << signal);
		globalBlock = globalBlock & mask;
	}
	UNLOCK;
}

void PCB::execute()
{
	LOCK;
	int i = 0;

	while (i < 16)
	{
		if (priorities[i] != -1 && (((1U << priorities[i]) & block) == 0) && (((1U << priorities[i]) & globalBlock) == 0))
		{

			if (priorities[i] == 0)
				destroy();
			else
				signalLists[priorities[i]]->execute();

			priorities[i] = -1;
		}

		i++;
	}

	i = 0;
	int j = 0;

	while (i < 16)
	{
		if (priorities[i] != -1)
		{
			if (!(i == j))
			{
				priorities[j] = priorities[i];
			}

			int temp = priorities[j];
			priorities[i] = -1;

			if (i == j)
				priorities[i] = temp;

			j++;
		}

		i++;
	}
	UNLOCK;
}
