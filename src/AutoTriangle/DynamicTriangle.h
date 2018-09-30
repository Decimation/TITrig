//
// Created by Decimation on 9/29/2018.
//

#ifndef TITRIG_AUTOTRIANGLE_H
#define TITRIG_AUTOTRIANGLE_H

#include "../Types.h"
#include <stdbool.h>

typedef struct
{
	triangle_t   triangle;
	trigstatus_t status;
} dynamictriangle_t;

// @formatter:off

#define tri_SetValueInternal(Auto, V, Value)    \
    Auto->triangle.V = Value;                   \
    Auto->status.V = true;                      \

#define tri_GetValueInternal(Auto, V)           \
    return Auto->triangle.V;                    \


#define tri_SetAngle(this, angle, value)    \
    switch (angle) {                        \
		case ANGLE_A:                       \
		this.triangle.A = value;            \
		this.status.A=true;					\
		break;                              \
	case ANGLE_B:                           \
		this.triangle.B = value;            \
		this.status.B=true;					\
		break;                              \
	case ANGLE_C90:                         \
		this.triangle.C = value;            \
		this.status.C=true;					\
		break;                              \
	}                                       \


#define tri_SetSide(this, side, value)    	\
	switch (side) {                         \
		case SIDE_a:                        \
		this.triangle.a = value;            \
		this.status.a=true;					\
		break;								\
	case SIDE_b:							\
		this.triangle.b = value;            \
		this.status.b=true;					\
		break;								\
	case SIDE_c:							\
		this.triangle.c = value;            \
		this.status.c=true;					\
		break;								\
	}										\

// @formatter:on


void __tri_Round(triangle_t* this, uint8_t places);

void lib_NameOfAngle(char* buf, angle_t angle);

void tri_Reset(dynamictriangle_t* this);

void tri_Round(dynamictriangle_t* this, uint8_t places);

real_t tri_SquareSide(dynamictriangle_t* this, side_t side);

real_t* tri_GetSideValuePtr(dynamictriangle_t* this, side_t side);

real_t* tri_GetAngleValuePtr(dynamictriangle_t* this, angle_t angle);

real_t tri_DivSide(dynamictriangle_t* this, side_t side, real_t val);

real_t tri_SubAngle(dynamictriangle_t* this, angle_t angle, real_t value);

bool tri_SidesEq(dynamictriangle_t* this, side_t one, side_t two);

real_t tri_MulSide(dynamictriangle_t* this, side_t side, real_t val);

/**
 * Updates the corresponding labels with the values of the triangle
 * @param this
 */
void tri_UpdateBuffers(dynamictriangle_t* this);

bool tri_AngleEq(dynamictriangle_t* this, angle_t angle, real_t value);

bool tri_SideEq(dynamictriangle_t* this, side_t side, real_t value);

real_t tri_GetAngleValue(dynamictriangle_t* this, angle_t angle);

real_t tri_GetSideValue(dynamictriangle_t* this, side_t side);

extern bool g_enableRounding;

#endif