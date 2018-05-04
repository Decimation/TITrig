//
// Created by Decimation on 4/28/2018.
//

#ifndef TITRIG_RIGHTTRIANGLE_H
#define TITRIG_RIGHTTRIANGLE_H

#include <tice.h>
#include <graphx.h>
#include "GraphicsExt.h"

void right_SolveTriangle();

static void right_SelectAngle();

static void ui_DrawFunctions_90();

static void right_Redraw();

static void ui_DrawFunctions_B();

static void ui_DrawFunctions_A();

static void pythag_CheckSolvability();

static void right_Sync();

static void ui_DispSimplified(real_t* r);

static void right_TruncateLabels(int len);

static void ui_SwitchDispSimpRoot(superpoint_t* currSelection);

static void ui_ClearDispSimpRoot();

static void ui_ClearFraction(gfx_point_t point);

static void ui_ClearArea(gfx_point_t point, uint8_t w, uint8_t h);

static void right_Reset();

static void ui_DispFunctionRatioRoot(gfx_point_t pt, real_t x, real_t y);

#endif //TITRIG_RIGHTTRIANGLE_H
