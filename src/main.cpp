#include "Thread.h"

int userMain(int argv, char *argc[]);

static void interrupt (*oldTimerInterrupt)(...);

int main(int argv, char *argc[])
{
    init();

    int value = userMain(argv, argc);

    restore();

    return 0;
}
