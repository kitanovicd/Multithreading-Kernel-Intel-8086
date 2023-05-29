
#include "Timer.h"
#include "Def.h"
#include "PCB.h"
#include "SCHEDULER.H"
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <iostream.h>
#include "KerSem.h"
#include "Semlist.h"
#include "Semaphor.h"
#include <STDARG.H>
#include "Event.h"
#include "IVTEntry.h"
#include "Thread.h"

static volatile unsigned tsp;
static volatile unsigned tss;
static volatile unsigned tbp;
static volatile int timeCounter = defaultTimeSlice;

volatile char explicitTimerCall;
volatile char contextSwitchOnDemand;
volatile char runIndefinitelly;

int t = -1;
unsigned oldTimerOFF, oldTimerSEG;

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	HARD_LOCK
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	HARD_UNLOCK
	return res;
}

void tick()
{
	t++;

	if (t)
		syncPrintf("%d\n", t);
}

class Idle : public Thread
{
public:
	Idle(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) : Thread(stackSize, 2) {}
	~Idle()
	{
		waitToComplete();
	}
	void run()
	{
		while (1)
		{
		}
	}
};

void interrupt timer(...)
{
	if (!explicitTimerCall)
	{
		asm int 60h;
		tick();
		--timeCounter;

		KernelSem::search();
	}

	explicitTimerCall = 0;

	if (((timeCounter == 0) && (PCB::running->pcbTimeSlice != 0)) || (contextSwitchOnDemand && PCB::cnt == 0))
	{
		contextSwitchOnDemand = 0;
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if ((PCB::running != PCB::idle) && (PCB::running->status == RUNNABLE))
		{
			PCB::running->bit = 0;
			Scheduler::put(PCB::running);
		}

		PCB::running = Scheduler::get();
		if (PCB::running)
			PCB::running->bit = 0;

		if (PCB::running == 0)
		{
			PCB::running = PCB::idle;
		}

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		timeCounter = PCB::running->pcbTimeSlice;
		// runIndefinitelly=!(timeCounter>0);

		asm {
        	mov sp,tsp
			mov ss,tss
			mov bp,tbp
		}

		if (PCB::running != PCB::idle)
		{
			PCB::running->execute();
		}
	}
}

void dispatch()
{
	HARD_LOCK;
	explicitTimerCall = 1;
	contextSwitchOnDemand = 1;
	timer();
	HARD_UNLOCK;
}

void inic()
{
	asm {
		cli
		push es
		push ax

		mov ax,0 //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer // postavlja
		mov word ptr es:0020h, offset timer // novu rutinu

		mov ax, oldTimerSEG //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}

	Thread *T = new Thread();
	PCB::running = new PCB(T);
	PCB::list.remove(T->myPCB->getID());
	T->myPCB = PCB::running;

	Idle *i = new Idle();
	PCB::idle = i->myPCB;
}

void restore()
{

	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}
