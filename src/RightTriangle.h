//
// Created by Decimation on 4/28/2018.
//

#ifndef TITRIG_RIGHTTRIANGLE_H
#define TITRIG_RIGHTTRIANGLE_H

#include <tice.h>
#include <graphx.h>

void right_SolveTriangle();

static void right_SelectAngle();

static void sohcahtoa_Draw90();

static void right_Redraw();

static void sohcahtoa_DrawB();

static void sohcahtoa_DrawA();

static void pythag_CheckSolvability();

static void right_Sync();

static void disp_Simplified(real_t* r);

static void right_TruncateLabels(int len);

static void ui_SwitchDispSimpRoot();

static void ui_ClearDispSimpRoot();

static void ui_ClearFraction(gfx_point_t point);

static void ui_ClearArea(gfx_point_t point, uint8_t w, uint8_t h);

#endif //TITRIG_RIGHTTRIANGLE_H
