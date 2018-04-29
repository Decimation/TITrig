//
// Created by Decimation on 4/28/2018.
//

#include <debug.h>
#include "RightTriangle.h"

//
// Created by Decimation on 4/14/2018.
//

#include "GraphicsExt.h"
#include "Library.h"
#include "Geometry.h"
#include "Simplifiers.h"
#include <string.h>

static superpoint_t xangles[3] = {
		/* verts[2] + X, verts[3] + Y */
		{{30 + 5,   129 - 10}, "90"},   // 90
		{{30 + 120, 129 - 10}, "B"},  // BBB
		{{37,       27},       "A"}   // AAA
};

static superpoint_t xsides[3] = {
		/* verts[2] + X, verts[3] + Y */
		{{30 - 25, 129 - 50}, "b"},  // bbb
		{{30 + 50, 129 + 10}, "a"},  // aaa
		{{30 + 90, 129 - 70}, "c"}   // ccc
};

static superpoint_t xsides_oah[3] = {
		/* verts[2] + X, verts[3] + Y */
		{{30 - 25, 129 - 40}, "..."},  // bbb
		{{30 + 50, 129 + 20}, "..."},  // aaa
		{{30 + 90, 129 - 60}, "..."}   // ccc
};

#define side_a xsides[1]
#define side_b xsides[0]
#define side_c xsides[2]

#define angle_A xangles[2]
#define angle_B xangles[1]
#define angle_C xangles[0]

static const superpoint_t ui_btn_Mode           = {280, 230, "Mode"};
static const superpoint_t ui_btn_Clear          = {215, 230, "Clear"};
static const superpoint_t ui_btn_EnableSimpRoot = {150, 230, "Root"};

static bool              ui_dispSimpRoot;
static superpoint_t      ui_Wait                = {{230, 40}, "Processing..."};
static superpoint_t      ui_Mode                = {{230, 10}, "ANGLE MODE"};
static const gfx_point_t ui_RootPoint           = {230, 25};

static const char lbl_AngleMode[] = "ANGLE MODE";
static const char lbl_SideMode[]  = "SIDE MODE";
static const char lbl_ADJ[]       = "ADJ";
static const char lbl_HYP[]       = "HYP";
static const char lbl_OPP[]       = "OPP";

static triangle_t   triangle;
static trigstatus_t trigstatus;

static const superpoint_t funcData[3] = {
		{10,  175, "sin = "},
		{110, 175, "cos = "},
		{210, 175, "tan = "}
};

static const gfx_point_t funcFracPoints[3] = {
		{10 + 35,  175},
		{110 + 35, 175},
		{210 + 35, 175}
};

static const superpoint_t funcData2[3] = {
		{10,  210, "cot = "},
		{110, 210, "sec = "},
		{210, 210, "csc = "}
};

static const gfx_point_t func2FracPoints[3] = {
		{10 + 35,  210},
		{110 + 35, 210},
		{210 + 35, 210}
};


void right_Sync()
{
	dbg_sprintf(dbgout, "Synchronizing right triangle...\n");
	geo_RoundTriangle(&triangle, gRound);
	if (trigstatus.A)
	{
		os_RealToStr(angle_A.label, &triangle.A, 0, 0, 6);
	}
	if (trigstatus.B)
	{
		os_RealToStr(angle_B.label, &triangle.B, 0, 0, 6);
	}
	if (trigstatus.a)
	{
		os_RealToStr(side_a.label, &triangle.a, 0, 0, 6);
	}
	if (trigstatus.b)
	{
		os_RealToStr(side_b.label, &triangle.b, 0, 0, 6);
	}
	if (trigstatus.c)
	{
		os_RealToStr(side_c.label, &triangle.c, 0, 0, 6);
	}

	right_TruncateLabels(gDigitThreshold);
	right_Redraw();
}

void right_TruncateLabels(int len)
{
	int i = 0;
	for (; i < 3; i++)
	{
		lib_StrCut(xangles[i].label, len, 20 - len);
		lib_StrCut(xsides[i].label, len, 20 - len);
	}
}

