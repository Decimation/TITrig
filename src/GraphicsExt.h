//
// Created by Decimation on 4/27/2018.
//

#ifndef TITRIG_GRAPHICSEXT_H
#define TITRIG_GRAPHICSEXT_H

#include <graphx.h>
#include <stddef.h>
#include <tice.h>
#include "Settings.h"

typedef struct {
	gfx_point_t point;
	char        label[20];
} superpoint_t;

typedef struct {
	real_t A, B, C;
	real_t a, b, c;
	real_t area;
	real_t perimeter;
} triangle_t;

typedef struct {
	bool a, b, c;
	bool A, B, C;
	bool complete;
	bool isSSA;
	//bool area;
	//bool perimeter;
} trigstatus_t;

void gfx_Print(const superpoint_t* p);

void gfx_Clear(const superpoint_t* p);

real_t io_gfx_ReadReal(superpoint_t* vBuffer);

bool PointEq(const superpoint_t a, const superpoint_t b);

void sp_SetLabel(const superpoint_t* p, const char* s);

void gfx_ClearHighlight(const superpoint_t* p);

void gfx_HighlightPoint(const superpoint_t* p);

void gfx_RadicalFraction(gfx_point_t point, int24_t numOuter, int24_t numInner, int24_t denomOuter, int24_t denomInner);

void gfx_Fraction(gfx_point_t point, real_t num, real_t denom);

void gfx_FractionStr(gfx_point_t point, const char* num, const char* denom);

void gfx_Sqrt(gfx_point_t point, int24_t outer, int24_t inner);

void gfx_PrintColor(const superpoint_t* p, uint8_t color);

void gfx_SetFocus(superpoint_t** currSel, superpoint_t* from, superpoint_t* to);

#endif //TITRIG_GRAPHICSEXT_H
