//
// Created by Decimation on 4/27/2018.
//

#include <debug.h>
#include "RightTriangle.h"

#define MAJOR 2
#define MINOR 0

int main(void)
{

	dbg_sprintf(dbgout, "\n[----------------------------------------]\n");
	dbg_sprintf(dbgout, "[TITrig] %d.%d\n", MAJOR, MINOR);


	right_SolveTriangle();

	return 0;
}