/*
 * SemaphoreList.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#include "SemaphoreList.h"
#include "Def.h"
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

SemaphoreList::SemaphoreList()
{
	head = tail = 0;
}

SemaphoreList::~SemaphoreList()
{

	while (head)
	{
		SemaphoreListElement *old = head;
		head = head->next;
		delete old;
	}
}
void SemaphoreList::add(PCB *t, Time time, KernelSem *kernelSemaphore)
{

	SemaphoreListElement *current = head;
	SemaphoreListElement *preveous = 0;

	while (current && (current->timeToWait <= time))
	{

		time = time - current->timeToWait;
		preveous = current;
		current = current->next;
	}

	SemaphoreListElement *newElement = new SemaphoreListElement(t, time, kernelSemaphore);
	newElement->next = current;

	if (preveous)
		preveous->next = newElement;
	else
		head = newElement;

	if (current)
		current->timeToWait -= time;
	else
		tail = newElement;
}

PCB *SemaphoreList::find(ID id)
{
	SemaphoreListElement *current = head;

	while (current && current->pcb->getID() != id)
	{
		current = current->next;
	}

	if (current)
		return current->pcb;

	return 0;
}

void SemaphoreList::remove(ID id)
{
	SemaphoreListElement *current = head;
	SemaphoreListElement *preveous = 0;

	int i = 0;

	while (current && current->pcb->getID() != id)
	{
		preveous = current;
		current = current->next;
	}

	if (current)
	{

		if (preveous)
		{
			preveous->next = current->next;

			if (current->next == 0)
				tail = preveous;

			delete current;
		}
		else
		{
			head = current->next;
			delete current;
		}
	}
}
