//
// Created by Decimation on 4/27/2018.
//

#ifndef TITRIG_RIGHTTRIANGLE_H
#define TITRIG_RIGHTTRIANGLE_H

#include "System.h"
#include "GraphicsExt.h"


// Area = (1/2)|det[xA xB xC  ] |
//				   [ yA yB yC ] |
//				   [ 1	1	1 ] |
//void AreaFrom3Points();

void trig_SolveTriangle();

static void ui_DispData();

static void trig_HeronsFormula();

static void ui_ClearMeasurements();

static void ui_ClearAngleSideData();

real_t trig_AreaBySin(real_t a, real_t b, real_t C);

static void ui_DispMeasurements();

static void trig_DrawTriangleSides();

static void trig_Sync();

static void trig_SelectSide();

static void trig_SelectAngle();

static void trig_Redraw();

static void trig_Quit();

static void trig_CheckSolvability();

static void dbg_printTriangle();

static void trig_TruncateLabels(int len);

static void ui_DispAngleSideData();

static void trig_SolveMissingAngle();

static void trig_SolveSSS();

static void trig_Reset();

#endif //TITRIG_RIGHTTRIANGLE_H
