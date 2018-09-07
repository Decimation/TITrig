//
// Created by Decimation on 9/6/2018.
//

#ifndef TITRIG_LABELPOINT_T_H
#define TITRIG_LABELPOINT_T_H

#include "Types.h"

#define kLabelPointLabelSize 20

typedef struct
{
	gfx_point_t point;
	char        label[kLabelPointLabelSize];
} labelpoint_t;

void lp_FullClear(const labelpoint_t* p);

void lp_SetLabelFromInt24(const labelpoint_t* p, int i);

bool lp_Equal(labelpoint_t a, labelpoint_t b);

void lp_SetLabel(const labelpoint_t* p, const char* s);

void lp_ClearHighlight(const labelpoint_t* p);

void lp_Highlight(const labelpoint_t* p);

void lp_PrintColor(const labelpoint_t* p, uint8_t color);

void lp_SetFocus(labelpoint_t** currSel, labelpoint_t* from, labelpoint_t* to);

void lp_Print(const labelpoint_t* p);

void lp_Clear(const labelpoint_t* p);

#endif