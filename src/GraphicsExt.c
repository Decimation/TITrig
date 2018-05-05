//
// Created by Decimation on 4/27/2018.
//

#include "GraphicsExt.h"
#include "Library.h"
#include <string.h>
#include <debug.h>

void gfx_RadicalFraction(gfx_point_t point, int24_t numOuter, int24_t numInner, int24_t denomOuter, int24_t denomInner)
{
	char numInnerBuf[10], numOuterBuf[10];
	char denomInnerBuf[10], denomOuterBuf[10];

	int numLength, denomLength;

	sprintf(numOuterBuf, "%d", numOuter);
	sprintf(numInnerBuf, "%d", numInner);

	sprintf(denomOuterBuf, "%d", denomOuter);
	sprintf(denomInnerBuf, "%d", denomInner);

	numLength   = (strlen(numOuterBuf) + strlen(numInnerBuf));
	denomLength = (strlen(denomOuterBuf) + strlen(denomInnerBuf));

	//todo: center the fractions in these two cases
	if (numLength > denomLength)
	{
		gfx_HorizLine(point.x - 1, point.y, gfx_GetStringWidth(numInnerBuf) + gfx_GetStringWidth(numOuterBuf) + 7);

		point.y -= 9;
		gfx_Sqrt(point, numOuter, numInner);

		point.y += 14;
		gfx_Sqrt(point, denomOuter, denomInner);
	}
	else if (numLength < denomLength)
	{
		gfx_HorizLine(point.x - 1, point.y, gfx_GetStringWidth(denomInnerBuf) + gfx_GetStringWidth(denomOuterBuf) + 7);

		point.y -= 9;
		gfx_Sqrt(point, numOuter, numInner);

		point.y += 14;
		gfx_Sqrt(point, denomOuter, denomInner);
	}
	else
	{
		gfx_HorizLine(point.x - 1, point.y, gfx_GetStringWidth(numInnerBuf) + gfx_GetStringWidth(numOuterBuf) + 7);

		point.y -= 9;
		gfx_Sqrt(point, numOuter, numInner);

		point.y += 14;
		gfx_Sqrt(point, denomOuter, denomInner);

	}
}

void gfx_Fraction(gfx_point_t point, real_t num, real_t denom)
{
	char numbuf[10];
	char denombuf[10];
	os_RealToStr(numbuf, &num, 0, 0, 6);
	os_RealToStr(denombuf, &denom, 0, 0, 6);
	gfx_FractionStr(point, numbuf, denombuf);
}

void gfx_FractionStr(gfx_point_t point, const char* num, const char* denom)
{


	if (strlen(num) > strlen(denom))
	{
		gfx_HorizLine(point.x - 1, point.y + 3, gfx_GetStringWidth(num) + 1);

		gfx_PrintStringXY(denom, point.x + ((gfx_GetStringWidth(denom) + 1) / 2), point.y + 5);
		gfx_PrintStringXY(num, point.x, point.y - 5);
	}
	else if (strlen(num) < strlen(denom))
	{
		gfx_HorizLine(point.x - 1, point.y + 3, gfx_GetStringWidth(denom) + 1);

		gfx_PrintStringXY(num, point.x + ((gfx_GetStringWidth(num) + 1) / 2), point.y - 5);
		gfx_PrintStringXY(denom, point.x, point.y + 5);
	}
	else
	{
		gfx_HorizLine(point.x - 1, point.y + 3, gfx_GetStringWidth(denom) + 1);

		gfx_PrintStringXY(num, point.x, point.y - 5);
		gfx_PrintStringXY(denom, point.x, point.y + 5);
	}
}

void gfx_Sqrt(gfx_point_t point, int24_t outer, int24_t inner)
{
	char buf[20];
	int  strW;
	dbg_sprintf(dbgout, "[GFX] Drawing sqrt\n");
	sprintf(buf, "%d", outer);
	strW = gfx_GetStringWidth(buf);

	//190, 155
	gfx_PrintStringXY(buf, point.x, point.y);

	//198, 158 -> 200, 160
	gfx_Line(point.x + strW, point.y + 3, point.x + strW + 2, point.y + 7);

	//205, 155
	gfx_VertLine(point.x + strW + 3, point.y - 3, 11);

	sprintf(buf, "%d", inner);
	gfx_PrintStringXY(buf, point.x + strW + 6, point.y);

	gfx_HorizLine(point.x + strW + 3, point.y - 3, gfx_GetStringWidth(buf) + 3);
}

void gfx_Clear(const superpoint_t* p)
{
	const int w = gfx_GetStringWidth(p->label);
	gfx_SetColor(gfx_white);
	gfx_FillRectangle(p->point.x, p->point.y, w, 9);
	gfx_SetColor(gfx_blue);
}

