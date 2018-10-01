//
// Created by Decimation on 9/29/2018.
//

#include "DynamicTriangle.h"
#include "../Types.h"

#include "../System.h"
#include "../Right/RightTriangle.h"

bool g_enableRounding;


void tri_Reset(dynamictriangle_t* this)
{
	int i = 0;
	uint8_t* bptr = (uint8_t*) &this->status;
	real_t * rptr = (real_t*) &this->triangle;

	for (; i < sizeof(trigstatus_t) / sizeof(uint8_t); i++) {
		bptr[i] = false;
	}
	for (i = 0; i < sizeof(triangle_t) / sizeof(real_t); i++) {
		rptr[i] = os_Int24ToReal(0);
	}
}

void __tri_Round(triangle_t* this, uint8_t places)
{
	int i = 0;
	real_t oldVal;
	char buf[20], buf2[20];
	real_t* const mem = (real_t*) this;
	if (!g_enableRounding) {
		return;
	}
	for (; i < sizeof(*this) / sizeof(real_t); i++) {
		oldVal = mem[i];
		mem[i] = os_RealRound(&mem[i], places);
		sys_WriteBuffer(buf, &oldVal);
		sys_WriteBuffer(buf2, &mem[i]);
		dbg_sprintf(dbgout, "[Dyn] Rounded real_t memory @ 0x%p [%s] -> [%s]\n", &mem[i], buf,buf2);
	}
}

void tri_Round(dynamictriangle_t* this, uint8_t places)
{
	__tri_Round(&this->triangle, places);
}

real_t tri_SquareSide(dynamictriangle_t* this, side_t side)
{
	LocalReal(2);
	return os_RealPow(tri_GetSideValuePtr(this, side), &real2);
}

real_t* tri_GetSideValuePtr(dynamictriangle_t* this, side_t side)
{
	switch (side) {
		case SIDE_a:
			return &this->triangle.a;
		case SIDE_b:
			return &this->triangle.b;
		case SIDE_c:
			return &this->triangle.c;
	}
	THROW("tri_GetSideValuePtr")
	// This should never happen
	return NULL;
}

real_t* tri_GetAngleValuePtr(dynamictriangle_t* this, angle_t angle)
{
	switch (angle) {
		case ANGLE_A:
			return &this->triangle.A;
		case ANGLE_B:
			return &this->triangle.B;
		case ANGLE_C90:
			return &this->triangle.C;
	}
	THROW("tri_GetAngleValuePtr")
	// This should never happen
	return NULL;
}

real_t tri_DivSide(dynamictriangle_t* this, side_t side, real_t val)
{
	real_t sideVal = tri_GetSideValue(this, side);
	return os_RealDiv(&sideVal, &val);
}

real_t tri_SubAngle(dynamictriangle_t* this, angle_t angle, real_t value)
{
	real_t angleVal = tri_GetAngleValue(this, angle);
	return os_RealSub(&angleVal, &value);
}

bool tri_SidesEq(dynamictriangle_t* this, side_t one, side_t two)
{
	real_t oneVal = tri_GetSideValue(this, one);
	real_t twoVal = tri_GetSideValue(this, two);
	return (bool) (os_RealCompare(&oneVal, &twoVal) == 0);
}

real_t tri_MulSide(dynamictriangle_t* this, side_t side, real_t val)
{
	real_t sideVal = tri_GetSideValue(this, side);
	return os_RealMul(&sideVal, &val);
}


void tri_UpdateBuffers(dynamictriangle_t* this)
{
	if (this->status.A) {
		sys_WriteBuffer(r_angle_A.label, &this->triangle.A);
	}
	if (this->status.B) {
		sys_WriteBuffer(r_angle_B.label, &this->triangle.B);
	}
	if (this->status.C) {
		sys_WriteBuffer(r_angle_C.label, &this->triangle.C);
	}
	if (this->status.a) {
		sys_WriteBuffer(r_side_a.label, &this->triangle.a);
	}
	if (this->status.b) {
		sys_WriteBuffer(r_side_b.label, &this->triangle.b);
	}
	if (this->status.c) {
		sys_WriteBuffer(r_side_c.label, &this->triangle.c);
	}
}

bool tri_AngleEq(dynamictriangle_t* this, angle_t angle, real_t value)
{
	real_t value2 = tri_GetAngleValue(this, angle);
	return (bool) (os_RealCompare(&value2, &value) == 0);
}

bool tri_SideEq(dynamictriangle_t* this, side_t side, real_t value)
{
	real_t value2 = tri_GetSideValue(this, side);
	return (bool) (os_RealCompare(&value2, &value) == 0);
}

real_t tri_GetAngleValue(dynamictriangle_t* this, angle_t angle)
{
	switch (angle) {
		case ANGLE_A:
			tri_GetValueInternal(this, A)
		case ANGLE_B:
			tri_GetValueInternal(this, B)
		case ANGLE_C90:
			tri_GetValueInternal(this, C)
	}
	THROW("tri_GetAngleValue")
	// This should never happen
	return os_Int24ToReal(-1);
}

real_t tri_GetSideValue(dynamictriangle_t* this, side_t side)
{

	switch (side) {
		case SIDE_a:
			tri_GetValueInternal(this, a)
		case SIDE_b:
			tri_GetValueInternal(this, b)
		case SIDE_c:
			tri_GetValueInternal(this, c)
	}

	THROW("tri_GetSideValue")
	// This should never happen
	return os_Int24ToReal(-1);
}