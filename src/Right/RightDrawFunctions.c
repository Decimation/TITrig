//
// Created by Decimation on 9/29/2018.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-misplaced-array-index"

#include <graphx.h>
#include <debug.h>
#include "RightDrawFunctions.h"
#include "../Types.h"
#include "RightTriangle.h"
#include "../System.h"

void ui_DrawFunctions_A()
{
	DrawFunction drawFn = ui_ResolveDrawFunction();
	ui_ClearFunctionPoints();

	// When angle AAA is selected
	lp_SetLabel(&g_rXSides_OAH[0], lbl_ADJ); // bbb = adj
	lp_SetLabel(&g_rXSides_OAH[1], lbl_OPP); // aaa = opp
	lp_SetLabel(&g_rXSides_OAH[2], lbl_HYP); // ccc = hyp


	lp_PrintColor(&ui_Wait, gfx_red);

	dbg_sprintf(dbgout, "Drawing functions\n");

	// SOH
	drawFn(fnSinPt, SIDE_a, SIDE_c);

	// CAH
	drawFn(fnCosPt, SIDE_b, SIDE_c);

	// TOA
	drawFn(fnTanPt, SIDE_a, SIDE_b);

	// cot = adj/opp
	drawFn(fnCotPt, SIDE_b, SIDE_a);

	// sec = hyp/adj
	drawFn(fnSecPt, SIDE_c, SIDE_b);

	// csc = hyp/opp
	drawFn(fnCscPt, SIDE_c, SIDE_a);


	lp_Clear(&ui_Wait);
	dbg_sprintf(dbgout, "Completed drawing of A ratios\n");

	ui_RedrawFunctionPoints();
	dbg_sprintf(dbgout, "Completed printing of labels for A\n");
}

static void ui_RedrawFunctionPoints()
{
	int index = 0;
	for (; index < CountOf(g_rXSides_OAH); index++) {
		lp_PrintColor(&g_rXSides_OAH[index], gfx_green);
		lp_PrintColor(&r_func[index].left, gfx_blue);
		lp_PrintColor(&r_func2[index].left, gfx_blue);
	}
}

static void ui_ClearFunctionPoints()
{
	int index = 0;
	for (; index < CountOf(r_func); index++) {
		gfx_ClearFraction(r_func[index].data);
		gfx_ClearFraction(r_func2[index].data);
	}
}

void ui_DrawFunctions_B()
{

	DrawFunction drawFn = ui_ResolveDrawFunction();
	ui_ClearFunctionPoints();


	// When angle BBB is selected
	lp_SetLabel(&g_rXSides_OAH[0], lbl_OPP); // bbb = opp
	lp_SetLabel(&g_rXSides_OAH[1], lbl_ADJ); // aaa = adj
	lp_SetLabel(&g_rXSides_OAH[2], lbl_HYP); // ccc = hyp

	lp_PrintColor(&ui_Wait, gfx_red);

	// SOH
	drawFn(fnSinPt, SIDE_b, SIDE_c);

	// CAH
	drawFn(fnCosPt, SIDE_a, SIDE_c);

	// TOA
	drawFn(fnTanPt, SIDE_b, SIDE_a);

	// cot = adj/opp
	drawFn(fnCotPt, SIDE_a, SIDE_b);

	// sec = hyp/adj
	drawFn(fnSecPt, SIDE_c, SIDE_a);

	// csc = hyp/opp
	drawFn(fnCscPt, SIDE_c, SIDE_b);

	lp_Clear(&ui_Wait);
	dbg_sprintf(dbgout, "Completed drawing of B ratios\n");

	ui_RedrawFunctionPoints();
	dbg_sprintf(dbgout, "Completed printing of labels for B\n");
}

static DrawFunction ui_ResolveDrawFunction()
{
	if (!ui_dispSimpRoot) {
		dbg_sprintf(dbgout, "&right_DrawFunction\n");
		return &right_DrawFraction;
	}
	dbg_sprintf(dbgout, "&right_DispFunctionRatioRoot\n");
	return &right_DispFunctionRatioRoot;

//	return (!ui_dispSimpRoot) ? &right_DrawFraction : &right_DispFunctionRatioRoot;
}

void ui_DrawFunctions_90()
{
	int index = 0;
	// Draw nothing when 90 is selected
	lp_SetLabel(&g_rXSides_OAH[0], "B"); // bbb
	lp_SetLabel(&g_rXSides_OAH[1], "A"); // aaa
	lp_SetLabel(&g_rXSides_OAH[2], "C"); // ccc

	for (; index < CountOf(g_rXSides_OAH); index++) {
		lp_Clear(&g_rXSides_OAH[index]);
		dp_Clear(&r_func[index]);
		dp_Clear(&r_func2[index]);
	}
}
#pragma clang diagnostic pop