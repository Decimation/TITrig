//
// Created by Decimation on 5/4/2018.
//

#ifndef TITRIG_TYPES_H
#define TITRIG_TYPES_H

#include <stdbool.h>
#include <tice.h>
#include <graphx.h>
#include <debug.h>


typedef struct
{
	real_t A, B, C;
	real_t a, b, c;
	real_t area;
	real_t perimeter;
} triangle_t;

typedef struct
{
	bool a, b, c;
	bool A, B, C;
	bool complete;
	bool isSSA;
	//bool area;
	//bool perimeter;
} trigstatus_t;

typedef enum
{
	ANGLE_A, ANGLE_B, ANGLE_C90
} angle_t;

typedef enum
{
	SIDE_a, SIDE_b, SIDE_c
} side_t;


#endif //TITRIG_TYPES_H
