//
// Created by Decimation on 4/27/2018.
//

#include "Main.h"

int main(void)
{
	char                      buf[20];
	real_t                    r;
	const real_t              real1   = os_Int24ToReal(1);
	const real_t              real2   = os_Int24ToReal(2);
	const static superpoint_t iopoint = {{12, 116}, "..."};
	const static superpoint_t title   = {{12, 16}, "TITrig"};
	const static superpoint_t author  = {{12, 36}, "By Decimation"};


	dbg_sprintf(dbgout, "\n[----------------------------------------]\n");
	dbg_sprintf(dbgout, "[TITrig] %d.%d\n", kMajor, kMinor);

	os_ClrHome();
	gfx_Begin();


	gfx_PrintColor(&title, gfx_blue);
	gfx_PrintColor(&author, gfx_green);

	sprintf(buf, "Build %d.%d", kMajor, kMinor);
	gfx_PrintStringXY(buf, 12, 56);

	gfx_PrintStringXY("1. Right", 12, 76);
	gfx_PrintStringXY("2. Non-right", 12, 96);

	gfx_PrintStringXY("- Press Clear to quit", 12, 136);

	gfx_Print(&iopoint);
	r = io_gfx_ReadReal(&iopoint);


	if (os_RealCompare(&real1, &r) == 0)
	{
		right_SolveTriangle();
	}
	else if (os_RealCompare(&real2, &r) == 0)
	{
		trig_SolveTriangle();
	}
	else
	{
		gfx_End();
		return 0;
	}

	gfx_End();
	return 0;
}