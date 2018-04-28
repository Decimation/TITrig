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

void gfx_Clear(superpoint_t* p);

real_t io_gfx_ReadReal(superpoint_t* vBuffer);

bool PointEq(superpoint_t a, superpoint_t b);

void sp_SetLabel(superpoint_t* p, const char* s);

void gfx_ClearHighlight(superpoint_t* p);

void gfx_HighlightPoint(superpoint_t* p);

void gfx_PrintColor(superpoint_t* p, uint8_t color);

#endif //TITRIG_GRAPHICSEXT_H
