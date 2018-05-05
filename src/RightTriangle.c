//
// Created by Decimation on 4/28/2018.
//

#include <debug.h>
#include "RightTriangle.h"

//
// Created by Decimation on 4/14/2018.
//

#include "Library.h"
#include "Geometry.h"
#include "Simplifiers.h"

static superpoint_t xangles[3] = {
		{{30 + 5,   129 - 10}, "90"},
		{{30 + 120, 129 - 10}, "B"},
		{{37,       27},       "A"}
};

static superpoint_t xsides[3] = {
		{{30 - 25, 129 - 50}, "b"},
		{{30 + 50, 129 + 10}, "a"},
		{{30 + 90, 129 - 70}, "c"}
};

static superpoint_t xsides_oah[3] = {
		{{30 - 25, 129 - 40}, "..."},
		{{30 + 50, 129 + 20}, "..."},
		{{30 + 90, 129 - 60}, "..."}
};


static const superpoint_t ui_btn_Mode           = {280, 230, "Mode"};
static const superpoint_t ui_btn_Clear          = {215, 230, "Clear"};
static const superpoint_t ui_btn_EnableSimpRoot = {150, 230, "Root"};

static bool              ui_dispSimpRoot;
static superpoint_t      ui_Wait                = {{230, 40}, "Processing..."};
static superpoint_t      ui_Mode                = {{230, 10}, "ANGLE MODE"};
static const gfx_point_t ui_RootPoint           = {230, 25};
static superpoint_t      ui_Type                = {{230, 70}, "..."};

static const char lbl_AngleMode[] = "ANGLE MODE";
static const char lbl_SideMode[]  = "SIDE MODE";
static const char lbl_ADJ[]       = "ADJ";
static const char lbl_HYP[]       = "HYP";
static const char lbl_OPP[]       = "OPP";

static triangle_t   triangle;
static trigstatus_t trigstatus;

static bool is45_45_90;
static bool is30_60_90;

static const char ui_Theta = (char) 91;

static superpoint_t* currentSelection;

/**
 * Triangle coordinates
 */
const int verts[6] = {
		30,
		10,   /* (x0, y0) */
		30,
		129,  /* (x1, y1) */
		189,
		129, /* (x2, y2) */
};

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
		{10,  210, "csc = "},
		{110, 210, "sec = "},
		{210, 210, "cot = "}
};

static const gfx_point_t func2FracPoints[3] = {
		{10 + 35,  210},
		{110 + 35, 210},
		{210 + 35, 210}
};

#define fnCotPt func2FracPoints[2]
#define fnSecPt func2FracPoints[1]
#define fnCscPt func2FracPoints[0]

#define fnSinPt funcFracPoints[0]
#define fnCosPt funcFracPoints[1]
#define fnTanPt funcFracPoints[2]

#define side_a xsides[1]
#define side_b xsides[0]
#define side_c xsides[2]

#define angle_A xangles[2]
#define angle_B xangles[1]
#define angle_C xangles[0]


static void ui_AutoDrawFunctions()
{
	if (PointEq(*currentSelection, angle_A))
	{
		ui_DrawFunctions_A();
	}
	else if (PointEq(*currentSelection, angle_B))
	{
		ui_DrawFunctions_B();
	}
	else if (PointEq(*currentSelection, angle_C))
	{
		ui_DrawFunctions_90();
	}
}

static void right_DebugTriangle()
{
	char buf[20];
	os_RealToStr(buf, &triangle.A, 0, 0, -1);
	dbg_sprintf(dbgout, "Angle A: %s | ", buf);
	os_RealToStr(buf, &triangle.a, 0, 0, -1);
	dbg_sprintf(dbgout, "Side a: %s\n", buf);

	os_RealToStr(buf, &triangle.B, 0, 0, -1);
	dbg_sprintf(dbgout, "Angle B: %s | ", buf);
	os_RealToStr(buf, &triangle.b, 0, 0, -1);
	dbg_sprintf(dbgout, "Side b: %s\n", buf);

	os_RealToStr(buf, &triangle.C, 0, 0, -1);
	dbg_sprintf(dbgout, "Angle C: %s | ", buf);
	os_RealToStr(buf, &triangle.c, 0, 0, -1);
	dbg_sprintf(dbgout, "Side c: %s\n", buf);

	dbg_sprintf(dbgout, "Angle availability: [%s, %s, %s]\n", trigstatus.A ? "1" : "0", trigstatus.B ? "1" : "0",
				trigstatus.C ? "1" : "0");
	dbg_sprintf(dbgout, "Side availability: [%s, %s, %s]\n", trigstatus.a ? "1" : "0", trigstatus.b ? "1" : "0",
				trigstatus.c ? "1" : "0");

	os_RealToStr(buf, &triangle.area, 0, 0, -1);
	dbg_sprintf(dbgout, "Area: %s\n", buf);
	os_RealToStr(buf, &triangle.perimeter, 0, 0, -1);
	dbg_sprintf(dbgout, "Perimeter: %s\n", buf);
}

