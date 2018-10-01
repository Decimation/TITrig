//
// Created by Decimation on 4/28/2018.
//

#ifndef TITRIG_RIGHTTRIANGLE_H
#define TITRIG_RIGHTTRIANGLE_H

#include <tice.h>
#include <graphx.h>
#include "../GraphicsExt.h"
#include "../DynamicTriangle/DynamicTriangle.h"
#include "../LabelPoint/DataPoint.h"


#define fnCotPt r_func2[2].data
#define fnSecPt r_func2[1].data
#define fnCscPt r_func2[0].data

#define fnSinPt r_func[0].data
#define fnCosPt r_func[1].data
#define fnTanPt r_func[2].data

#define r_side_a g_rXSides[1]
#define r_side_b g_rXSides[0]
#define r_side_c g_rXSides[2]

#define r_angle_A g_rXAngles[2]
#define r_angle_B g_rXAngles[1]
#define r_angle_C g_rXAngles[0]

extern const char        lbl_ADJ[];
extern const char        lbl_HYP[];
extern const char        lbl_OPP[];
extern const datapoint_t r_func2[3];
extern const datapoint_t r_func[3];
extern dynamictriangle_t g_rAutoTriangle;
extern labelpoint_t      ui_Wait;
extern labelpoint_t      g_rXSides[3];
extern labelpoint_t      g_rXSides_OAH[3];
extern labelpoint_t      g_rXAngles[3];
extern bool              ui_dispSimpRoot;

// @formatter:off

#define io_HandleKeyInput(Key)          		\
	if (Key == sk_Clear) {              		\
		return;                         		\
	}                                   		\
	if (Key == sk_Zoom) {               		\
		ui_SwitchDispSimpRoot();        		\
	}                                   		\
	if (Key == sk_Window) {             		\
		ui_SwitchRounding();            		\
	}                                   		\
	if (Key == sk_Yequ) {               		\
		ui_SetRounding();               		\
	}                                   		\
	if (Key == sk_Del) {                		\
		tri_Round(&g_rAutoTriangle, g_round);   \
	}                                       	\
	if (Key == sk_Stat) {                		\
		right_Sync();   						\
	}                                       	\


#define io_HandleSideSelection(Key, KeyRequired, SidePtArgOne, SidePtArgTwo, SideArgTwo)    \
	if (Key == KeyRequired && right_CurrentSelectionEq(SidePtArgOne)) {                     \
		lp_SetFocus(&r_currentSelection, &SidePtArgOne, &SidePtArgTwo);                     \
		right_DispSimplified(SideArgTwo);                                                   \
	}                                                                                       \


#define io_HandleAngleSelection(Key, KeyRequired, AnglePtArgOne, AnglePtArgTwo,DrawFunctionsAngleFn)	\
	if (Key == KeyRequired && right_CurrentSelectionEq(AnglePtArgOne)) {								\
		lp_SetFocus(&r_currentSelection, &AnglePtArgOne, &AnglePtArgTwo);								\
		((DrawFunctionsAngle)&DrawFunctionsAngleFn)();													\
	}																									\

// @formatter:on

static void ui_IteratePossibleSelections();


static void dbgPt(char* lbl, labelpoint_t pt);

static bool right_CurrentSelectionEq(labelpoint_t pt);

void right_SolveTriangle();

static void right_SelectSide();

static void right_SelectAngle();

static void right_Redraw();

/**
 * @brief right_DrawFraction(), right_DispFunctionRatioRoot()
 */
typedef void (* DrawFunction)(gfx_point_t, side_t, side_t);

/**
 * @brief ui_DrawFunctions_A(), ui_DrawFunctions_B(), ui_DrawFunctions_90()
 */
typedef void (* DrawFunctionsAngle)(void);

static void pythag_CheckSolvability();

/**
 * Uses basic heuristics to automatically detect if a triangle is
 * a special right triangle (45-45-90, 30-60-90)
 *
 * An SRT will automatically be detected if
 *  - Its angles are detected
 *  - Sides of a 45-45-90 are detected
 *
 * A 30-60-90 will not be detected by its sides.
 *
 * Also updates displayed values
 */
static void right_Sync();

void ui_DispSimplified(real_t* r);

static void right_TruncateLabels(int len);

static void dbgSide(char* lbl, side_t side);

static void dbgAngle(char* lbl, angle_t angle);

void right_dbg();

static void ui_SetRounding();

static void ui_AutoDrawFunctions();

static void ui_SwitchRounding();

static void ui_SwitchDispSimpRoot();

static void ui_ClearDispSimpRoot();

static void right_Reset();

static void right_DrawSides();

void right_DrawFraction(gfx_point_t pt, side_t num, side_t denom);

void right_DispFunctionRatioRoot(gfx_point_t pt, side_t num, side_t denom);

#endif //TITRIG_RIGHTTRIANGLE_H
