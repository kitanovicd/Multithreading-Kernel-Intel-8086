/*
 * Def.h
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */

#ifndef DEF_H_
#define DEF_H_

#define NEW 0
#define RUNNABLE 1
#define RUN 2
#define UNRUNNABLE 3
#define STOP 4
#define HARD_LOCK asm pushf;\
	         asm cli;

#define HARD_UNLOCK asm popf;
#define CPU_FLAGS_INIT_VALUE 0x0200


typedef int ID;
typedef unsigned long StackSize;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
const StackSize defaultStackSize = 4096;



#endif /* DEF_H_ */