/**
 * Uses basic heuristics to automatically detect if a triangle is
 * a special right triangle (45-45-90, 30-60-90)
 *
 * An SRT will automatically be detected if
 *  - Its angles are detected
 *  - Sides of a 45-45-90 are detected
 *
 * A 30-60-90 will not be detected by its sides.
 */
static void right_Sync()
{
	const real_t real45 = os_Int24ToReal(45);
	const real_t real60 = os_Int24ToReal(60);
	const real_t real30 = os_Int24ToReal(30);
	const real_t real2  = os_Int24ToReal(2);
	const real_t sqrt2  = os_RealSqrt(&real2);
	const real_t real3  = os_Int24ToReal(3);
	const real_t sqrt3  = os_RealSqrt(&real3);


	dbg_sprintf(dbgout, "[RightTrig] Synchronizing right triangle...\n");
	geo_RoundTriangle(&triangle, gRound);

	if (trigstatus.A)
	{
		os_RealToStr(angle_A.label, &triangle.A, 0, 0, 6);
	}
	if (trigstatus.B)
	{
		os_RealToStr(angle_B.label, &triangle.B, 0, 0, 6);
	}

	/**
	 * Detect if the triangle is a special right triangle by checking its angles
	 */
	if (os_RealCompare(&triangle.A, &real45) == 0 || os_RealCompare(&triangle.B, &real45) == 0)
	{
		is45_45_90 = true;
	}
	if (os_RealCompare(&triangle.A, &real30) == 0 || os_RealCompare(&triangle.B, &real60) == 0)
	{
		is30_60_90 = true;

		dbg_sprintf(dbgout, "[RightTrig] 30-60-90 case: Side a is x, angle A is 30\n");
		/**
		 * 30-60-90 case:
		 * Side a is x
		 * Angle A is 30
		 */
		if (trigstatus.a && os_RealCompare(&triangle.A, &real30) == 0)
		{
			triangle.b   = os_RealMul(&triangle.a, &sqrt3);
			trigstatus.b = true;
			triangle.c   = os_RealMul(&triangle.a, &real2);
			trigstatus.c = true;
		}
		else if (trigstatus.b && os_RealCompare(&triangle.A, &real30) == 0)
		{
			triangle.a   = os_RealDiv(&triangle.b, &sqrt3);
			trigstatus.a = true;
			triangle.c   = os_RealMul(&triangle.a, &real2);
			trigstatus.c = true;
		}
		else if (trigstatus.c && os_RealCompare(&triangle.A, &real30) == 0)
		{
			triangle.a   = os_RealDiv(&triangle.c, &real2);
			trigstatus.a = true;
			triangle.b   = os_RealMul(&triangle.a, &sqrt3);
			trigstatus.b = true;
		}
		geo_RoundTriangle(&triangle, 1);
	}
	else if (os_RealCompare(&triangle.A, &real60) == 0 || os_RealCompare(&triangle.B, &real30) == 0)
	{
		is30_60_90 = true;

		dbg_sprintf(dbgout, "[RightTrig] 30-60-90 case: Side b is x, angle A is 60\n");
		/**
		 * 30-60-90 case:
		 * Side b is x
		 * Angle A is 60
		 */
		if (trigstatus.b && os_RealCompare(&triangle.A, &real60) == 0)
		{
			triangle.a   = os_RealMul(&triangle.b, &sqrt3);
			trigstatus.a = true;
			triangle.c   = os_RealMul(&triangle.b, &real2);
			trigstatus.c = true;
		}
		else if (trigstatus.a && os_RealCompare(&triangle.A, &real60) == 0)
		{
			triangle.b   = os_RealDiv(&triangle.a, &sqrt3);
			trigstatus.b = true;
			triangle.c   = os_RealMul(&triangle.b, &real2);
			trigstatus.c = true;
		}
		else if (trigstatus.c && os_RealCompare(&triangle.A, &real60) == 0)
		{
			triangle.b   = os_RealDiv(&triangle.c, &real2);
			trigstatus.b = true;
			triangle.a   = os_RealMul(&triangle.b, &sqrt3);
			trigstatus.a = true;
		}

		geo_RoundTriangle(&triangle, 1);
	}

	/**
	 * Detect if the triangle is a special right triangle by checking its sides
	 *
	 * If it's a 45-45-90, both shorter legs will be congruent (a and b)
	 */
	if ((trigstatus.a && trigstatus.b) && os_RealCompare(&triangle.a, &triangle.b) == 0)
	{
		is45_45_90 = true;


	}

	if (is30_60_90)
	{
		sp_SetLabel(&ui_Type, "30-60-90");
		gfx_PrintColor(&ui_Type, gfx_blue);
		dbg_sprintf(dbgout, "30-60-90 detected\n");
	}
	if (is45_45_90)
	{
		sp_SetLabel(&ui_Type, "45-45-90");
		gfx_PrintColor(&ui_Type, gfx_blue);
		dbg_sprintf(dbgout, "45-45-90 detected\n");
	}

	if (trigstatus.a)
	{
		os_RealToStr(side_a.label, &triangle.a, 0, 0, 6);
	}
	if (trigstatus.a && is45_45_90)
	{
		triangle.b   = triangle.a;
		triangle.c   = os_RealMul(&triangle.a, &sqrt2);
		trigstatus.b = true;
		trigstatus.c = true;

	}


	if (trigstatus.b)
	{
		os_RealToStr(side_b.label, &triangle.b, 0, 0, 6);
	}
	if (trigstatus.b && is45_45_90)
	{
		triangle.a   = triangle.b;
		triangle.c   = os_RealMul(&triangle.b, &sqrt2);
		trigstatus.a = true;
		trigstatus.c = true;
		os_RealToStr(side_a.label, &triangle.a, 0, 0, 6);
	}


	if (trigstatus.c)
	{
		os_RealToStr(side_c.label, &triangle.c, 0, 0, 6);
	}
	if (trigstatus.c && is45_45_90)
	{
		triangle.a   = os_RealDiv(&triangle.c, &sqrt2);
		triangle.b   = triangle.a;
		trigstatus.a = true;
		trigstatus.b = true;
		os_RealToStr(side_a.label, &triangle.a, 0, 0, 6);
		os_RealToStr(side_b.label, &triangle.b, 0, 0, 6);
	}


	right_TruncateLabels(gDigitThreshold);
	right_Redraw();
	ui_AutoDrawFunctions();
}

