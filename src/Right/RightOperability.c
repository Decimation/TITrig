//
// Created by Decimation on 9/29/2018.
//

#include "RightOperability.h"
#include "../Library.h"


labelpoint_t* right_SideToPoint(side_t side)
{
	switch (side) {
		case SIDE_a:
			return &r_side_a;
		case SIDE_b:
			return &r_side_b;
		case SIDE_c:
			return &r_side_c;
	}

	THROW("right_SideToPoint")
	// This should never happen
	return ((labelpoint_t*) NULL);
}

real_t right_MulSide(side_t side, real_t value)
{
	return tri_MulSide(&g_rAutoTriangle, side, value);
}

bool right_AngleEq(angle_t angle, real_t value)
{
	return tri_AngleEq(&g_rAutoTriangle, angle, value);
}

real_t right_SquareSide(side_t side)
{
	return tri_SquareSide(&g_rAutoTriangle, side);
}

void right_DispSimplified(side_t side)
{
	ui_DispSimplified(tri_GetSideValuePtr(&g_rAutoTriangle, side));
}

real_t* right_GetSideValuePtr(side_t side) {
	return tri_GetSideValuePtr(&g_rAutoTriangle, side);
}

real_t* right_GetAngleValuePtr(angle_t angle)
{
	return tri_GetAngleValuePtr(&g_rAutoTriangle, angle);
}

void right_SetAndDispSide(side_t side)
{
	//right_SetSide(SIDE_c, io_gfx_ReadReal(&side_c));
	//ui_DispSimplified(&triangle.c);


	real_t v = io_gfx_ReadReal(right_SideToPoint(side));
	right_SetSide(side, v);

	ui_DispSimplified(tri_GetSideValuePtr(&g_rAutoTriangle, side));
}

bool right_SidesEq(side_t one, side_t two)
{
	return tri_SidesEq(&g_rAutoTriangle, one, two);
}

side_t right_PointToSide(labelpoint_t pt) {
	if (lp_Equals(pt, r_side_a)) {
		return SIDE_a;
	}
	if (lp_Equals(pt, r_side_b)) {
		return SIDE_b;
	}
	if (lp_Equals(pt, r_side_c)) {
		return SIDE_c;
	}

	THROW("right_PointToSide")
	return 0;
}

void right_UpdateBuffers()
{
	tri_UpdateBuffers(&g_rAutoTriangle);
}

real_t right_DivSide(side_t side, real_t value)
{
	return tri_DivSide(&g_rAutoTriangle, side, value);
}

real_t right_GetSide(side_t side)
{
	return tri_GetSideValue(&g_rAutoTriangle, side);
}

