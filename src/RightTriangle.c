//
// Created by Decimation on 4/14/2018.
//

#include <debug.h>
#include <string.h>
#include "RightTriangle.h"
#include "Library.h"
#include "Geometry.h"
#include "Simplifiers.h"
#include "Types.h"
#include "Trigonometry.h"

static const char s_lbl_szEllipses[] = "...";
static const char s_lbl_szAngleA[]   = "A";
static const char s_lbl_szAngleB[]   = "B";
static const char s_lbl_szAngle90[]  = "90";

static const char s_lbl_szSideA[] = "a";
static const char s_lbl_szSideB[] = "b";
static const char s_lbl_szSideC[] = "c";

static labelpoint_t s_rgAngles[3] = {
		{{30 + 5,   129 - 10}},           // 90
		{{30 + 120, 129 - 10}},           // B
		{{37,       27}}                  // A
};

static labelpoint_t s_rgSides[3] = {
		{{30 - 25, 129 - 50}},             // b
		{{30 + 50, 129 + 10}},             // a
		{{30 + 90, 129 - 70}}              // c
};

static labelpoint_t s_rgSidesOAH[3] = {
		{{30 - 25, 129 - 40}},
		{{30 + 50, 129 + 20}},
		{{30 + 90, 129 - 60}}
};


static const labelpoint_t s_ui_btnMode        = {280, 230, "Mode"};
static const labelpoint_t s_ui_btnClear       = {215, 230, "Clear"};
static const labelpoint_t s_ui_btnRoot        = {150, 230, "Root"};
static const labelpoint_t s_ui_btnRound       = {85, 230, "Round"};
static const labelpoint_t s_ui_btnRoundPlaces = {20, 230, "0"};

static bool              s_ui_bDispSimpRoot;
static labelpoint_t      s_ui_Wait            = {{230, 40}, "Processing..."};
static labelpoint_t      s_ui_Mode            = {{230, 10}, "ANGLE MODE"};
static const gfx_point_t s_ui_RootPoint       = {230, 25};
static labelpoint_t      s_ui_Type            = {{230, 70}, "..."};

static const char s_lbl_szAngleMode[] = "ANGLE MODE";
static const char s_lbl_szSideMode[]  = "SIDE MODE";
static const char s_lbl_szAdj[]       = "ADJ";
static const char s_lbl_szHyp[]       = "HYP";
static const char s_lbl_szOpp[]       = "OPP";

static triangle_t   s_triangle;
static trigstatus_t s_trigStatus;

static bool s_bIs45_45_90;
static bool s_bIs30_60_90;

// static const char ui_Theta = (char) 91;

static labelpoint_t* s_pCurrentSelection;

/**
 * Triangle coordinates
 */
const int s_rgVerts[] = {
		30,
		10,   /* (x0, y0) */
		30,
		129,  /* (x1, y1) */
		189,
		129, /* (x2, y2) */
};

static const labelpoint_t s_rgFuncData[] = {
		{10,  175, "sin = "},
		{110, 175, "cos = "},
		{210, 175, "tan = "}
};

static const gfx_point_t s_rgFuncFracPoints[] = {
		{10 + 35,  175},
		{110 + 35, 175},
		{210 + 35, 175}
};

static const labelpoint_t s_rgFuncData2[] = {
		{10,  210, "csc = "},
		{110, 210, "sec = "},
		{210, 210, "cot = "}
};

static const gfx_point_t s_rgFuncFracPoints2[] = {
		{10 + 35,  210},
		{110 + 35, 210},
		{210 + 35, 210}
};


static void ui_InitLabels()
{
	int i = 0;
	char* ptr;

	strcpy(Point_Angle_A.label, s_lbl_szAngleA);
	strcpy(Point_Angle_B.label, s_lbl_szAngleB);
	strcpy(Point_Angle_C.label, s_lbl_szAngle90);

	strcpy(Point_Side_a.label, s_lbl_szSideA);
	strcpy(Point_Side_b.label, s_lbl_szSideB);
	strcpy(Point_Side_c.label, s_lbl_szSideC);


	ptr = (char*) ((uint8_t*) &s_rgSidesOAH) + sizeof(gfx_point_t);
	for (; i < sizeof(s_rgSidesOAH) / sizeof(labelpoint_t); ++i) {
		strcpy(ptr, s_lbl_szEllipses);
		ptr += kLabelPointLabelSize;
	}
	ptr = NULL;


}

