//
// Created by Decimation on 4/28/2018.
//

#ifndef TITRIG_RIGHTTRIANGLE_H
#define TITRIG_RIGHTTRIANGLE_H

#include <tice.h>
#include <graphx.h>

void right_SolveTriangle();

void right_SelectAngle();

void sohcahtoa_Draw90();

void right_Redraw();

void sohcahtoa_DrawB();

void sohcahtoa_DrawA();

void pythag_CheckSolvability();

void right_Sync();

void disp_Simplified(real_t* r);

void right_TruncateLabels(int len);

void ui_SwitchDispSimpRoot();

void ui_ClearDispSimpRoot();

void ui_ClearFraction(gfx_point_t point);

void ui_ClearArea(gfx_point_t point, uint8_t w, uint8_t h);

#endif //TITRIG_RIGHTTRIANGLE_H
