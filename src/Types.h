//
// Created by Decimation on 5/4/2018.
//

#ifndef TITRIG_TYPES_H
#define TITRIG_TYPES_H

#include <stdbool.h>
#include <tice.h>
#include <graphx.h>

typedef struct
{
	real_t A, B, C;
	real_t a, b, c;
	real_t area;
	real_t perimeter;
} triangle_t;

typedef struct
{

	/**
	 * Angles
	 */
	bool a, b, c;

	/**
	 * Sides
	 */
	bool A, B, C;

	/**
	 * Whether the triangle is complete
	 */
	bool complete;
	bool isSSA;
} trigstatus_t;


typedef enum
{
	ANGLE_A, ANGLE_B, ANGLE_C90
} angletype_t;

typedef enum {
	SIDE_A, SIDE_B, SIDE_C
} sidetype_t;

#endif //TITRIG_TYPES_H