static void ui_AutoDrawFunctions()
{
	if (local_SelectionEquals(Point_Angle_A)) {
		ui_DrawFunctions_A();
	}
	else if (local_SelectionEquals(Point_Angle_B)) {
		ui_DrawFunctions_B();
	}
	else if (local_SelectionEquals(Point_Angle_C)) {
		ui_DrawFunctions_90();
	}
}

static void right_DebugTriangle()
{
	char buf[20];
	os_RealToStr(buf, &s_triangle.A, 0, 0, -1);
	dbg_sprintf(dbgout, "Angle A: %s | ", buf);
	os_RealToStr(buf, &s_triangle.a, 0, 0, -1);
	dbg_sprintf(dbgout, "Side a: %s\n", buf);

	os_RealToStr(buf, &s_triangle.B, 0, 0, -1);
	dbg_sprintf(dbgout, "Angle B: %s | ", buf);
	os_RealToStr(buf, &s_triangle.b, 0, 0, -1);
	dbg_sprintf(dbgout, "Side b: %s\n", buf);

	os_RealToStr(buf, &s_triangle.C, 0, 0, -1);
	dbg_sprintf(dbgout, "Angle C: %s | ", buf);
	os_RealToStr(buf, &s_triangle.c, 0, 0, -1);
	dbg_sprintf(dbgout, "Side c: %s\n", buf);

	dbg_sprintf(dbgout, "Angle availability: [%s, %s, %s]\n", s_trigStatus.A ? "1" : "0", s_trigStatus.B ? "1" : "0",
				s_trigStatus.C ? "1" : "0");
	dbg_sprintf(dbgout, "Side availability: [%s, %s, %s]\n", s_trigStatus.a ? "1" : "0", s_trigStatus.b ? "1" : "0",
				s_trigStatus.c ? "1" : "0");

	os_RealToStr(buf, &s_triangle.area, 0, 0, -1);
	dbg_sprintf(dbgout, "Area: %s\n", buf);
	os_RealToStr(buf, &s_triangle.perimeter, 0, 0, -1);
	dbg_sprintf(dbgout, "Perimeter: %s\n", buf);
}

static void ui_SetRounding()
{
	if (g_uRound == 9) {
		g_uRound = 0;
	}
	else if (g_uRound <= 9) {
		g_uRound++;
	}

	lp_SetLabelFromInt24(&s_ui_btnRoundPlaces, g_uRound);


}

static void local_SetSide(real_t r, sidetype_t sidetype)
{
	tri_SetSide(&s_triangle, r, &s_trigStatus, sidetype);
}