static void right_TruncateLabels(int len)
{
	int i = 0;
	for (; i < 3; i++)
	{
		lib_StrCut(xangles[i].label, len, 20 - len);
		lib_StrCut(xsides[i].label, len, 20 - len);
	}
}

// todo: check domain and imaginary numbers
static void pythag_CheckSolvability()
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

static void ui_ClearFraction(gfx_point_t point)
{
	static const uint8_t w = 60;
	static const uint8_t h = 35;
	point.x -= 1;
	point.y -= 16;
	ui_ClearArea(point, w, h);
}

static void ui_DispFunctionRatioRoot(gfx_point_t pt, real_t x, real_t y)
{
	int24_t rad[2], rad2[2];

	SimplifyRadicalFromDecimal(x, rad);
	SimplifyRadicalFromDecimal(y, rad2);
	gfx_RadicalFraction(pt, rad[0], rad[1], rad2[0], rad2[1]);
}

static void ui_DrawFunctions_A()
{
	int index = 0;

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
		gfx_Fraction(fnSinPt, triangle.a, triangle.c);

		// CAH
		gfx_Fraction(fnCosPt, triangle.b, triangle.c);

		// TOA
		gfx_Fraction(fnTanPt, triangle.a, triangle.b);

		// cot = adj/opp
		gfx_Fraction(fnCotPt, triangle.b, triangle.a);

		// sec = hyp/adj
		gfx_Fraction(fnSecPt, triangle.c, triangle.b);

		// csc = hyp/opp
		gfx_Fraction(fnCscPt, triangle.c, triangle.a);
	}
	else
	{
		// SOH
		ui_DispFunctionRatioRoot(fnSinPt, triangle.a, triangle.c);

		// CAH
		ui_DispFunctionRatioRoot(fnCosPt, triangle.b, triangle.c);

		// TOA
		ui_DispFunctionRatioRoot(fnTanPt, triangle.a, triangle.b);

		// cot = adj/opp
		ui_DispFunctionRatioRoot(fnCotPt, triangle.b, triangle.a);

		// sec = hyp/adj
		ui_DispFunctionRatioRoot(fnSecPt, triangle.c, triangle.b);

		// csc = hyp/opp
		ui_DispFunctionRatioRoot(fnCscPt, triangle.c, triangle.a);
	}
	gfx_Clear(&ui_Wait);
	dbg_sprintf(dbgout, "Completed drawing of A ratios\n");

	for (index = 0; index < 3; index++)
	{
		gfx_PrintColor(&xsides_oah[index], gfx_green);
		gfx_PrintColor(&funcData[index], gfx_blue);
		gfx_PrintColor(&funcData2[index], gfx_blue);
	}
	dbg_sprintf(dbgout, "Completed printing of labels for A\n");
}

