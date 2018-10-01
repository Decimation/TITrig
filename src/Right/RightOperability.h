//
// Created by Decimation on 9/29/2018.
//

#ifndef TITRIG_RIGHTOPERABILITY_H
#define TITRIG_RIGHTOPERABILITY_H

#include "../Types.h"
#include "RightTriangle.h"
#include "../DynamicTriangle/DynamicTriangle.h"
#include "../LabelPoint/LabelPoint.h"

#define available_A g_rAutoTriangle.status.A
#define available_B g_rAutoTriangle.status.B
#define available_C g_rAutoTriangle.status.C

#define available_a g_rAutoTriangle.status.a
#define available_b g_rAutoTriangle.status.b
#define available_c g_rAutoTriangle.status.c

side_t right_PointToSide(labelpoint_t pt);

real_t* right_GetSideValuePtr(side_t side);

real_t right_SquareSide(side_t side);

void right_DispSimplified(side_t side);

real_t* right_GetAngleValuePtr(angle_t angle);

labelpoint_t* right_SideToPoint(side_t side);

void right_SetAndDispSide(side_t side);

bool right_SidesEq(side_t one, side_t two);

bool right_AngleEq(angle_t angle, real_t value);

void right_UpdateBuffers();

//void i_right_SetSide(side_t side, real_t* value);

real_t right_GetSide(side_t side);

//void i_right_SetAngle(angle_t angle, real_t* value);

real_t right_MulSide(side_t side, real_t value);

real_t right_DivSide(side_t side, real_t value);

#define right_SetAngle(__angle, __value)                \
    tri_SetAngle(g_rAutoTriangle, __angle, __value)     \

#define right_SetSide(__side, __value)                  \
    tri_SetSide(g_rAutoTriangle,__side, __value)        \

#endif//TITRIG_RIGHTOPERABILITYH
