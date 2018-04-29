//
// Created by Decimation on 4/27/2018.
//

#include "Main.h"
#include "Triangle.h"


int main(void)
{
	real_t r;
	const real_t real1 = os_Int24ToReal(1);
	const static superpoint_t point = {{12,56}, "..."};

	dbg_sprintf(dbgout, "\n[----------------------------------------]\n");
	dbg_sprintf(dbgout, "[TITrig] %d.%d\n", MAJOR, MINOR);

	os_ClrHome();
	gfx_Begin();
	gfx_PrintStringXY("1. Right",12,16);
	gfx_PrintStringXY("2. Non-right",12,36);
	gfx_Print(&point);
	r = io_gfx_ReadReal(&point);

	if (os_RealCompare(&real1, &r) == 0) {
		right_SolveTriangle();
	}
	else {
		trig_SolveTriangle();
	}

	gfx_End();


	return 0;
}