// todo: check domain and imaginary numbers
void pythag_CheckSolvability()
{
	real_t       apow, bpow, cpow;
	const real_t real2 = os_Int24ToReal(2);

	if (trigstatus.a && trigstatus.b && !trigstatus.c)
	{
		//a^2 + b^2 = c^2
		apow = os_RealPow(&triangle.a, &real2);
		bpow = os_RealPow(&triangle.b, &real2);
		cpow = os_RealAdd(&apow, &bpow);
		triangle.c   = os_RealSqrt(&cpow);
		trigstatus.c = true;

		return;
	}
	else if (!trigstatus.a && trigstatus.b && trigstatus.c)
	{
		//a^2 + b^2 = c^2

		bpow = os_RealPow(&triangle.b, &real2);
		cpow = os_RealPow(&triangle.c, &real2);
		apow = os_RealSub(&cpow, &bpow);
		triangle.a   = os_RealSqrt(&apow);
		trigstatus.a = true;
		return;
	}
	else if (trigstatus.a && !trigstatus.b && trigstatus.c)
	{
		//a^2 + b^2 = c^2

		apow = os_RealPow(&triangle.a, &real2);
		cpow = os_RealPow(&triangle.c, &real2);
		bpow = os_RealSub(&cpow, &apow);
		triangle.b   = os_RealSqrt(&bpow);
		trigstatus.b = true;
		return;
	}
}

void CheckForInt(char* in, char* possibleSrc)
{
	if (in[strlen(in) - 2] == '1')
	{
		dbg_sprintf(dbgout, "[RightTrig] Int detected: %s\n", in);
		strcpy(in, possibleSrc);
	}
	else if (in[strlen(in) - 2] == '0')
	{
		strcpy(in, "0");
	}
}

// For right triangle only
void ui_ClearFraction(gfx_point_t point)
{
	static const uint8_t w = 60;
	static const uint8_t h = 35;
	point.x -= 1;
	point.y -= 16;
	ui_ClearArea(point, w, h);
}

void sohcahtoa_DrawA()
{
	int     index = 0;
	int24_t rad[2], rad2[2];


	for (index = 0; index < 3; index++)
	{
		ui_ClearFraction(funcFracPoints[index]);
		ui_ClearFraction(func2FracPoints[index]);
	}

	// When angle AAA is selected
	sp_SetLabel(&xsides_oah[0], lbl_ADJ); // bbb = adj
	sp_SetLabel(&xsides_oah[1], lbl_OPP); // aaa = opp
	sp_SetLabel(&xsides_oah[2], lbl_HYP); // ccc = hyp


	gfx_PrintColor(&ui_Wait, gfx_red);
	if (!ui_dispSimpRoot)
	{
		// SOH
		//sp_SetFuncDataLabel(triangle.a, triangle.c, 0);
		gfx_Fraction(funcFracPoints[0], triangle.a, triangle.c);

		// CAH
		//sp_SetFuncDataLabel(triangle.b, triangle.c, 1);
		gfx_Fraction(funcFracPoints[1], triangle.b, triangle.c);

		// TOA
		//sp_SetFuncDataLabel(triangle.a, triangle.b, 2);
		gfx_Fraction(funcFracPoints[2], triangle.a, triangle.b);

		// cot = adj/opp
		//sp_SetFuncData2Label(triangle.b, triangle.a, 0);
		gfx_Fraction(func2FracPoints[0], triangle.b, triangle.a);

		// sec = hyp/adj
		//sp_SetFuncData2Label(triangle.c, triangle.b, 1);
		gfx_Fraction(func2FracPoints[1], triangle.c, triangle.b);

		// csc = hyp/opp
		//sp_SetFuncData2Label(triangle.c, triangle.a, 2);
		gfx_Fraction(func2FracPoints[2], triangle.c, triangle.a);
	}
	else
	{
		// SOH
		SimplifyRadicalFromDecimal(triangle.a, rad);
		SimplifyRadicalFromDecimal(triangle.c, rad2);
		gfx_RadicalFraction(funcFracPoints[0], rad[0], rad[1], rad2[0], rad2[1]);


		// CAH
		SimplifyRadicalFromDecimal(triangle.b, rad);
		SimplifyRadicalFromDecimal(triangle.c, rad2);
		gfx_RadicalFraction(funcFracPoints[1], rad[0], rad[1], rad2[0], rad2[1]);

		// TOA
		SimplifyRadicalFromDecimal(triangle.a, rad);
		SimplifyRadicalFromDecimal(triangle.b, rad2);
		gfx_RadicalFraction(funcFracPoints[2], rad[0], rad[1], rad2[0], rad2[1]);


		// cot = adj/opp
		SimplifyRadicalFromDecimal(triangle.b, rad);
		SimplifyRadicalFromDecimal(triangle.a, rad2);
		gfx_RadicalFraction(func2FracPoints[0], rad[0], rad[1], rad2[0], rad2[1]);


		// sec = hyp/adj
		SimplifyRadicalFromDecimal(triangle.c, rad);
		SimplifyRadicalFromDecimal(triangle.b, rad2);
		gfx_RadicalFraction(func2FracPoints[1], rad[0], rad[1], rad2[0], rad2[1]);


		// csc = hyp/opp
		SimplifyRadicalFromDecimal(triangle.c, rad);
		SimplifyRadicalFromDecimal(triangle.a, rad2);
		gfx_RadicalFraction(func2FracPoints[2], rad[0], rad[1], rad2[0], rad2[1]);
	}
	gfx_Clear(&ui_Wait);

	for (index = 0; index < 3; index++)
	{
		gfx_PrintColor(&xsides_oah[index], gfx_green);
		gfx_PrintColor(&funcData[index], gfx_blue);
		gfx_PrintColor(&funcData2[index], gfx_blue);
	}
}

