//
// Created by Decimation on 4/27/2018.
//

#ifndef TITRIG_GRAPHICSEXT_H
#define TITRIG_GRAPHICSEXT_H

#include <graphx.h>
#include <stddef.h>
#include <tice.h>
#include "Settings.h"
#include "Types.h"
#include "LabelPoint/LabelPoint.h"

void gfx_DispFunctionRatioRoot(gfx_point_t pt, real_t x, real_t y);

void gfx_ClearArea(gfx_point_t point, uint8_t w, uint8_t h);

void gfx_ClearFraction(gfx_point_t point);

void gfx_RadicalFraction(gfx_point_t point, int24_t numOuter, int24_t numInner, int24_t denomOuter, int24_t denomInner);

void gfx_Fraction(gfx_point_t point, real_t num, real_t denom);

void gfx_FractionStr(gfx_point_t point, const char* num, const char* denom);

void gfx_Sqrt(gfx_point_t point, int24_t outer, int24_t inner);

#endif //TITRIG_GRAPHICSEXT_H
