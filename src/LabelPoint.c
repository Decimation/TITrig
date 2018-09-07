//
// Created by Decimation on 9/6/2018.
//

#include <debug.h>
#include <string.h>
#include "LabelPoint.h"
#include "Library.h"

void lp_Clear(const labelpoint_t* p)
{
	const int w = gfx_GetStringWidth(p->label);
	gfx_SetColor(gfx_white);
	gfx_FillRectangle(p->point.x, p->point.y, w, 9);
	gfx_SetColor(gfx_blue);
}

void lp_Print(const labelpoint_t* p)
{
	gfx_PrintStringXY(p->label, p->point.x, p->point.y);
}

void lp_SetLabelFromInt24(const labelpoint_t* p, int i)
{
	lp_FullClear(p);
	sprintf(p->label, "%d", i);
}

void lp_SetLabel(const labelpoint_t* p, const char* s)
{
	lp_FullClear(p);
	strncpy(p->label, s, strlen(s));
}

void lp_FullClear(const labelpoint_t* p)
{
	lp_Clear(p);
	lib_MemZero(p->label, kLabelPointLabelSize);
}

void lp_SetFocus(labelpoint_t** currSel, labelpoint_t* from, labelpoint_t* to)
{
	lp_ClearHighlight(from);
	lp_Highlight(to);
	*currSel = to;
	dbg_sprintf(dbgout, "[GFX] %s -> %s\n", from->label, to->label);
}

void lp_ClearHighlight(const labelpoint_t* p)
{
	lp_Clear(p);
	lp_PrintColor(p, gfx_black);
}

void lp_PrintColor(const labelpoint_t* p, uint8_t color)
{
	gfx_SetTextFGColor(color);
	gfx_PrintStringXY(p->label, p->point.x, p->point.y);
	gfx_SetTextFGColor(gfx_black);
}

void lp_Highlight(const labelpoint_t* p)
{
	lp_PrintColor(p, gfx_red);
}


bool lp_Equal(const labelpoint_t a, const labelpoint_t b)
{
	return (bool) (a.point.x == b.point.x && a.point.y == b.point.y);
}