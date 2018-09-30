//
// Created by Decimation on 4/27/2018.
//

#include "Main.h"
#include "Library.h"
#include "/LabelPoint/LabelPoint.h"

int main(void)
{
	char   buf[20];
	real_t r;
	LocalReal(1);
	LocalReal(2);


	const static labelpoint_t iopoint = {{12, 116}, "..."};
	const static labelpoint_t title   = {{12, 16}, "TITrig"};
	const static labelpoint_t author  = {{12, 36}, "By Decimation"};

	dbg_sprintf(dbgout, "\n[----------------------------------------]\n");
	dbg_sprintf(dbgout, "[TITrig] %d.%d\n", kMajor, kMinor);

	os_ClrHome();
	gfx_Begin();


	lp_PrintColor(&title, gfx_blue);
	lp_PrintColor(&author, gfx_green);

	sprintf(buf, "Build %d.%d", kMajor, kMinor);
	gfx_PrintStringXY(buf, 12, 56);

	gfx_PrintStringXY("1. Right", 12, 76);
	gfx_PrintStringXY("2. Non-right", 12, 96);

	gfx_PrintStringXY("- Press Clear to quit", 12, 136);

	lp_Print(&iopoint);
	r = io_gfx_ReadReal(&iopoint);


	if (os_RealCompare(&real1, &r) == 0) {
		right_SolveTriangle();
	}
	else if (os_RealCompare(&real2, &r) == 0) {
		trig_SolveTriangle();
	}

	gfx_End();
	return 0;
}