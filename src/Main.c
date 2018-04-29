//
// Created by Decimation on 4/27/2018.
//

#include <debug.h>
#include <graphx.h>
#include "RightTriangle.h"
#include "GraphicsExt.h"
#include "Library.h"

#define MAJOR 2
#define MINOR 0

int main(void)
{

	dbg_sprintf(dbgout, "\n[----------------------------------------]\n");
	dbg_sprintf(dbgout, "[TITrig] %d.%d\n", MAJOR, MINOR);

	//gfx_Begin();
	//gfx_RadicalFraction(point, 2, 5, 3, 15);
	right_SolveTriangle();

	return 0;
}