static void ui_DrawFunctions_B()
{
	int index = 0;

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
		gfx_Fraction(fnSinPt, triangle.b, triangle.c);

		// CAH
		gfx_Fraction(fnCosPt, triangle.a, triangle.c);

		// TOA
		gfx_Fraction(fnTanPt, triangle.b, triangle.a);

		// cot = adj/opp
		gfx_Fraction(fnCotPt, triangle.a, triangle.b);

		// sec = hyp/adj
		gfx_Fraction(fnSecPt, triangle.c, triangle.a);

		// csc = hyp/opp
		gfx_Fraction(fnCscPt, triangle.c, triangle.b);
	}
	else
	{
		// SOH
		ui_DispFunctionRatioRoot(fnSinPt, triangle.b, triangle.c);

		// CAH
		ui_DispFunctionRatioRoot(fnCosPt, triangle.a, triangle.c);

		// TOA
		ui_DispFunctionRatioRoot(fnTanPt, triangle.b, triangle.a);

		// cot = adj/opp
		ui_DispFunctionRatioRoot(fnCotPt, triangle.a, triangle.b);

		// sec = hyp/adj
		ui_DispFunctionRatioRoot(fnSecPt, triangle.c, triangle.a);

		// csc = hyp/opp
		ui_DispFunctionRatioRoot(fnCscPt, triangle.c, triangle.b);
	}
	gfx_Clear(&ui_Wait);
	dbg_sprintf(dbgout, "Completed drawing of B ratios\n");

	for (index = 0; index < 3; index++)
	{
		gfx_PrintColor(&xsides_oah[index], gfx_green);
		gfx_PrintColor(&funcData[index], gfx_blue);
		gfx_PrintColor(&funcData2[index], gfx_blue);
	}
	dbg_sprintf(dbgout, "Completed printing of labels for B\n");
}

static void right_Redraw()
{
	int index = 0;

	for (; index < 3; index++)
	{
		gfx_Clear(&xangles[index]);
		gfx_Print(&xangles[index]);

		gfx_Clear(&xsides[index]);
		gfx_Print(&xsides[index]);
	}

	right_DrawSides();
}

static void ui_DrawFunctions_90()
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

static void ui_DispSimplified(real_t* r)
{
	int24_t rad[2];

	if (!ui_dispSimpRoot)
	{
		return;
	}
	gfx_PrintColor(&ui_Wait, gfx_red);

	dbg_sprintf(dbgout, "[RightTrig] Simplifying radical...\n");
	SimplifyRadicalFromDecimal(*r, rad);

	ui_ClearDispSimpRoot();

	gfx_Sqrt(ui_RootPoint, rad[0], rad[1]);
	gfx_Clear(&ui_Wait);
}

static void ui_ClearArea(gfx_point_t point, uint8_t w, uint8_t h)
{
	gfx_SetColor(gfx_white);
	gfx_FillRectangle(point.x, point.y, w, h);
	gfx_SetColor(gfx_black);
}

static void ui_ClearDispSimpRoot()
{
	static const gfx_point_t point = {230, 20};
	ui_ClearArea(point, 70, 35);
}

/**
 * Toggles displaying of simplified root form
 */
static void ui_SwitchDispSimpRoot()
{
	real_t buf;
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
		buf = os_StrToReal(currentSelection->label, NULL);
		ui_DispSimplified(&buf);
	}

	ui_AutoDrawFunctions();
}

static void right_Reset()
{
	int i = 0;
	bool  * bptr = (bool*) &trigstatus;
	real_t* rptr = (real_t*) &triangle;

	for (; i < sizeof(trigstatus) / sizeof(bool); i++)
	{
		bptr[i] = false;
	}
	for (i = 0; i < sizeof(triangle) / sizeof(real_t); i++)
	{
		rptr[i] = os_Int24ToReal(0);
	}

	sp_SetLabel(&side_a, "a");
	sp_SetLabel(&side_b, "b");
	sp_SetLabel(&side_c, "c");

	sp_SetLabel(&angle_A, "A");
	sp_SetLabel(&angle_B, "B");

	sp_SetLabel(&ui_Type, "...");

	is45_45_90 = false;
	is30_60_90 = false;

	right_Redraw();
	right_Sync();
	ui_ClearDispSimpRoot();
	gfx_Clear(&ui_Type);
	dbg_sprintf(dbgout, "[RightTrig] Reset triangle\n");


}

