//
// Created by Decimation on 4/28/2018.
//

#ifndef TITRIG_RIGHTTRIANGLE_H
#define TITRIG_RIGHTTRIANGLE_H

#include "../AutoTriangle/DynamicTriangle.h"
#include <tice.h>
#include <graphx.h>
#include "../GraphicsExt.h"


#define fnCotPt func2FracPoints[2]
#define fnSecPt func2FracPoints[1]
#define fnCscPt func2FracPoints[0]

#define fnSinPt funcFracPoints[0]
#define fnCosPt funcFracPoints[1]
#define fnTanPt funcFracPoints[2]

#define r_side_a g_rXSides[1]
#define r_side_b g_rXSides[0]
#define r_side_c g_rXSides[2]

#define r_angle_A g_rXAngles[2]
#define r_angle_B g_rXAngles[1]
#define r_angle_C g_rXAngles[0]

// @formatter:off

#define io_HandleKeyInput(Key)          \
    if (Key == sk_Clear) {              \
        return;                         \
    }                                   \
    if (Key == sk_Zoom) {               \
        ui_SwitchDispSimpRoot();        \
    }                                   \
    if (Key == sk_Window) {             \
        ui_SwitchRounding();            \
    }                                   \
    if (Key == sk_Yequ) {               \
        ui_SetRounding();               \
    }                                   \


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

extern const char lbl_ADJ[];
extern const char lbl_HYP[];
extern const char lbl_OPP[];

extern dynamictriangle_t     g_rAutoTriangle;
extern labelpoint_t       ui_Wait;
extern labelpoint_t       g_rXSides[3];
extern labelpoint_t       g_rXSides_OAH[3];
extern labelpoint_t       g_rXAngles[3];
extern const labelpoint_t funcData[3];
extern const gfx_point_t  funcFracPoints[3];
extern const labelpoint_t funcData2[3];
extern const gfx_point_t  func2FracPoints[3];
extern bool               ui_dispSimpRoot;

static void dbgPt(char* lbl, labelpoint_t pt);

static bool right_CurrentSelectionEq(labelpoint_t pt);

void right_SolveTriangle();

static void right_SelectSide();

static void right_SelectAngle();

static void right_Redraw();

/**
 * @brief
 */
typedef void (* DrawFunction)(gfx_point_t, side_t, side_t);

/**
 * @brief ui_DrawFunctions_A(), ui_DrawFunctions_B(), ui_DrawFunctions_90()
 */
typedef void (* DrawFunctionsAngle)(void);

static void pythag_CheckSolvability();

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

void ui_ClearFraction(gfx_point_t point);

static void ui_ClearArea(gfx_point_t point, uint8_t w, uint8_t h);

static void right_Reset();

static void ui_DispFunctionRatioRoot(gfx_point_t pt, real_t x, real_t y);

static void right_DrawSides();

void right_DrawFraction(gfx_point_t pt, side_t num, side_t denom);

void right_DispFunctionRatioRoot(gfx_point_t pt, side_t num, side_t denom);

#endif //TITRIG_RIGHTTRIANGLE_H