void gfx_Print(const superpoint_t* p)
{
	gfx_PrintStringXY(p->label, p->point.x, p->point.y);
}

void sp_SetLabel(const superpoint_t* p, const char* s)
{
	gfx_Clear(p);
	lib_MemZero(p->label, 20);
	strncpy(p->label, s, strlen(s));
}


real_t io_gfx_ReadSqrt(superpoint_t* point)
{
	real_t      tmp, initialValue;
	uint8_t     key;
	int         i       = 0;
	const int   strW    = gfx_GetStringWidth(point->label);
	static char chars[] = "\0\0\0\0\0\0\0\0\0\0\"-RMH\0\0?[69LG\0\0.258KFC\0 147JEB\0\0XSNIDA\0\0\0\0\0\0\0\0";
	chars[33] = '0';
	chars[18] = '3';

	if (strlen(point->label) != 0)
	{
		initialValue = os_StrToReal(point->label, NULL);
	}
	else
	{
		initialValue = os_Int24ToReal(1);
	}

	gfx_Clear(point);
	gfx_Print(point);

	gfx_Line(point->point.x + strW, point->point.y + 3, point->point.x + strW + 2, point->point.y + 7);

	gfx_VertLine(point->point.x + strW + 3, point->point.y - 3, 11);

	gfx_HorizLine(point->point.x + strW + 3, point->point.y - 3, strW + 3);

	//gfx_Clear(point);
	lib_MemZero(point->label, 20);

	while ((key = os_GetCSC()) != sk_Enter)
	{
		if (key == sk_Del)
		{
			gfx_Clear(point);
			point->label[--i] = '\0';
			gfx_Clear(point);
			gfx_Print(point);
		}

		else if (chars[key] && i + 1 <= gDigitThreshold)
		{
			point->label[i++] = chars[key];
			gfx_PrintStringXY(point->label, point->point.x + strW + 6, point->point.y);
		}
		gfx_HorizLine(point->point.x + strW + 3, point->point.y - 3, gfx_GetStringWidth(point->label) + 3);
	}

	gfx_SetColor(gfx_white);
	gfx_FillRectangle(point->point.x, point->point.y - 3, gfx_GetStringWidth(point->label) + strW + 6,
					  point->point.y + 3);
	gfx_SetColor(gfx_black);

	tmp = os_StrToReal(point->label, NULL);
	tmp = os_RealSqrt(&tmp);
	return os_RealMul(&initialValue, &tmp);
}

real_t io_gfx_ReadReal(superpoint_t* point)
{
	bool        isNeg   = false;
	uint8_t     key, i  = 0;
	real_t      rbuffer;
	static char chars[] = "\0\0\0\0\0\0\0\0\0\0\"-RMH\0\0?[69LG\0\0.258KFC\0 147JEB\0\0XSNIDA\0\0\0\0\0\0\0\0";
	gfx_Clear(point);
	lib_MemZero(point->label, 20);
	chars[33] = '0';
	chars[18] = '3';

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

		if (chars[key] == 'I')
		{

			rbuffer = io_gfx_ReadSqrt(point);
			return rbuffer;
		}

		else if (chars[key] && i + 1 <= gDigitThreshold)
		{
			point->label[i++] = chars[key];
		}
		gfx_Print(point);
		gfx_HorizLine(point->point.x, point->point.y + 8, gfx_GetStringWidth(point->label));
	}

	rbuffer = os_StrToReal(point->label, NULL);
	if (isNeg) rbuffer = os_RealNeg(&rbuffer);

	/**/
	//lib_StrReplace(point->label, char_Neg, '-');
	//dbg_sprintf(dbgout, "[IO In] %s\n", point->label);

	return rbuffer;
}

bool PointEq(const superpoint_t a, const superpoint_t b)
{
	return a.point.x == b.point.x && a.point.y == b.point.y;
}

void gfx_SetFocus(superpoint_t** currSel, superpoint_t* from, superpoint_t* to)
{
	gfx_ClearHighlight(from);
	gfx_HighlightPoint(to);
	*currSel = to;
	dbg_sprintf(dbgout, "[GFX] %s -> %s\n", from->label, to->label);
}

void gfx_ClearHighlight(const superpoint_t* p)
{
	gfx_Clear(p);
	gfx_PrintColor(p, gfx_black);
}

void gfx_PrintColor(const superpoint_t* p, uint8_t color)
{
	gfx_SetTextFGColor(color);
	gfx_PrintStringXY(p->label, p->point.x, p->point.y);
	gfx_SetTextFGColor(gfx_black);
}

void gfx_HighlightPoint(const superpoint_t* p)
{
	gfx_PrintColor(p, gfx_red);
}