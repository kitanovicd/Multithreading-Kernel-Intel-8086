#include "Queue.h"
#include "Def.h"
#include <stdio.h>
#include <stdlib.h>
#include "SCHEDULER.H"

Queue::Queue()
{

	this->head = 0;
	this->tail = 0;
	len = 0;
}

Queue::~Queue()
{
	while (!(this->head == 0))
	{
		QueueElement *old = this->head;
		this->head = this->head->next;

		delete old;
		len--;
	}
}

void Queue::put(PCB *pcb)
{
	if (head == 0)
		head = tail = new QueueElement(pcb);
	else
		tail = tail->next = new QueueElement(pcb);

	len++;
}

int Queue::size() const
{
	return len;
}

PCB *Queue::get()
{

	if (this->head == 0)
		return 0;

	QueueElement *temp = this->head;
	this->head = this->head->next;
	len--;

	PCB *pcb = temp->pcb;
	delete temp;
	return pcb;
}

PCB *Queue::get(ID id)
{
	QueueElement *current = head;
	QueueElement *preveous = 0;
	while (current != 0 && current->pcb->getID() != id)
	{
		preveous = current;
		current = preveous->next;
	}

	if (current == 0)
		return 0;

	if (preveous)
		preveous->next = current->next;
	else
		head = current->next;

	if (current->next == 0)
		tail = preveous;

	PCB *pcb = current->pcb;

	delete current;
	return pcb;
}
