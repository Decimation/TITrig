//
// Created by Decimation on 4/27/2018.
//

#ifndef TITRIG_RIGHTTRIANGLE_H
#define TITRIG_RIGHTTRIANGLE_H


#include "Library.h"
#include "GraphicsExt.h"
#include "Geometry.h"

// Area = (1/2)|det[xA xB xC  ] |
//				   [ yA yB yC ] |
//				   [ 1	1	1 ] |
//void AreaFrom3Points();

void ui_DispData();

void trig_HeronsFormula();

void ui_ClearMeasurements();

void ui_ClearAngleSideData();

real_t trig_AreaBySin(real_t a, real_t b, real_t C);

void ui_DispMeasurements();

void trig_DrawTriangleSides();

void trig_Sync();

void trig_SelectSide();

void trig_SelectAngle();

void trig_Redraw();

void trig_SolveTriangle();

void trig_Quit();

void trig_CheckSolvability();

void dbg_printTriangle();

void trig_TruncateLabels(int len);

void ui_DispAngleSideData();

void trig_SolveMissingAngle();

void trig_SolveSSS();

void trig_Reset();

#endif //TITRIG_RIGHTTRIANGLE_H
