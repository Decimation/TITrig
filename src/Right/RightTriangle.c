//
// Created by Decimation on 4/14/2018.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-misplaced-array-index"

#include <debug.h>
#include "RightTriangle.h"
#include "../System.h"
#include "../Simplifiers.h"
#include "../Types.h"
#include "RightOperability.h"
#include "RightDrawFunctions.h"
#include "../LabelPoint/LabelPoint.h"
#include "../LabelPoint/DataPoint.h"


labelpoint_t g_rXAngles[3] = {
		{{30 + 5,   129 - 10}, "90"},
		{{30 + 120, 129 - 10}, "B"},
		{{37,       27},       "A"}
};

labelpoint_t g_rXSides[3] = {
		{{30 - 25, 129 - 50}, "b"},
		{{30 + 50, 129 + 10}, "a"},
		{{30 + 90, 129 - 70}, "c"}
};

labelpoint_t g_rXSides_OAH[3] = {
		{{30 - 25, 129 - 40}, "..."},
		{{30 + 50, 129 + 20}, "..."},
		{{30 + 90, 129 - 60}, "..."}
};


static const labelpoint_t r_ui_btn_Mode           = {280, 230, "Mode"};
static const labelpoint_t r_ui_btn_Clear          = {215, 230, "Clear"};
static const labelpoint_t r_ui_btn_EnableSimpRoot = {150, 230, "Root"};
static const labelpoint_t r_ui_btn_EnableRounding = {85, 230, "Round"};
static const labelpoint_t r_ui_btn_RoundPlaces    = {20, 230, "0"};

bool                     ui_dispSimpRoot;
labelpoint_t             ui_Wait                  = {{230, 40}, "Processing..."};
static labelpoint_t      ui_Mode                  = {{230, 10}, "ANGLE MODE"};
static const gfx_point_t ui_RootPoint             = {230, 25};
static labelpoint_t      ui_Type                  = {{230, 70}, "..."};

static const char lbl_AngleMode[] = "ANGLE MODE";
static const char lbl_SideMode[]  = "SIDE MODE";
const char        lbl_ADJ[]       = "ADJ";
const char        lbl_HYP[]       = "HYP";
const char        lbl_OPP[]       = "OPP";

dynamictriangle_t g_rAutoTriangle;

static bool is45_45_90;
static bool is30_60_90;

static const char ui_Theta = (char) 91;

static labelpoint_t* r_currentSelection;


/**
 * Triangle coordinates
 */
const int r_verts[6] = {
		30, 10,   /* (x0, y0) */
		30, 129,  /* (x1, y1) */
		189, 129, /* (x2, y2) */
};

const datapoint_t r_func[3] = {
		{{10,  175, "sin = "}, {10 + 35,  175}},
		{{110, 175, "cos = "}, {110 + 35, 175}},
		{{210, 175, "tan = "}, {210 + 35, 175}},
};


const datapoint_t r_func2[3] = {
		{{10,  210, "csc = "}, {10 + 35,  210}},
		{{110, 210, "sec = "}, {110 + 35, 210}},
		{{210, 210, "cot = "}, {210 + 35, 210}},
};


static void ui_AutoDrawFunctions()
{
	if (right_CurrentSelectionEq(r_angle_A)) {
		ui_DrawFunctions_A();
	}
	else if (right_CurrentSelectionEq(r_angle_B)) {
		ui_DrawFunctions_B();
	}
	else if (right_CurrentSelectionEq(r_angle_C)) {
		ui_DrawFunctions_90();
	}
}


static void ui_SetRounding()
{
	// Clamp
	if (g_round == g_digitThreshold) {
		g_round = 0;
	}
	else if (g_round <= g_digitThreshold) {
		g_round++;
	}

	lp_Clear(&r_ui_btn_RoundPlaces);
	sprintf(r_ui_btn_RoundPlaces.label, "%d", g_round);
	lp_Print(&r_ui_btn_RoundPlaces);

}


