#include "KerSem.h"
#include "Semaphor.h"
#include "Queue.h"
#include "SCHEDULE.H"
#include "Timer.h"
#include "Semlist.h"
#include "PCB.h"
#include "Def.h"
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

List KernelSem::list;

KernelSem::KernelSem(int init)
{
    value = init;
    waitQueue = new Queue();
}

KernelSem::~KernelSem()
{
    HARD_LOCK;
    delete waitQueue;
    HARD_UNLOCK;
}

int KernelSem::val() const
{
    return value;
}

int KernelSem::signal(int n)
{
    HARD_LOCK;
    int i = 0;

    if (n < 0)
    {
        HARD_UNLOCK;
        return n;
    }
    else
    {
        if (n == 0)
        {
            if (value + 1 <= 0)
            {
                PCB *temp = waitQueue->get();
                list.remove(temp->getID());

                if (temp)
                {
                    temp->bit = 1;
                    temp->status = RUNNABLE;
                }

                Scheduler::put(temp);

                i++;
                n--;
                value++;
            }
            else
            {
                ++value;
            }
        }
        else
        {
            value += n;

            while (n > 0 || waitQueue->size() != 0)
            {
                PCB *t = waitQueue->get();
                list.remove(t->getID());

                t->status = RUNNABLE;
                i++;
                n--;

                Scheduler::put(t);
            }
        }
    }

    HARD_UNLOCK;
    return i;
}

int KernelSem::wait(Time maxTimeToWait)
{
    HARD_LOCK;
    value--;

    if (value < 0)
    {
        PCB::running->status = UNRUNNABLE;
        waitQueue->put(PCB::running);

        if (maxTimeToWait > 0)
            list.add(PCB::running, maxTimeToWait, this);

        HARD_UNLOCK;
        dispatch();
        HARD_LOCK;

        if (maxTimeToWait == 0)
        {
            HARD_UNLOCK;
            return 1;
        }
        else
        {
            if (PCB::running->bit == 1)
            {
                PCB::running->bit = 0;
                HARD_UNLOCK;
                return 1;
            }
            else
            {
                HARD_UNLOCK;
                return 0;
            }
        }
    }

    HARD_UNLOCK;
    return 1;
}

void KernelSem::put(PCB *pcbb)
{
    PCB *pcb = waitQueue->get(pcbb->getID());
    ++value;
    pcb->status = RUNNABLE;
    Scheduler::put(pcb);
}

void KernelSem::search()
{
    HARD_LOCK;
    if ((list.head))
    {
        ListElement *current;
        --(list.head->timeToWait);

        while (list.head && list.head->pcb && list.head->timeToWait == 0)
        {
            current = list.head;
            list.head = list.head->next;
            current->sem->put(current->pcb);
            delete current;
        }
    }
    HARD_UNLOCK;
}