static void local_SetAngle(real_t r, angletype_t angletype)
{
	tri_SetAngle(&s_triangle, r, &s_trigStatus, angletype);
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
	LocalReal(45);
	LocalReal(30);
	LocalReal(60);
	LocalReal(2);
	LocalReal(3);

	const real_t sqrt2 = os_RealSqrt(&real2);
	const real_t sqrt3 = os_RealSqrt(&real3);


	dbg_sprintf(dbgout, "[RightTrig] Synchronizing right triangle...\n");
	geo_RoundTriangle(&s_triangle, g_uRound);

	if (s_trigStatus.A) {
		ui_WriteLabel(Point_Angle_A.label, &s_triangle.A);
	}
	if (s_trigStatus.B) {
		ui_WriteLabel(Point_Angle_B.label, &s_triangle.B);
	}

	/**
	 * Detect if the triangle is a special right triangle by checking its angles
	 */
	if (os_RealCompare(&s_triangle.A, &real45) == 0 || os_RealCompare(&s_triangle.B, &real45) == 0) {
		s_bIs45_45_90 = true;
	}
	if (os_RealCompare(&s_triangle.A, &real30) == 0 || os_RealCompare(&s_triangle.B, &real60) == 0) {
		s_bIs30_60_90 = true;

		dbg_sprintf(dbgout, "[RightTrig] 30-60-90 case: Side a is x, angle A is 30\n");
		/**
		 * 30-60-90 case:
		 * Side a is x
		 * Angle A is 30
		 */
		if (s_trigStatus.a && os_RealCompare(&s_triangle.A, &real30) == 0) {

			local_SetSide(os_RealMul(&s_triangle.a, &sqrt3), SIDE_B);
			local_SetSide(os_RealMul(&s_triangle.a, &real2), SIDE_C);
		}
		else if (s_trigStatus.b && os_RealCompare(&s_triangle.A, &real30) == 0) {

			local_SetSide(os_RealDiv(&s_triangle.b, &sqrt3), SIDE_A);
			local_SetSide(os_RealMul(&s_triangle.a, &real2), SIDE_C);
		}
		else if (s_trigStatus.c && os_RealCompare(&s_triangle.A, &real30) == 0) {

			local_SetSide(os_RealDiv(&s_triangle.c, &real2), SIDE_A);
			local_SetSide(os_RealMul(&s_triangle.a, &sqrt3), SIDE_B);
		}
		//geo_RoundTriangle(&triangle, 1);
	}
	else if (os_RealCompare(&s_triangle.A, &real60) == 0 || os_RealCompare(&s_triangle.B, &real30) == 0) {
		s_bIs30_60_90 = true;

		dbg_sprintf(dbgout, "[RightTrig] 30-60-90 case: Side b is x, angle A is 60\n");
		/**
		 * 30-60-90 case:
		 * Side b is x
		 * Angle A is 60
		 */
		if (s_trigStatus.b && os_RealCompare(&s_triangle.A, &real60) == 0) {

			local_SetSide(os_RealMul(&s_triangle.b, &sqrt3), SIDE_A);
			local_SetSide(os_RealMul(&s_triangle.b, &real2), SIDE_C);
		}
		else if (s_trigStatus.a && os_RealCompare(&s_triangle.A, &real60) == 0) {

			local_SetSide(os_RealDiv(&s_triangle.a, &sqrt3), SIDE_B);
			local_SetSide(os_RealMul(&s_triangle.b, &real2), SIDE_C);
		}
		else if (s_trigStatus.c && os_RealCompare(&s_triangle.A, &real60) == 0) {

			local_SetSide(os_RealDiv(&s_triangle.c, &real2), SIDE_B);
			local_SetSide(os_RealMul(&s_triangle.b, &sqrt3), SIDE_A);
		}

		//geo_RoundTriangle(&triangle, 1);
	}

	/**
	 * Detect if the triangle is a special right triangle by checking its sides
	 *
	 * If it's a 45-45-90, both shorter legs will be congruent (a and b)
	 */
	if ((s_trigStatus.a && s_trigStatus.b) && os_RealCompare(&s_triangle.a, &s_triangle.b) == 0) {
		s_bIs45_45_90 = true;
	}

	if (s_bIs30_60_90) {
		lp_SetLabel(&s_ui_Type, "30-60-90");
		lp_PrintColor(&s_ui_Type, gfx_blue);
		dbg_sprintf(dbgout, "30-60-90 detected\n");
	}
	if (s_bIs45_45_90) {
		lp_SetLabel(&s_ui_Type, "45-45-90");
		lp_PrintColor(&s_ui_Type, gfx_blue);
		dbg_sprintf(dbgout, "45-45-90 detected\n");
	}

	if (s_trigStatus.a) {

		ui_WriteLabel(Point_Side_a.label, &s_triangle.a);
	}
	if (s_trigStatus.a && s_bIs45_45_90) {

		local_SetSide(s_triangle.a, SIDE_B);
		local_SetSide(os_RealMul(&s_triangle.a, &sqrt2), SIDE_C);

	}


	if (s_trigStatus.b) {

		ui_WriteLabel(Point_Side_b.label, &s_triangle.b);
	}
	if (s_trigStatus.b && s_bIs45_45_90) {

		local_SetSide(s_triangle.b, SIDE_A);
		local_SetSide(os_RealMul(&s_triangle.b, &sqrt2), SIDE_C);

		ui_WriteLabel(Point_Side_a.label, &s_triangle.a);
	}


	if (s_trigStatus.c) {

		ui_WriteLabel(Point_Side_c.label, &s_triangle.c);
	}
	if (s_trigStatus.c && s_bIs45_45_90) {


		local_SetSide(os_RealDiv(&s_triangle.c, &sqrt2), SIDE_A);
		local_SetSide(s_triangle.a, SIDE_B);

		ui_WriteLabel(Point_Side_a.label, &s_triangle.a);
		ui_WriteLabel(Point_Side_b.label, &s_triangle.b);
	}


	right_TruncateLabels(g_uDigitThreshold);
	right_Redraw();
	ui_AutoDrawFunctions();
}