static void right_Sync()
{
	LocalReal(45);
	LocalReal(60);
	LocalReal(30);
	LocalReal(2);
	LocalReal(3);

	const real_t sqrt2  = os_RealSqrt(&real2);
	const real_t sqrt3  = os_RealSqrt(&real3);
	gfx_point_t  origin = {0, 0};


	dbg_sprintf(dbgout, "[RightTrig] Synchronizing right triangle...\n");
	tri_Round(&g_rAutoTriangle, g_round);

	/**
	 * Detect if the triangle is a special right triangle by checking its angles
	 */
	if (right_AngleEq(ANGLE_A, real45) || right_AngleEq(ANGLE_B, real45)) {
		is45_45_90 = true;
	}
	if (right_AngleEq(ANGLE_A, real30) || right_AngleEq(ANGLE_B, real60)) {
		is30_60_90 = true;

		dbg_sprintf(dbgout, "[RightTrig] 30-60-90 case: Side a is x, angle A is 30\n");

		/**
		 * 30-60-90 case:
		 * Side a is x
		 * Angle A is 30
		 */
		if (available_a && right_AngleEq(ANGLE_A, real30)) {
			right_SetSide(SIDE_b, right_MulSide(SIDE_a, sqrt3));
			right_SetSide(SIDE_c, right_MulSide(SIDE_a, real2));
		}
		else if (available_b && right_AngleEq(ANGLE_A, real30)) {
			right_SetSide(SIDE_a, right_DivSide(SIDE_b, sqrt3));
			right_SetSide(SIDE_b, right_MulSide(SIDE_a, real2));
		}
		else if (available_c && right_AngleEq(ANGLE_A, real30)) {
			right_SetSide(SIDE_a, right_DivSide(SIDE_c, real2));
			right_SetSide(SIDE_b, right_MulSide(SIDE_a, sqrt3));
		}
	}
	else if (right_AngleEq(ANGLE_A, real60) || right_AngleEq(ANGLE_B, real30)) {
		is30_60_90 = true;

		dbg_sprintf(dbgout, "[RightTrig] 30-60-90 case: Side b is x, angle A is 60\n");

		/**
		 * 30-60-90 case:
		 * Side b is x
		 * Angle A is 60
		 */
		if (available_b && right_AngleEq(ANGLE_A, real60)) {
			right_SetSide(SIDE_a, right_MulSide(SIDE_b, sqrt3));
			right_SetSide(SIDE_c, right_MulSide(SIDE_b, real2));
		}
		else if (available_a && right_AngleEq(ANGLE_A, real60)) {
			right_SetSide(SIDE_b, right_DivSide(SIDE_a, sqrt3));
			right_SetSide(SIDE_c, right_MulSide(SIDE_b, real2));
		}
		else if (available_c && right_AngleEq(ANGLE_A, real60)) {
			right_SetSide(SIDE_b, right_DivSide(SIDE_c, real2));
			right_SetSide(SIDE_a, right_MulSide(SIDE_b, sqrt3));
		}

	}

	/**
	 * Detect if the triangle is a special right triangle by checking its sides
	 *
	 * If it's a 45-45-90, both shorter legs will be congruent (a and b)
	 */
	if ((available_a && available_b) && right_SidesEq(SIDE_a, SIDE_b)) {
		is45_45_90 = true;
	}

	if (is30_60_90) {
		lp_SetLabel(&ui_Type, "30-60-90");
		lp_PrintColor(&ui_Type, gfx_blue);
		dbg_sprintf(dbgout, "30-60-90 detected\n");
	}
	if (is45_45_90) {
		lp_SetLabel(&ui_Type, "45-45-90");
		lp_PrintColor(&ui_Type, gfx_blue);
		dbg_sprintf(dbgout, "45-45-90 detected\n");
	}

	if (available_a && is45_45_90) {
		right_SetSide(SIDE_b, right_GetSide(SIDE_a));
		right_SetSide(SIDE_c, right_MulSide(SIDE_a, sqrt2));
	}

	if (available_b && is45_45_90) {
		right_SetSide(SIDE_a, right_GetSide(SIDE_b));
		right_SetSide(SIDE_c, right_MulSide(SIDE_b, sqrt2));
	}

	if (available_c && is45_45_90) {
		right_SetSide(SIDE_a, right_DivSide(SIDE_c, sqrt2));
		right_SetSide(SIDE_b, right_GetSide(SIDE_a));
	}


	// 0, 0 -> 210, 155
	gfx_ClearArea(origin, 210, 155);

	right_UpdateBuffers();
	right_TruncateLabels(g_digitThreshold);
	right_Redraw();
	ui_AutoDrawFunctions();
}