static void right_SelectSide()
{
	uint8_t key;


	currentSelection = &side_b;
	gfx_HighlightPoint(&side_b);
	ui_DrawFunctions_90();
	ui_DispSimplified(&triangle.b);
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
			right_Reset();
			gfx_SetFocus(&currentSelection, currentSelection, &side_a);
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

			gfx_SetFocus(&currentSelection, &side_b, &side_c);
			if (trigstatus.c)
			{
				ui_DispSimplified(&triangle.c);
			}
		}

		/* bbb -> aaa */
		if (key == sk_Down && PointEq(*currentSelection, side_b))
		{
			gfx_SetFocus(&currentSelection, &side_b, &side_a);
			if (trigstatus.a)
			{
				ui_DispSimplified(&triangle.a);
			}
		}

		/* ccc -> aaa */
		if (key == sk_Down && PointEq(*currentSelection, side_c))
		{
			gfx_SetFocus(&currentSelection, &side_c, &side_a);
			if (trigstatus.a)
			{
				ui_DispSimplified(&triangle.a);
			}
		}

		/* ccc -> bbb */
		if (key == sk_Left && PointEq(*currentSelection, side_c))
		{
			gfx_SetFocus(&currentSelection, &side_c, &side_b);
			if (trigstatus.b)
			{
				ui_DispSimplified(&triangle.b);
			}
		}

		/* aaa -> ccc */
		if (key == sk_Up || key == sk_Right && PointEq(*currentSelection, side_a))
		{
			gfx_SetFocus(&currentSelection, &side_a, &side_c);
			if (trigstatus.c)
			{
				ui_DispSimplified(&triangle.c);
			}
		}

		/* aaa -> bbb */
		if (key == sk_Left && PointEq(*currentSelection, side_a))
		{
			gfx_SetFocus(&currentSelection, &side_a, &side_b);
			if (trigstatus.b)
			{
				ui_DispSimplified(&triangle.b);
			}
		}
	}

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

static void right_SelectAngle()
{
	uint8_t key;

	const real_t real90 = os_Int24ToReal(90);
	currentSelection = &angle_C;
	gfx_HighlightPoint(&angle_C);
	ui_DrawFunctions_90();
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
			right_Reset();
			gfx_SetFocus(&currentSelection, currentSelection, &angle_C);
			ui_DrawFunctions_90();
		}

		if (key == sk_Graph)
		{
			gfx_Clear(&ui_Mode);
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
			gfx_SetFocus(&currentSelection, &angle_C, &angle_B);

			ui_DrawFunctions_B();
		}

		/* 90 -> AAA */
		if (key == sk_Up && PointEq(*currentSelection, angle_C))
		{
			gfx_SetFocus(&currentSelection, &angle_C, &angle_A);

			ui_DrawFunctions_A();
		}

		/* AAA -> 90 */
		if (key == sk_Down && PointEq(*currentSelection, angle_A))
		{
			gfx_SetFocus(&currentSelection, &angle_A, &angle_C);

			ui_DrawFunctions_90();
		}

		/* AAA -> BBB */
		if (key == sk_Right && PointEq(*currentSelection, angle_A))
		{
			gfx_SetFocus(&currentSelection, &angle_A, &angle_B);

			ui_DrawFunctions_B();
		}

		/* BBB -> 90 */
		if (key == sk_Left && PointEq(*currentSelection, angle_B))
		{
			gfx_SetFocus(&currentSelection, &angle_B, &angle_C);

			ui_DrawFunctions_90();
		}

		/* BBB -> AAA */
		if (key == sk_Up && PointEq(*currentSelection, angle_B))
		{
			gfx_SetFocus(&currentSelection, &angle_B, &angle_A);

			ui_DrawFunctions_A();
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

static void right_DrawSides()
{
	/* Leg a */
	gfx_Line(verts[0], verts[1], verts[2], verts[3]);

	/* Leg b */
	gfx_Line(verts[2], verts[3], verts[4], verts[5]);

	/* Hypotenuse */
	gfx_Line(verts[4], verts[5], verts[0], verts[1]);
}

void right_SolveTriangle()
{
	int index;

	triangle.C   = os_Int24ToReal(90);
	trigstatus.C = true;
	ui_dispSimpRoot = true;

	gfx_Begin();
	gfx_SetColor(gfx_blue);
	gfx_SetTextFGColor(gfx_black);

	right_DrawSides();

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