void sohcahtoa_DrawB()
{
	int     index = 0;
	int24_t rad[2], rad2[2];

	for (index = 0; index < 3; index++)
	{
		ui_ClearFraction(funcFracPoints[index]);
		ui_ClearFraction(func2FracPoints[index]);
	}

	// When angle BBB is selected
	sp_SetLabel(&xsides_oah[0], lbl_OPP); // bbb = opp
	sp_SetLabel(&xsides_oah[1], lbl_ADJ); // aaa = adj
	sp_SetLabel(&xsides_oah[2], lbl_HYP); // ccc = hyp

	gfx_PrintColor(&ui_Wait, gfx_red);
	if (!ui_dispSimpRoot)
	{
		// SOH
		//sp_SetFuncDataLabel(triangle.b, triangle.c, 0);
		gfx_Fraction(funcFracPoints[0], triangle.b, triangle.c);

		// CAH
		//sp_SetFuncDataLabel(triangle.a, triangle.c, 1);
		gfx_Fraction(funcFracPoints[1], triangle.a, triangle.c);

		// TOA
		//sp_SetFuncDataLabel(triangle.b, triangle.a, 2);
		gfx_Fraction(funcFracPoints[2], triangle.b, triangle.a);

		// cot = adj/opp
		//sp_SetFuncData2Label(triangle.a, triangle.b, 0);
		gfx_Fraction(func2FracPoints[0], triangle.a, triangle.b);

		// sec = hyp/adj
		//sp_SetFuncData2Label(triangle.c, triangle.a, 1);
		gfx_Fraction(func2FracPoints[1], triangle.c, triangle.a);

		// csc = hyp/opp
		//sp_SetFuncData2Label(triangle.c, triangle.b, 2);
		gfx_Fraction(func2FracPoints[2], triangle.c, triangle.b);
	}
	else
	{
		//todo

		// SOH
		SimplifyRadicalFromDecimal(triangle.b, rad);
		SimplifyRadicalFromDecimal(triangle.c, rad2);
		gfx_RadicalFraction(funcFracPoints[0], rad[0], rad[1], rad2[0], rad2[1]);

		// CAH
		SimplifyRadicalFromDecimal(triangle.a, rad);
		SimplifyRadicalFromDecimal(triangle.c, rad2);
		gfx_RadicalFraction(funcFracPoints[1], rad[0], rad[1], rad2[0], rad2[1]);

		// TOA
		SimplifyRadicalFromDecimal(triangle.b, rad);
		SimplifyRadicalFromDecimal(triangle.a, rad2);
		gfx_RadicalFraction(funcFracPoints[2], rad[0], rad[1], rad2[0], rad2[1]);

		// cot = adj/opp
		SimplifyRadicalFromDecimal(triangle.a, rad);
		SimplifyRadicalFromDecimal(triangle.b, rad2);
		gfx_RadicalFraction(func2FracPoints[0], rad[0], rad[1], rad2[0], rad2[1]);

		// sec = hyp/adj
		SimplifyRadicalFromDecimal(triangle.c, rad);
		SimplifyRadicalFromDecimal(triangle.a, rad2);
		gfx_RadicalFraction(func2FracPoints[1], rad[0], rad[1], rad2[0], rad2[1]);

		// csc = hyp/opp
		SimplifyRadicalFromDecimal(triangle.c, rad);
		SimplifyRadicalFromDecimal(triangle.b, rad2);
		gfx_RadicalFraction(func2FracPoints[2], rad[0], rad[1], rad2[0], rad2[1]);
	}

	gfx_Clear(&ui_Wait);

	for (index = 0; index < 3; index++)
	{
		gfx_PrintColor(&xsides_oah[index], gfx_green);
		gfx_PrintColor(&funcData[index], gfx_blue);
		gfx_PrintColor(&funcData2[index], gfx_blue);
	}
}

