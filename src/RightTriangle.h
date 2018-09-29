//
// Created by Decimation on 4/28/2018.
//

#ifndef TITRIG_RIGHTTRIANGLE_H
#define TITRIG_RIGHTTRIANGLE_H

#include <tice.h>
#include <graphx.h>
#include "GraphicsExt.h"

#define Point_FN_Cot s_rgFuncFracPoints2[2]
#define Point_FN_Sec s_rgFuncFracPoints2[1]
#define Point_FN_Csc s_rgFuncFracPoints2[0]

#define Point_FN_Sin s_rgFuncFracPoints[0]
#define Point_FN_Cos s_rgFuncFracPoints[1]
#define Point_FN_Tan s_rgFuncFracPoints[2]

#define Point_Side_a s_rgSides[1]
#define Point_Side_b s_rgSides[0]
#define Point_Side_c s_rgSides[2]

#define Point_Angle_A s_rgAngles[2]
#define Point_Angle_B s_rgAngles[1]
#define Point_Angle_C s_rgAngles[0]

#define Point_Side_OAH_a s_rgSidesOAH[1]
#define Point_Side_OAH_b s_rgSidesOAH[0]
#define Point_Side_OAH_c s_rgSidesOAH[2]

static void ui_InitLabels(void);

static void ui_SwapFocus(labelpoint_t* from, labelpoint_t* to, real_t* disp);

static void local_SetSide(real_t r, sidetype_t sidetype);

static void local_SetAngle(real_t r, angletype_t angletype);

static bool local_SelectionEquals(labelpoint_t p);

void right_SolveTriangle(void);

static void right_SelectSide(void);

static void right_SelectAngle(void);

static void ui_DrawFunctions_90(void);

static void right_Redraw(void);

static void ui_DrawFunctions_B(void);

static void ui_DrawFunctions_A(void);

static void pythag_CheckSolvability(void);

static void right_Sync(void);

static void ui_DispSimplified(real_t* r);

static void right_TruncateLabels(int len);

static void ui_WriteLabel(char* szLabel, real_t* pReal);

static void ui_SetRounding(void);

static void ui_AutoDrawFunctions(void);

static void ui_SwitchRounding(void);

static void ui_SwitchDispSimpRoot(void);

static void ui_ClearDispSimpRoot(void);

static void ui_ClearFraction(gfx_point_t point);

static void ui_ClearArea(gfx_point_t point, uint8_t w, uint8_t h);

static void right_Reset(void);

static void ui_DispFunctionRatioRoot(gfx_point_t pt, real_t x, real_t y);

static void right_DrawSides(void);

static void right_DebugTriangle(void);

#endif //TITRIG_RIGHTTRIANGLE_H
