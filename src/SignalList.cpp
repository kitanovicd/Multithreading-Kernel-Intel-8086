/*
 * SignalList.cpp
 *
 *  Created on: Aug 12, 2019
 *      Author: OS1
 */

#include "SignalList.h"
#include <stdio.h>
#include <stdlib.h>

SignalList::SignalList()
{
	head = tail = 0;
}

SignalList::~SignalList()
{
	while (head)
	{
		SignalListElement *old = head;
		head = head->next;
		delete old;
	}
}

void SignalList::add(SignalHandler t)
{
	if (tail)
	{
		tail = tail->next = new SignalListElement(t);
	}
	else
	{
		head = new SignalListElement(t);
		tail = head;
	}
}

void SignalList::start()
{
	SignalListElement *current = head;

	while (current != 0)
	{
		current->handler();
		current = current->next;
	}
}

void SignalList::swap(SignalHandler handler1, SignalHandler handler2)
{
	SignalListElement *first = head;
	SignalListElement *second = head;

	while (first && second && (first->handler != handler1 || second->handler != handler2))
	{
		if (first->handler != handler1)
		{
			first = first->next;
		}
		if (second->handler != handler2)
		{
			second = second->next;
		}
	}
	if (first && second)
	{
		SignalHandler t = first->handler;
		first->handler = second->handler;
		second->handler = t;
	}
}

void SignalList::copy(SignalList *list)
{
	SignalListElement *current = list->head;

	while (current)
	{
		add(current->handler);
		current = current->next;
	}
}

void SignalList::execute()
{

	SignalListElement *current = head;

	while (current)
	{
		current->handler();
		current = current->next;
	}
}