void right_Redraw()
{
	int index = 0;

	for (; index < 3; index++)
	{
		gfx_Clear(&xangles[index]);
		gfx_Print(&xangles[index]);

		gfx_Clear(&xsides[index]);
		gfx_Print(&xsides[index]);
	}
}

void sohcahtoa_Draw90()
{
	int index = 0;
	// Draw nothing when 90 is selected
	sp_SetLabel(&xsides_oah[0], "B"); // bbb
	sp_SetLabel(&xsides_oah[1], "A"); // aaa
	sp_SetLabel(&xsides_oah[2], "C"); // ccc

	for (index = 0; index < 3; index++)
	{
		gfx_Clear(&xsides_oah[index]);
		gfx_Clear(&funcData[index]);
		gfx_Clear(&funcData2[index]);
		ui_ClearFraction(funcFracPoints[index]);
		ui_ClearFraction(func2FracPoints[index]);
	}
}

void disp_Simplified(real_t* r)
{
	int24_t rad[2];

	if (!ui_dispSimpRoot)
	{
		return;
	}
	gfx_PrintColor(&ui_Wait, gfx_red);

	dbg_sprintf(dbgout, "Simplifying radical...\n");
	SimplifyRadicalFromDecimal(*r, rad);

	ui_ClearDispSimpRoot();

	gfx_Sqrt(ui_RootPoint, rad[0], rad[1]);
	gfx_Clear(&ui_Wait);
}

void ui_ClearArea(gfx_point_t point, uint8_t w, uint8_t h)
{
	gfx_SetColor(gfx_white);
	gfx_FillRectangle(point.x, point.y, w, h);
	gfx_SetColor(gfx_black);
}

void ui_ClearDispSimpRoot()
{
	static const gfx_point_t point = {230, 20};
	ui_ClearArea(point, 70, 35);
}

void ui_SwitchDispSimpRoot()
{
	if (ui_dispSimpRoot)
	{
		ui_ClearDispSimpRoot();
		ui_dispSimpRoot = false;
		gfx_PrintColor(&ui_btn_EnableSimpRoot, gfx_red);
	}
	else if (!ui_dispSimpRoot)
	{
		ui_dispSimpRoot = true;
		gfx_PrintColor(&ui_btn_EnableSimpRoot, gfx_green);
	}
}

