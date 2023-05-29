#include <iostream.h>
#include <dos.h>
#include "SCHEDULER.H"
#include "Timer.h"
#include "PCB.h"
#include "Thread.h"
#include "Def.h"
#include <stdio.h>
#include <dos.h>
#include "Semaphor.h"
#include <iostream.h>
#include <stdlib.h>
#include <DOS.H>
#include <STDIO.H>
// #include <STDARG.H>
const double pi = 3.1415926535;

int userMain(int argv, char *argc[]);
static void interrupt (*oldTimerInterrupt)(...);
void system_restore()
{
	HARD_LOCK
	delete PCB::running;
#ifndef BCC_BLOCK_IGNORE
	setvect(0x8, oldTimerInterrupt);
#endif
	HARD_UNLOCK
}
double cosinus_c(double x);
double cosinus_asm(double x);
double cosinus_asm(double x)
{
	if (x < 0)
	{
		return cosinus_c(-x);
	}
	int mnozilac = x / (2 * pi);
	double vrednost = x - mnozilac * 2 * pi;
	if ((vrednost > pi / 2) && (vrednost < pi))
		return -cosinus_c(pi - vrednost);
	if ((vrednost > pi) && (vrednost < 3 * pi / 2))
		return -cosinus_c(vrednost - pi);
	if ((vrednost > 3 * pi / 2) && (vrednost < 2 * pi))
		return cosinus_c(2 * pi - vrednost);
	double retu = 1;
	double mult = -1;
	double jump = vrednost * vrednost;
	double stepen = jump;
	int divv = 2;
	float fakt = 2;
	int i = 1;

loop:
	asm {

	        fld stepen
			fld mult
			fmulp // stepen*mul
	        fld fakt
			fdivp // stepen*mul/fakt
			fld retu
			faddp
			fstp retu // ret=ret + stepen*mul/fakt

										// priprema za sledeci krug
			fld mult
			fchs
			fstp mult // mult=-mult

			fld stepen
			fld jump
			fmulp
			fstp stepen // stepen=stepen*jump


            inc divv
			fild divv
			fld fakt
			fmulp
			inc divv
			fild divv
			fmulp
			fstp fakt // fakt=fakt*(div+1)*(div+2) div=div+2

			inc i
			cmp i,7
			jl loop

	}
	return retu;
}
double cosinus_c(double x)
{

	if (x < 0)
	{
		return cosinus_c(-x);
	}
	int mnozilac = x / (2 * pi);
	double vrednost = x - (mnozilac * 2 * pi);

	if ((vrednost > pi / 2) && (vrednost <= pi))
		return -cosinus_c(pi - vrednost);
	if ((vrednost > pi) && (vrednost <= 3 * pi / 2))
		return -cosinus_c(vrednost - pi);
	if ((vrednost > 3 * pi / 2) && (vrednost <= 2 * pi))
		return cosinus_c(2 * pi - vrednost);
	double ret = 1;
	double mul = -1;
	double jump = vrednost * vrednost;
	double stepen = jump;
	int div = 2;
	int fakt = 2;
	int i = 1;
	while (i < 7)
	{
		ret += mul * stepen / fakt;
		mul = mul * (-1);
		stepen = stepen * jump;
		div += 2;
		fakt = fakt * div * (div - 1);
		i++;
	}
	return ret;
}

int main(int argv, char *argc[])
{

	/*inic();
	int value=userMain(argc,argv);
	restore();
	return 0;*/
	double x = 3.25 * pi;
	printf("%.9lf\n", cosinus_c(x));
	printf("%.9lf\n", cosinus_asm(x));
	return 0;
}
