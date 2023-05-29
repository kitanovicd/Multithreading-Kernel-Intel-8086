
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include "SCHEDULER.H"
#include "Semaphor.h"
#include "Timer.h"
#include "PCB.h"
#include "Thread.h"
#include "Def.h"

const double PI = 3.1415926535;

int userMain(int argv, char *argc[]);

static void interrupt (*oldTimerInterrupt)(...);

int main(int argv, char *argc[])
{
    init();

    int value = userMain(argv, argc);

    restore();

    return 0;
}