void right_SelectSide()
{
	uint8_t key;
	superpoint_t* currentSelection;

	currentSelection = &side_b;
	gfx_HighlightPoint(&side_b);
	sohcahtoa_Draw90();
	RECURSE:
	while ((key = os_GetCSC()) != sk_Enter)
	{

		if (key == sk_Clear)
		{
			return;
		}

		if (key == sk_Zoom)
		{
			ui_SwitchDispSimpRoot();
		}

		if (key == sk_Trace)
		{
			// todo
			currentSelection = &side_a;
			gfx_HighlightPoint(&side_a);
		}

		if (key == sk_Graph)
		{
			gfx_Clear(&ui_Mode);

			sp_SetLabel(&ui_Mode, lbl_AngleMode);
			gfx_Print(&ui_Mode);
			gfx_ClearHighlight(currentSelection);
			ui_ClearDispSimpRoot();
			right_SelectAngle();
			return;
		}

		/* bbb -> ccc */
		if (key == sk_Right && PointEq(*currentSelection, side_b))
		{

			gfx_ClearHighlight(&side_b);
			gfx_HighlightPoint(&side_c);
			currentSelection = &side_c;
			if (trigstatus.c)
			{
				disp_Simplified(&triangle.c);
			}
		}

		/* bbb -> aaa */
		if (key == sk_Down && PointEq(*currentSelection, side_b))
		{
			gfx_ClearHighlight(&side_b);
			gfx_HighlightPoint(&side_a);
			currentSelection = &side_a;
			if (trigstatus.a)
			{
				disp_Simplified(&triangle.a);
			}
		}

		/* ccc -> aaa */
		if (key == sk_Down && PointEq(*currentSelection, side_c))
		{
			gfx_ClearHighlight(&side_c);
			gfx_HighlightPoint(&side_a);
			currentSelection = &side_a;
			if (trigstatus.a)
			{
				disp_Simplified(&triangle.a);
			}
		}

		// [0] bbb
		// [1] aaa
		// [2] ccc

		/* ccc -> bbb */
		if (key == sk_Left && PointEq(*currentSelection, side_c))
		{
			gfx_ClearHighlight(&side_c);
			gfx_HighlightPoint(&side_b);
			currentSelection = &side_b;
			if (trigstatus.b)
			{
				disp_Simplified(&triangle.b);
			}
		}

		/* aaa -> ccc */
		if (key == sk_Up || key == sk_Right && PointEq(*currentSelection, side_a))
		{
			gfx_ClearHighlight(&side_a);
			gfx_HighlightPoint(&side_c);
			currentSelection = &side_c;
			if (trigstatus.c)
			{
				disp_Simplified(&triangle.c);
			}
		}

		/* aaa -> bbb */
		if (key == sk_Left && PointEq(*currentSelection, side_a))
		{
			gfx_ClearHighlight(&side_a);
			gfx_HighlightPoint(&side_b);
			currentSelection = &side_b;
			if (trigstatus.b)
			{
				disp_Simplified(&triangle.b);
			}
		}
	}

	// [0] bbb
	// [1] aaa
	// [2] ccc

	if (PointEq(*currentSelection, side_b))
	{
		dbg_sprintf(dbgout, "[RightTrig] User selected side %s\n", side_b.label);

		triangle.b   = io_gfx_ReadReal(&side_b);
		trigstatus.b = true;
	}

	if (PointEq(*currentSelection, side_a))
	{
		dbg_sprintf(dbgout, "[RightTrig] User selected side %s\n", side_a.label);
		triangle.a   = io_gfx_ReadReal(&side_a);
		trigstatus.a = true;
	}

	if (PointEq(*currentSelection, side_c))
	{
		dbg_sprintf(dbgout, "[RightTrig] User selected side %s\n", side_c.label);
		triangle.c   = io_gfx_ReadReal(&side_c);
		trigstatus.c = true;
	}

	pythag_CheckSolvability();
	right_Sync();
	gfx_HighlightPoint(currentSelection);

	goto RECURSE;
}

