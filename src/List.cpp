#include "List.h"
#include "Thread.h"
#include "Def.h"

List::List()
{
	head = tail = 0;
}

List::~List()
{

	while (head)
	{
		ListElement *old = head;
		head = head->next;
		delete old;
	}
}
void List::add(PCB *t)
{

	if (tail)
	{
		tail = tail->next = new ListElement(t);
	}
	else
		head = tail = new ListElement(t);
}

PCB *List::find(ID id)
{
	ListElement *current = head;

	while (current && current->pcb->getID() != id)
	{
		current = current->next;
	}

	if (current)
		return current->pcb;

	return 0;
}

void List::remove(ID id)
{

	int i = 0;
	ListElement *preveous = 0;
	ListElement *current = head;

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

			if (head == 0)
				tail = 0;

			delete current;
		}
	}
}