static void right_TruncateLabels(int len)
{
	int i = 0;
	for (; i < CountOf(g_rXSides); i++) {
		sys_StrCut(g_rXAngles[i].label, len, kLabelPointLabelSize - len);
		sys_StrCut(g_rXSides[i].label, len, kLabelPointLabelSize - len);
	}
}

// todo: check domain and imaginary numbers
static void pythag_CheckSolvability()
{
	real_t apow, bpow, cpow;

	LocalReal(2);

	if (available_a && available_b && !available_c) {
		//a^2 + b^2 = c^2
		//apow = os_RealPow(&triangle.a, &real2);
		//bpow = os_RealPow(&triangle.b, &real2);
		apow = right_SquareSide(SIDE_a);
		bpow = right_SquareSide(SIDE_b);
		cpow = os_RealAdd(&apow, &bpow);
		right_SetSide(SIDE_c, os_RealSqrt(&cpow));
		return;
	}
	else if (!available_a && available_b && available_b) {
		//a^2 + b^2 = c^2

		bpow = right_SquareSide(SIDE_b);
		cpow = right_SquareSide(SIDE_c);
		apow = os_RealSub(&cpow, &bpow);
		right_SetSide(SIDE_a, os_RealSqrt(&apow));
		return;
	}
	else if (available_a && !available_b && available_c) {
		//a^2 + b^2 = c^2

		apow = right_SquareSide(SIDE_a);
		cpow = right_SquareSide(SIDE_c);
		bpow = os_RealSub(&cpow, &apow);
		right_SetSide(SIDE_b, os_RealSqrt(&bpow));
		return;
	}
}


void right_DrawFraction(gfx_point_t pt, side_t num, side_t denom)
{
	dbg_sprintf(dbgout, "right::DrawFraction\n");
	gfx_Fraction(pt, *right_GetSideValuePtr(num), *right_GetSideValuePtr(denom));
}

void right_DispFunctionRatioRoot(gfx_point_t pt, side_t num, side_t denom)
{
	gfx_DispFunctionRatioRoot(pt, *right_GetSideValuePtr(num), *right_GetSideValuePtr(denom));
}


static void dbgSide(char* lbl, side_t side)
{
	char sideBuf[20];
	sys_WriteBuffer(sideBuf, right_GetSideValuePtr(side));
	dbg_sprintf(dbgout, "[%s] Side: %s\n", lbl, sideBuf);
}

static void dbgPt(char* lbl, labelpoint_t pt)
{
	dbg_sprintf(dbgout, "[%s] Point: %s\n", lbl, pt.label);
}

static void dbgAngle(char* lbl, angle_t angle)
{
	char angleBuf[20];
	sys_WriteBuffer(angleBuf, right_GetAngleValuePtr(angle));
	dbg_sprintf(dbgout, "[%s] Angle: %s\n", lbl, angleBuf);
}

void right_dbg()
{
	dbgSide("a", SIDE_a);
	dbgSide("b", SIDE_b);
	dbgSide("c", SIDE_c);

	dbgAngle("A", ANGLE_A);
	dbgAngle("B", ANGLE_B);
	dbgAngle("C", ANGLE_C90);

	dbgPt("side_a", r_side_a);
	dbgPt("side_b", r_side_b);
	dbgPt("side_c", r_side_c);

	dbgPt("angle_A", r_angle_A);
	dbgPt("angle_B", r_angle_B);
	dbgPt("angle_C", r_angle_C);
}

static void right_Redraw()
{
	int index = 0;

	for (; index < CountOf(g_rXAngles); index++) {
		lp_ClrPrint(&g_rXAngles[index]);
		lp_ClrPrint(&g_rXSides[index]);
	}

	right_DrawSides();
}

void ui_DispSimplified(real_t* r)
{
	int24_t rad[2];

	if (!ui_dispSimpRoot) {
		return;
	}
	lp_PrintColor(&ui_Wait, gfx_red);
	ui_ClearDispSimpRoot();

	dbg_sprintf(dbgout, "[RightTrig] Simplifying radical...\n");
	*r = os_RealRound(r, g_round);
	sp_SimplifyRadicalFromDecimal(*r, rad);

	ui_ClearDispSimpRoot();

	gfx_Sqrt(ui_RootPoint, rad[0], rad[1]);
	lp_Clear(&ui_Wait);
}


static void ui_ClearDispSimpRoot()
{
	static const gfx_point_t point = {230, 20};
	gfx_ClearArea(point, 70, 35);
}

