//
// Created by Decimation on 4/27/2018.
//

#ifndef TITRIG_TRIGONOMETRY_H
#define TITRIG_TRIGONOMETRY_H

#include <tice.h>
#include "Types.h"

float SinDeg(float f);

float ASinDeg(float f);

float CosineDeg(float f);

real_t os_RealSinDeg(real_t r);

real_t os_RealAcosDeg(real_t r);

real_t los_Side_x(real_t X, real_t y, real_t Y);

real_t los_Angle_x(real_t x, real_t Y, real_t y);

void tri_SetAngle(triangle_t* pTriangle, real_t r, trigstatus_t* pTriStatus, angletype_t angle);
void tri_SetSide(triangle_t* pTriangle, real_t r, trigstatus_t* pTriStatus,  sidetype_t side);
real_t loc_Side_x(real_t a, real_t b, real_t angle);

real_t loc_Angle_x(real_t c, real_t a, real_t b);

#endif //TITRIG_TRIGONOMETRY_H
