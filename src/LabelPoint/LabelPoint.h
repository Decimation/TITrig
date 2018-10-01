//
// Created by Decimation on 9/30/2018.
//


#ifndef TITRIG_LABELPOINT_H
#define TITRIG_LABELPOINT_H


#include <graphx.h>
#include <tice.h>

#define kLabelPointLabelSize 20

typedef struct
{
	gfx_point_t point;
	char        label[kLabelPointLabelSize];
} labelpoint_t;

static void io_InitChars();

void lp_ZeroLabel(const labelpoint_t* p);

void lp_Clear(const labelpoint_t* p);

void lp_Print(const labelpoint_t* p);

void lp_SetLabel(const labelpoint_t* p, const char* s);

static void io_HandleDel(labelpoint_t* point, int i);

real_t io_gfx_ReadSqrt(labelpoint_t* point);

void lp_ClrPrint(const labelpoint_t* p);

real_t io_gfx_ReadReal(labelpoint_t* point);

bool lp_Equals(labelpoint_t a, labelpoint_t b);

void lp_SetFocus(labelpoint_t** currSel, labelpoint_t* from, labelpoint_t* to);

void lp_ClearHighlight(const labelpoint_t* p);

void lp_PrintColor(const labelpoint_t* p, uint8_t color);

void lp_HighlightPoint(const labelpoint_t* p);

#endif