/**
 * Toggles displaying of simplified root form
 */
static void ui_SwitchDispSimpRoot()
{
	real_t buf;
	if (ui_dispSimpRoot) {
		ui_ClearDispSimpRoot();
		ui_dispSimpRoot = false;
		lp_PrintColor(&r_ui_btn_EnableSimpRoot, gfx_red);
	}
	else if (!ui_dispSimpRoot) {
		ui_dispSimpRoot = true;
		lp_PrintColor(&r_ui_btn_EnableSimpRoot, gfx_green);
		buf = os_StrToReal(r_currentSelection->label, NULL);
		ui_DispSimplified(&buf);
	}

	ui_AutoDrawFunctions();
}

static void right_Reset()
{
	tri_Reset(&g_rAutoTriangle);

	lp_SetLabel(&r_side_a, "a");
	lp_SetLabel(&r_side_b, "b");
	lp_SetLabel(&r_side_c, "c");

	lp_SetLabel(&r_angle_A, "A");
	lp_SetLabel(&r_angle_B, "B");

	lp_SetLabel(&ui_Type, "...");

	is45_45_90 = false;
	is30_60_90 = false;

	right_Redraw();
	right_Sync();
	ui_ClearDispSimpRoot();
	lp_Clear(&ui_Type);
	dbg_sprintf(dbgout, "[RightTrig] Reset triangle\n");
}


static void right_SelectSide()
{
	uint8_t key;

	r_currentSelection = &r_side_b;
	lp_HighlightPoint(&r_side_b);
	ui_DrawFunctions_90();
	right_DispSimplified(SIDE_b);
	RECURSE:
	while ((key = os_GetCSC()) != sk_Enter) {
		io_HandleKeyInput(key)

		if (key == sk_Trace) {
			right_Reset();
			lp_SetFocus(&r_currentSelection, r_currentSelection, &r_side_a);
		}

		if (key == sk_Graph) {
			lp_Clear(&ui_Mode);

			lp_SetLabel(&ui_Mode, lbl_AngleMode);
			lp_Print(&ui_Mode);
			lp_ClearHighlight(r_currentSelection);
			ui_ClearDispSimpRoot();
			right_SelectAngle();
			return;
		}

		/* bbb -> ccc */
		io_HandleSideSelection(key, sk_Right, r_side_b, r_side_c, SIDE_c)

		/* bbb -> aaa */
		io_HandleSideSelection(key, sk_Down, r_side_b, r_side_a, SIDE_a)

		/* ccc -> aaa */
		io_HandleSideSelection(key, sk_Down, r_side_c, r_side_a, SIDE_a)

		/* ccc -> bbb */
		io_HandleSideSelection(key, sk_Left, r_side_c, r_side_b, SIDE_b)


		/* aaa -> ccc */
		if (key == sk_Up || key == sk_Right && right_CurrentSelectionEq(r_side_a)) {
			lp_SetFocus(&r_currentSelection, &r_side_a, &r_side_c);
			right_DispSimplified(SIDE_c);
		}

		/* aaa -> bbb */
		io_HandleSideSelection(key, sk_Left, r_side_a, r_side_b, SIDE_b)
	}

	ui_IteratePossibleSelections();

	pythag_CheckSolvability();
	right_Sync();
	lp_HighlightPoint(r_currentSelection);

	goto RECURSE;
}

static void ui_IteratePossibleSelections()
{
	int i;
	for (i = 0; i < CountOf(g_rXSides); ++i) {
		if (right_CurrentSelectionEq(g_rXSides[i])) {
			dbg_sprintf(dbgout, "[RightTrig] User selected side %s\n", g_rXSides[i].label);
			right_SetAndDispSide(right_PointToSide(g_rXSides[i]));
		}
	}
}


static void ui_SwitchRounding()
{
	if (g_enableRounding) {
		lp_PrintColor(&r_ui_btn_EnableRounding, gfx_red);
		g_enableRounding = false;
	}
	else if (!g_enableRounding) {
		lp_PrintColor(&r_ui_btn_EnableRounding, gfx_green);
		g_enableRounding = true;
	}
}

