/*
 * SignList.cpp
 *
 *  Created on: Aug 12, 2019
 *      Author: OS1
 */

#include "SignList.h"
#include <stdio.h>
#include <stdlib.h>
signlist::signlist()
{
	first = last = 0;
}

signlist::~signlist()
{
	while (first)
	{
		Elementt *stari = first;
		first = first->next;
		delete stari;
	}
}

void signlist::add(SignalHandler t)
{
	if (last)
	{
		last = last->next = new Elementt(t);
	}
	else
	{
		first = new Elementt(t);
		last = first;
	}
}

void signlist::start()
{
	Elementt *tek = first;
	while (tek != 0)
	{
		tek->handler();
		tek = tek->next;
	}
}

void signlist::swap(SignalHandler hand1, SignalHandler hand2)
{
	Elementt *tek1 = first;
	Elementt *tek2 = first;
	while (tek1 && tek2 && (tek1->handler != hand1 || tek2->handler != hand2))
	{
		if (tek1->handler != hand1)
		{
			tek1 = tek1->next;
		}
		if (tek2->handler != hand2)
		{
			tek2 = tek2->next;
		}
	}
	if (tek1 && tek2)
	{
		SignalHandler t = tek1->handler;
		tek1->handler = tek2->handler;
		tek2->handler = t;
	}
}
void signlist::copy(signlist *list)
{
	Elementt *tek = list->first;
	while (tek)
	{
		add(tek->handler);
		tek = tek->next;
	}
}
void signlist::izvrsi()
{

	Elementt *tek = first;
	while (tek)
	{
		tek->handler();
		tek = tek->next;
	}
}