static void ui_WriteLabel(char* szLabel, real_t* pReal)
{
	os_RealToStr(szLabel, pReal, 0, 0, 6);
}

static void right_TruncateLabels(int len)
{
	int i = 0;
	dbg_WeakAssert(CountOf(s_rgAngles) == CountOf(s_rgSides));

	for (; i < CountOf(s_rgSides); i++) {
		lib_StrCut(s_rgAngles[i].label, len, kLabelPointLabelSize - len);
		lib_StrCut(s_rgSides[i].label, len, kLabelPointLabelSize - len);
	}
}

// todo: check domain and imaginary numbers
static void pythag_CheckSolvability()
{
	real_t apow, bpow, cpow;
	LocalReal(2);

	if (s_trigStatus.a && s_trigStatus.b && !s_trigStatus.c) {
		//a^2 + b^2 = c^2
		apow = os_RealPow(&s_triangle.a, &real2);
		bpow = os_RealPow(&s_triangle.b, &real2);
		cpow = os_RealAdd(&apow, &bpow);


		local_SetSide(os_RealSqrt(&cpow), SIDE_C);
		return;
	}
	else if (!s_trigStatus.a && s_trigStatus.b && s_trigStatus.c) {
		//a^2 + b^2 = c^2

		bpow = os_RealPow(&s_triangle.b, &real2);
		cpow = os_RealPow(&s_triangle.c, &real2);
		apow = os_RealSub(&cpow, &bpow);

		local_SetSide(os_RealSqrt(&apow), SIDE_A);
		return;
	}
	else if (s_trigStatus.a && !s_trigStatus.b && s_trigStatus.c) {
		//a^2 + b^2 = c^2

		apow = os_RealPow(&s_triangle.a, &real2);
		cpow = os_RealPow(&s_triangle.c, &real2);
		bpow = os_RealSub(&cpow, &apow);

		local_SetSide(os_RealSqrt(&bpow), SIDE_B);

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

	simp_SimplifyRadFromReal(x, rad);
	simp_SimplifyRadFromReal(y, rad2);
	gfx_RadicalFraction(pt, rad[0], rad[1], rad2[0], rad2[1]);
}

static void ui_DrawFunctions_A()
{
	int index = 0;

	for (index = 0; index < 3; index++) {
		ui_ClearFraction(s_rgFuncFracPoints[index]);
		ui_ClearFraction(s_rgFuncFracPoints2[index]);
	}

	// When angle AAA is selected
	lp_SetLabel(&Point_Side_OAH_b, s_lbl_szAdj); // bbb = adj
	lp_SetLabel(&Point_Side_OAH_a, s_lbl_szOpp); // aaa = opp
	lp_SetLabel(&Point_Side_OAH_c, s_lbl_szHyp); // ccc = hyp


	lp_PrintColor(&s_ui_Wait, gfx_red);
	if (!s_ui_bDispSimpRoot) {
		// SOH
		gfx_Fraction(Point_FN_Sin, s_triangle.a, s_triangle.c);

		// CAH
		gfx_Fraction(Point_FN_Cos, s_triangle.b, s_triangle.c);

		// TOA
		gfx_Fraction(Point_FN_Tan, s_triangle.a, s_triangle.b);

		// cot = adj/opp
		gfx_Fraction(Point_FN_Cot, s_triangle.b, s_triangle.a);

		// sec = hyp/adj
		gfx_Fraction(Point_FN_Sec, s_triangle.c, s_triangle.b);

		// csc = hyp/opp
		gfx_Fraction(Point_FN_Csc, s_triangle.c, s_triangle.a);
	}
	else {
		// SOH
		ui_DispFunctionRatioRoot(Point_FN_Sin, s_triangle.a, s_triangle.c);

		// CAH
		ui_DispFunctionRatioRoot(Point_FN_Cos, s_triangle.b, s_triangle.c);

		// TOA
		ui_DispFunctionRatioRoot(Point_FN_Tan, s_triangle.a, s_triangle.b);

		// cot = adj/opp
		ui_DispFunctionRatioRoot(Point_FN_Cot, s_triangle.b, s_triangle.a);

		// sec = hyp/adj
		ui_DispFunctionRatioRoot(Point_FN_Sec, s_triangle.c, s_triangle.b);

		// csc = hyp/opp
		ui_DispFunctionRatioRoot(Point_FN_Csc, s_triangle.c, s_triangle.a);
	}
	lp_Clear(&s_ui_Wait);
	dbg_sprintf(dbgout, "Completed drawing of A ratios\n");

	for (index = 0; index < 3; index++) {
		lp_PrintColor(&s_rgSidesOAH[index], gfx_green);
		lp_PrintColor(&s_rgFuncData[index], gfx_blue);
		lp_PrintColor(&s_rgFuncData2[index], gfx_blue);
	}
	dbg_sprintf(dbgout, "Completed printing of labels for A\n");
}

static void ui_DrawFunctions_B()
{
	int index = 0;

	for (index = 0; index < 3; index++) {
		ui_ClearFraction(s_rgFuncFracPoints[index]);
		ui_ClearFraction(s_rgFuncFracPoints2[index]);
	}

	// When angle BBB is selected
	lp_SetLabel(&Point_Side_OAH_b, s_lbl_szOpp); // bbb = opp
	lp_SetLabel(&Point_Side_OAH_a, s_lbl_szAdj); // aaa = adj
	lp_SetLabel(&Point_Side_OAH_c, s_lbl_szHyp); // ccc = hyp

	lp_PrintColor(&s_ui_Wait, gfx_red);
	if (!s_ui_bDispSimpRoot) {
		// SOH
		gfx_Fraction(Point_FN_Sin, s_triangle.b, s_triangle.c);

		// CAH
		gfx_Fraction(Point_FN_Cos, s_triangle.a, s_triangle.c);

		// TOA
		gfx_Fraction(Point_FN_Tan, s_triangle.b, s_triangle.a);

		// cot = adj/opp
		gfx_Fraction(Point_FN_Cot, s_triangle.a, s_triangle.b);

		// sec = hyp/adj
		gfx_Fraction(Point_FN_Sec, s_triangle.c, s_triangle.a);

		// csc = hyp/opp
		gfx_Fraction(Point_FN_Csc, s_triangle.c, s_triangle.b);
	}
	else {
		// SOH
		ui_DispFunctionRatioRoot(Point_FN_Sin, s_triangle.b, s_triangle.c);

		// CAH
		ui_DispFunctionRatioRoot(Point_FN_Cos, s_triangle.a, s_triangle.c);

		// TOA
		ui_DispFunctionRatioRoot(Point_FN_Tan, s_triangle.b, s_triangle.a);

		// cot = adj/opp
		ui_DispFunctionRatioRoot(Point_FN_Cot, s_triangle.a, s_triangle.b);

		// sec = hyp/adj
		ui_DispFunctionRatioRoot(Point_FN_Sec, s_triangle.c, s_triangle.a);

		// csc = hyp/opp
		ui_DispFunctionRatioRoot(Point_FN_Csc, s_triangle.c, s_triangle.b);
	}
	lp_Clear(&s_ui_Wait);
	dbg_sprintf(dbgout, "Completed drawing of B ratios\n");

	for (index = 0; index < 3; index++) {
		lp_PrintColor(&s_rgSidesOAH[index], gfx_green);
		lp_PrintColor(&s_rgFuncData[index], gfx_blue);
		lp_PrintColor(&s_rgFuncData2[index], gfx_blue);
	}
	dbg_sprintf(dbgout, "Completed printing of labels for B\n");
}

static void right_Redraw()
{
	int index = 0;

	for (; index < 3; index++) {
		lp_Clear(&s_rgAngles[index]);
		lp_Print(&s_rgAngles[index]);

		lp_Clear(&s_rgSides[index]);
		lp_Print(&s_rgSides[index]);
	}

	right_DrawSides();
}

static void ui_DrawFunctions_90()
{
	int index = 0;
	// Draw nothing when 90 is selected
	lp_SetLabel(&Point_Side_OAH_b, "B"); // bbb
	lp_SetLabel(&Point_Side_OAH_a, "A"); // aaa
	lp_SetLabel(&Point_Side_OAH_c, "C"); // ccc

	for (index = 0; index < 3; index++) {
		lp_Clear(&s_rgSidesOAH[index]);
		lp_Clear(&s_rgFuncData[index]);
		lp_Clear(&s_rgFuncData2[index]);
		ui_ClearFraction(s_rgFuncFracPoints[index]);
		ui_ClearFraction(s_rgFuncFracPoints2[index]);
	}
}

static void ui_DispSimplified(real_t* r)
{
	int24_t rad[2];

	if (!s_ui_bDispSimpRoot) {
		return;
	}
	lp_PrintColor(&s_ui_Wait, gfx_red);
	ui_ClearDispSimpRoot();

	dbg_sprintf(dbgout, "[RightTrig] Simplifying radical...\n");
	*r = os_RealRound(r, g_uRound);
	simp_SimplifyRadFromReal(*r, rad);

	ui_ClearDispSimpRoot();

	gfx_Sqrt(s_ui_RootPoint, rad[0], rad[1]);
	lp_Clear(&s_ui_Wait);
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
	if (s_ui_bDispSimpRoot) {
		ui_ClearDispSimpRoot();
		s_ui_bDispSimpRoot = false;
		lp_PrintColor(&s_ui_btnRoot, gfx_red);
	}
	else if (!s_ui_bDispSimpRoot) {
		s_ui_bDispSimpRoot = true;
		lp_PrintColor(&s_ui_btnRoot, gfx_green);
		buf = os_StrToReal(s_pCurrentSelection->label, NULL);
		ui_DispSimplified(&buf);
	}

	ui_AutoDrawFunctions();
}

static void right_Reset()
{
	int i = 0;
	bool  * bptr = (bool*) &s_trigStatus;
	real_t* rptr = (real_t*) &s_triangle;

	for (; i < sizeof(s_trigStatus) / sizeof(bool); i++) {
		bptr[i] = false;
	}
	for (i = 0; i < sizeof(s_triangle) / sizeof(real_t); i++) {
		rptr[i] = os_Int24ToReal(0);
	}

	ui_InitLabels();

	s_bIs45_45_90 = false;
	s_bIs30_60_90 = false;

	right_Redraw();
	right_Sync();
	ui_ClearDispSimpRoot();
	lp_Clear(&s_ui_Type);
	dbg_sprintf(dbgout, "[RightTrig] Reset triangle\n");
}

static void ui_SwapFocus(labelpoint_t* from, labelpoint_t* to, real_t* disp)
{
	lp_SetFocus(&s_pCurrentSelection, from, to);
	ui_DispSimplified(disp);
}

static void right_SelectSide()
{
	uint8_t key;

	s_pCurrentSelection = &Point_Side_b;
	lp_Highlight(&Point_Side_b);
	ui_DrawFunctions_90();
	ui_DispSimplified(&s_triangle.b);

	RECURSE:
	while ((key = os_GetCSC()) != sk_Enter) {
		if (key == sk_Clear) {
			return;
		}

		if (key == sk_Zoom) {
			ui_SwitchDispSimpRoot();
		}

		if (key == sk_Window) {
			ui_SwitchRounding();
		}

		if (key == sk_Yequ) {
			ui_SetRounding();
		}

		if (key == sk_Trace) {
			right_Reset();
			lp_SetFocus(&s_pCurrentSelection, s_pCurrentSelection, &Point_Side_a);
		}

		if (key == sk_Graph) {
			lp_Clear(&s_ui_Mode);

			lp_SetLabel(&s_ui_Mode, s_lbl_szAngleMode);
			lp_Print(&s_ui_Mode);
			lp_ClearHighlight(s_pCurrentSelection);
			ui_ClearDispSimpRoot();
			right_SelectAngle();
			return;
		}

		/* bbb -> ccc */
		if (key == sk_Right && local_SelectionEquals(Point_Side_b)) {
			ui_SwapFocus(&Point_Side_b, &Point_Side_c, &s_triangle.c);
		}

		/* bbb -> aaa */
		if (key == sk_Down && local_SelectionEquals(Point_Side_b)) {
			ui_SwapFocus(&Point_Side_b, &Point_Side_a, &s_triangle.a);
		}

		/* ccc -> aaa */
		if (key == sk_Down && local_SelectionEquals(Point_Side_c)) {
			ui_SwapFocus(&Point_Side_c, &Point_Side_a, &s_triangle.a);
		}

		/* ccc -> bbb */
		if (key == sk_Left && local_SelectionEquals(Point_Side_c)) {
			ui_SwapFocus(&Point_Side_c, &Point_Side_b, &s_triangle.b);
		}

		/* aaa -> ccc */
		if (key == sk_Up || key == sk_Right && local_SelectionEquals(Point_Side_a)) {
			ui_SwapFocus(&Point_Side_a, &Point_Side_c, &s_triangle.c);
		}

		/* aaa -> bbb */
		if (key == sk_Left && local_SelectionEquals(Point_Side_a)) {
			ui_SwapFocus(&Point_Side_a, &Point_Side_b, &s_triangle.b);
		}
	}

	if (local_SelectionEquals(Point_Side_b)) {
		dbg_sprintf(dbgout, "[RightTrig] User selected side %s\n", Point_Side_b.label);

		local_SetSide(io_gfx_ReadReal(&Point_Side_b), SIDE_B);
		ui_DispSimplified(&s_triangle.b);
	}

	if (local_SelectionEquals(Point_Side_a)) {
		dbg_sprintf(dbgout, "[RightTrig] User selected side %s\n", Point_Side_a.label);

		local_SetSide(io_gfx_ReadReal(&Point_Side_a), SIDE_A);
		ui_DispSimplified(&s_triangle.a);
	}

	if (local_SelectionEquals(Point_Side_c)) {
		dbg_sprintf(dbgout, "[RightTrig] User selected side %s\n", Point_Side_c.label);

		local_SetSide(io_gfx_ReadReal(&Point_Side_c), SIDE_C);
		ui_DispSimplified(&s_triangle.c);
	}

	pythag_CheckSolvability();
	right_Sync();
	lp_Highlight(s_pCurrentSelection);

	goto RECURSE;
}

static void ui_SwitchRounding()
{

	if (g_bEnableRounding) {
		lp_PrintColor(&s_ui_btnRound, gfx_red);
		g_bEnableRounding = false;
	}
	else if (!g_bEnableRounding) {
		lp_PrintColor(&s_ui_btnRound, gfx_green);
		g_bEnableRounding = true;
	}
}

static bool local_SelectionEquals(labelpoint_t p)
{
	return lp_Equal(*s_pCurrentSelection, p);
}


static void right_SelectAngle()
{
	uint8_t key;

	LocalReal(90);

	s_pCurrentSelection = &Point_Angle_C;
	lp_Highlight(&Point_Angle_C);
	ui_DrawFunctions_90();
	RECURSE:
	while ((key = os_GetCSC()) != sk_Enter) {
		if (key == sk_Clear) {
			return;
		}

		if (key == sk_Zoom) {
			ui_SwitchDispSimpRoot();
		}

		if (key == sk_Window) {
			ui_SwitchRounding();
		}
		if (key == sk_Yequ) {
			ui_SetRounding();
		}

		if (key == sk_Trace) {
			right_Reset();
			lp_SetFocus(&s_pCurrentSelection, s_pCurrentSelection, &Point_Angle_C);
			ui_DrawFunctions_90();
		}

		if (key == sk_Graph) {
			lp_Clear(&s_ui_Mode);
			lp_SetLabel(&s_ui_Mode, s_lbl_szSideMode);
			lp_Print(&s_ui_Mode);
			lp_ClearHighlight(s_pCurrentSelection);
			ui_ClearDispSimpRoot();
			right_SelectSide();
			return;
		}

		/* 90 -> BBB */
		if (key == sk_Right && local_SelectionEquals(Point_Angle_C)) {
			lp_SetFocus(&s_pCurrentSelection, &Point_Angle_C, &Point_Angle_B);

			ui_DrawFunctions_B();
		}

		/* 90 -> AAA */
		if (key == sk_Up && local_SelectionEquals(Point_Angle_C)) {
			lp_SetFocus(&s_pCurrentSelection, &Point_Angle_C, &Point_Angle_A);

			ui_DrawFunctions_A();
		}

		/* AAA -> 90 */
		if (key == sk_Down && local_SelectionEquals(Point_Angle_A)) {
			lp_SetFocus(&s_pCurrentSelection, &Point_Angle_A, &Point_Angle_C);

			ui_DrawFunctions_90();
		}

		/* AAA -> BBB */
		if (key == sk_Right && local_SelectionEquals(Point_Angle_A)) {
			lp_SetFocus(&s_pCurrentSelection, &Point_Angle_A, &Point_Angle_B);

			ui_DrawFunctions_B();
		}

		/* BBB -> 90 */
		if (key == sk_Left && local_SelectionEquals(Point_Angle_B)) {
			lp_SetFocus(&s_pCurrentSelection, &Point_Angle_B, &Point_Angle_C);

			ui_DrawFunctions_90();
		}

		/* BBB -> AAA */
		if (key == sk_Up && local_SelectionEquals(Point_Angle_B)) {
			lp_SetFocus(&s_pCurrentSelection, &Point_Angle_B, &Point_Angle_A);

			ui_DrawFunctions_A();
		}
	}

	if (local_SelectionEquals(Point_Angle_C)) {
		dbg_sprintf(dbgout, "[RightTrig] User selected angle 90, returning to angle selection\n");
		goto RECURSE;
	}

	if (local_SelectionEquals(Point_Angle_B)) {
		dbg_sprintf(dbgout, "[RightTrig] User selected angle %s\n", Point_Angle_B.label);

		local_SetAngle(io_gfx_ReadReal(&Point_Angle_B), ANGLE_B);
	}

	if (local_SelectionEquals(Point_Angle_A)) {
		dbg_sprintf(dbgout, "[RightTrig] User selected angle %s\n", Point_Angle_A.label);

		local_SetAngle(io_gfx_ReadReal(&Point_Angle_A), ANGLE_A);
	}

	if (s_trigStatus.A) {

		local_SetAngle(os_RealSub(&real90, &s_triangle.A), ANGLE_B);
		lp_Clear(&Point_Angle_B);
	}
	else if (s_trigStatus.B) {
		local_SetAngle(os_RealSub(&real90, &s_triangle.B), ANGLE_A);
		lp_Clear(&Point_Angle_A);
	}

	right_Sync();
	lp_Highlight(s_pCurrentSelection);
	goto RECURSE;
}

static void right_DrawSides()
{
	/* Leg a */
	gfx_Line(s_rgVerts[0], s_rgVerts[1], s_rgVerts[2], s_rgVerts[3]);

	/* Leg b */
	gfx_Line(s_rgVerts[2], s_rgVerts[3], s_rgVerts[4], s_rgVerts[5]);

	/* Hypotenuse */
	gfx_Line(s_rgVerts[4], s_rgVerts[5], s_rgVerts[0], s_rgVerts[1]);
}

void right_SolveTriangle()
{
	int index;

	s_triangle.C   = os_Int24ToReal(90);
	s_trigStatus.C = true;
	s_ui_bDispSimpRoot = true;
	g_bEnableRounding  = true;

	ui_InitLabels();
	gfx_Begin();
	gfx_SetColor(gfx_blue);
	gfx_SetTextFGColor(gfx_black);

	right_DrawSides();

	// Leg b, a, c
	for (index = 0; index < 3; index++) {
		lp_Print(&s_rgAngles[index]);
		lp_Print(&s_rgSides[index]);
	}

	lp_Print(&s_ui_btnClear);
	lp_Print(&s_ui_btnMode);
	lp_Print(&s_ui_btnRoundPlaces);
	lp_PrintColor(&s_ui_btnRoot, gfx_green);
	lp_Print(&s_ui_Mode);
	lp_PrintColor(&s_ui_btnRound, gfx_green);

	right_SelectAngle();

	gfx_ZeroScreen();
	gfx_End();

}