void right_SelectAngle()
{
	uint8_t key;
	superpoint_t* currentSelection;
	const real_t real90 = os_Int24ToReal(90);
	currentSelection = &angle_C;
	gfx_HighlightPoint(&angle_C);
	sohcahtoa_Draw90();
	RECURSE:
	while ((key = os_GetCSC()) != sk_Enter)
	{
		/**
		 * const gfx_point_t angles[3] =
		 * [0] = 90
		 * [1] = BBB
		 * [2] = AAA
		 */

		/**
		 * superpoint_t xsides_oah[3] =
		 * [0] = bbb
		 * [1] = aaa
		 * [2] = ccc
		 */

		if (key == sk_Clear)
		{
			return;
		}

		if (key == sk_Zoom)
		{
			ui_SwitchDispSimpRoot();
		}

		if (key == sk_Trace)
		{
			// todo
			currentSelection = &side_a;
			gfx_HighlightPoint(&side_a);
		}

		if (key == sk_Graph)
		{
			gfx_Clear(&ui_Mode);
			//mode.label = "ANGLE MODE";
			sp_SetLabel(&ui_Mode, lbl_SideMode);
			gfx_Print(&ui_Mode);
			gfx_ClearHighlight(currentSelection);
			ui_ClearDispSimpRoot();
			right_SelectSide();
			return;
		}

		/* 90 -> BBB */
		if (key == sk_Right && PointEq(*currentSelection, angle_C))
		{
			gfx_ClearHighlight(&angle_C);
			gfx_HighlightPoint(&angle_B);
			currentSelection = &angle_B;

			sohcahtoa_DrawB();
		}

		/* 90 -> AAA */
		if (key == sk_Up && PointEq(*currentSelection, angle_C))
		{
			gfx_ClearHighlight(&angle_C);
			gfx_HighlightPoint(&angle_A);
			currentSelection = &angle_A;

			sohcahtoa_DrawA();
		}

		/* AAA -> 90 */
		if (key == sk_Down && PointEq(*currentSelection, angle_A))
		{
			gfx_ClearHighlight(&angle_A);
			gfx_HighlightPoint(&angle_C);
			currentSelection = &angle_C;

			sohcahtoa_Draw90();
		}

		/* AAA -> BBB */
		if (key == sk_Right && PointEq(*currentSelection, angle_A))
		{
			gfx_ClearHighlight(&angle_A);
			gfx_HighlightPoint(&angle_B);
			currentSelection = &angle_B;

			sohcahtoa_DrawB();
		}

		/* BBB -> 90 */
		if (key == sk_Left && PointEq(*currentSelection, angle_B))
		{
			gfx_ClearHighlight(&angle_B);
			gfx_HighlightPoint(&angle_C);
			currentSelection = &angle_C;

			sohcahtoa_Draw90();
		}

		/* BBB -> AAA */
		if (key == sk_Up && PointEq(*currentSelection, angle_B))
		{
			gfx_ClearHighlight(&angle_B);
			gfx_HighlightPoint(&angle_A);
			currentSelection = &angle_A;

			sohcahtoa_DrawA();
		}
	}

	if (PointEq(*currentSelection, angle_C))
	{
		dbg_sprintf(dbgout, "[RightTrig] User selected angle 90, returning to angle selection\n");
		goto RECURSE;
	}

	if (PointEq(*currentSelection, angle_B))
	{
		dbg_sprintf(dbgout, "[RightTrig] User selected angle %s\n", angle_B.label);

		// 90
		// BBB
		// AAA

		triangle.B   = io_gfx_ReadReal(&angle_B);
		trigstatus.B = true;
	}

	if (PointEq(*currentSelection, angle_A))
	{
		dbg_sprintf(dbgout, "[RightTrig] User selected angle %s\n", angle_A.label);

		triangle.A   = io_gfx_ReadReal(&angle_A);
		trigstatus.A = true;
	}

	if (trigstatus.A)
	{
		triangle.B   = os_RealSub(&real90, &triangle.A);
		trigstatus.B = true;
		gfx_Clear(&angle_B);
	}
	else if (trigstatus.B)
	{
		triangle.A   = os_RealSub(&real90, &triangle.B);
		trigstatus.A = true;
		gfx_Clear(&angle_A);
	}

	right_Sync();
	gfx_HighlightPoint(currentSelection);
	goto RECURSE;
}

void right_SolveTriangle()
{
	int index;

	/* Triangle coordinates */
	const int verts[6] = {
			30, 10,   /* (x0, y0) */
			30, 129,  /* (x1, y1) */
			189, 129, /* (x2, y2) */
	};

	triangle.C   = os_Int24ToReal(90);
	trigstatus.C = true;
	ui_dispSimpRoot = true;

	gfx_Begin();
	gfx_SetColor(gfx_blue);
	gfx_SetTextFGColor(gfx_black);

	/* Leg a */
	gfx_Line(verts[0], verts[1], verts[2], verts[3]);

	/* Leg b */
	gfx_Line(verts[2], verts[3], verts[4], verts[5]);

	/* Hypotenuse */
	gfx_Line(verts[4], verts[5], verts[0], verts[1]);

	// Leg b, a, c
	for (index = 0; index < 3; index++)
	{
		gfx_Print(&xangles[index]);
		gfx_Print(&xsides[index]);
	}

	gfx_Print(&ui_btn_Clear);
	gfx_Print(&ui_btn_Mode);
	gfx_PrintColor(&ui_btn_EnableSimpRoot, gfx_green);
	gfx_Print(&ui_Mode);

	right_SelectAngle();

	gfx_ZeroScreen();
	gfx_End();

}