static void right_SelectAngle()
{
	uint8_t key;


	LocalReal(90);
	r_currentSelection = &r_angle_C;
	lp_HighlightPoint(&r_angle_C);
	ui_DrawFunctions_90();
	RECURSE:
	while ((key = os_GetCSC()) != sk_Enter) {
		io_HandleKeyInput(key)

		if (key == sk_Trace) {
			right_Reset();
			lp_SetFocus(&r_currentSelection, r_currentSelection, &r_angle_C);
			ui_DrawFunctions_90();
		}

		if (key == sk_Graph) {
			lp_Clear(&ui_Mode);
			lp_SetLabel(&ui_Mode, lbl_SideMode);
			lp_Print(&ui_Mode);
			lp_ClearHighlight(r_currentSelection);
			ui_ClearDispSimpRoot();
			right_SelectSide();
			return;
		}

		/* 90 -> BBB */
		io_HandleAngleSelection(key, sk_Right, r_angle_C, r_angle_B, ui_DrawFunctions_B);

		/* 90 -> AAA */
		io_HandleAngleSelection(key, sk_Up, r_angle_C, r_angle_A, ui_DrawFunctions_A);

		/* AAA -> 90 */
		io_HandleAngleSelection(key, sk_Down, r_angle_A, r_angle_C, ui_DrawFunctions_90);

		/* AAA -> BBB */
		io_HandleAngleSelection(key, sk_Right, r_angle_A, r_angle_B, ui_DrawFunctions_B);

		/* BBB -> 90 */
		io_HandleAngleSelection(key, sk_Left, r_angle_B, r_angle_C, ui_DrawFunctions_90);

		/* BBB -> AAA */
		io_HandleAngleSelection(key, sk_Up, r_angle_B, r_angle_A, ui_DrawFunctions_A);
	}

	if (right_CurrentSelectionEq(r_angle_C)) {
		dbg_sprintf(dbgout, "[RightTrig] User selected angle 90, returning to angle selection\n");
		goto RECURSE;
	}

	if (right_CurrentSelectionEq(r_angle_B)) {
		dbg_sprintf(dbgout, "[RightTrig] User selected angle %s\n", r_angle_B.label);


		right_SetAngle(ANGLE_B, io_gfx_ReadReal(&r_angle_B));
	}

	if (right_CurrentSelectionEq(r_angle_A)) {
		dbg_sprintf(dbgout, "[RightTrig] User selected angle %s\n", r_angle_A.label);


		right_SetAngle(ANGLE_A, io_gfx_ReadReal(&r_angle_A));
	}

	if (available_A) {

		right_SetAngle(ANGLE_B, os_RealSub(&real90, right_GetAngleValuePtr(ANGLE_A)));
		lp_Clear(&r_angle_B);
	}
	else if (available_B) {

		right_SetAngle(ANGLE_A, os_RealSub(&real90, right_GetAngleValuePtr(ANGLE_B)));
		lp_Clear(&r_angle_A);
	}

	right_Sync();
	lp_HighlightPoint(r_currentSelection);
	goto RECURSE;
}

static bool right_CurrentSelectionEq(labelpoint_t pt)
{
	return lp_Equals(*r_currentSelection, pt);
}

static void right_DrawSides()
{
	/* Leg a */
	gfx_Line(r_verts[0], r_verts[1], r_verts[2], r_verts[3]);

	/* Leg b */
	gfx_Line(r_verts[2], r_verts[3], r_verts[4], r_verts[5]);

	/* Hypotenuse */
	gfx_Line(r_verts[4], r_verts[5], r_verts[0], r_verts[1]);
}


/**
 * >> Entry point
 */
void right_SolveTriangle()
{
	int index;
	//tri_Reset(&autotriangle);



	ui_dispSimpRoot  = true;
	g_enableRounding = true;


	gfx_Begin();
	gfx_SetColor(gfx_blue);
	gfx_SetTextFGColor(gfx_black);

	right_DrawSides();


	// Leg b, a, c
	for (index = 0; index < CountOf(g_rXAngles); index++) {
		lp_Print(&g_rXAngles[index]);
		lp_Print(&g_rXSides[index]);
	}

	lp_Print(&r_ui_btn_Clear);
	lp_Print(&r_ui_btn_Mode);
	lp_Print(&r_ui_btn_RoundPlaces);
	lp_PrintColor(&r_ui_btn_EnableSimpRoot, gfx_green);
	lp_Print(&ui_Mode);
	lp_PrintColor(&r_ui_btn_EnableRounding, gfx_green);

	right_SelectAngle();

	gfx_ZeroScreen();
	gfx_End();

}

#pragma clang diagnostic pop