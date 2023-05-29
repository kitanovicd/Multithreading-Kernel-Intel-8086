/*
 * Timer.h
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */
#include "List.h"

#ifndef TIMER_H_
#define TIMER_H_

void interrupt timer(...);
void dispatch();
void inic();
void restore();
void tick();
int syncPrintf(const char *format, ...);

#endif /* TIMER_H_ */
