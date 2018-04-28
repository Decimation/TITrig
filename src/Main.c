//
// Created by Decimation on 4/27/2018.
//

#include <debug.h>
#include "Triangle.h"

#define MAJOR 1
#define MINOR 0

int main(void)
{
	dbg_sprintf(dbgout, "\n[----------------------------------------]\n");
	dbg_sprintf(dbgout, "[Trig] %d.%d\n", MAJOR, MINOR);
	trig_SolveTriangle();
	return 0;
}