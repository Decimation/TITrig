//
// Created by Decimation on 4/27/2018.
//

#include "GraphicsExt.h"
#include "Library.h"
#include "Triangle.h"
#include <tice.h>
#include <graphx.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>
#include <stdbool.h>

void gfx_Clear(superpoint_t* p)
{
	int w = gfx_GetStringWidth(p->label);
	gfx_SetColor(gfx_white);
	gfx_FillRectangle(p->point.x, p->point.y, w, 9);
	gfx_SetColor(gfx_blue);
}

void gfx_Print(const superpoint_t* p)
{
	gfx_PrintStringXY(p->label, p->point.x, p->point.y);
}

void sp_SetLabel(superpoint_t* p, const char* s)
{
	gfx_Clear(p);
	lib_MemZero(p->label, 20);
	strncpy(p->label, s, strlen(s));
}

real_t io_gfx_ReadReal(superpoint_t* point)
{
	bool        isNeg    = false;
	uint8_t     key, i   = 0;
	real_t      rbuffer;
	static char lchars[] = "\0\0\0\0\0\0\0\0\0\0\"-RMH\0\0?[69LG\0\0.258KFC\0 147JEB\0\0XSNIDA\0\0\0\0\0\0\0\0";
	gfx_Clear(point);
	lib_MemZero(point->label, 20);



	lchars[33] = '0';
	lchars[18] = '3';

	while ((key = os_GetCSC()) != sk_Enter)
	{

		if (key == sk_Del)
		{
			gfx_Clear(point);
			point->label[--i] = '\0';
			gfx_Clear(point);
			gfx_Print(point);
		}

		if (key == 0x11) // todo: remove negative number support in GFX as our triangles can't have signed values
		{
			dbg_sprintf(dbgout, "Negative sign detected\n");
			point->label[i++] = char_Neg;
			gfx_Clear(point);
			gfx_Print(point);
			isNeg = true;
		}

		else if (lchars[key] && i + 1 <= gDigitThreshold)
		{
			point->label[i++] = lchars[key];
		}
		gfx_Print(point);

		gfx_HorizLine(point->point.x, point->point.y + 8, gfx_GetStringWidth(point->label));
	}

	rbuffer = os_StrToReal(point->label ,NULL);
	if (isNeg) rbuffer = os_RealNeg(&rbuffer);

	/**/
	lib_StrReplace(point->label, char_Neg, '-');
	dbg_sprintf(dbgout, "[IO In] %s\n", point->label);

	return rbuffer;
}


bool PointEq(superpoint_t a, superpoint_t b)
{
	return a.point.x == b.point.x && a.point.y == b.point.y;
}

void gfx_ClearHighlight(superpoint_t* p)
{
	gfx_Clear(p);
	gfx_PrintColor(p, gfx_black);
}

void gfx_PrintColor(superpoint_t* p, uint8_t color)
{
	gfx_SetTextFGColor(color);
	gfx_PrintStringXY(p->label, p->point.x, p->point.y);
	gfx_SetTextFGColor(gfx_black);
}

void gfx_HighlightPoint(superpoint_t* p)
{
	gfx_PrintColor(p, gfx_red);
}