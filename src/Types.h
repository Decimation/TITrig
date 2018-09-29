//
// Created by Decimation on 5/4/2018.
//

#ifndef TITRIG_TYPES_H
#define TITRIG_TYPES_H

#include <stdbool.h>
#include <tice.h>
#include <graphx.h>

typedef struct {
	gfx_point_t point;
	char        label[20];
} superpoint_t;

typedef struct {
	real_t A, B, C;
	real_t a, b, c;
	real_t area;
	real_t perimeter;
} triangle_t;

typedef struct {
	bool a, b, c;
	bool A, B, C;
	bool complete;
	bool isSSA;
	//bool area;
	//bool perimeter;
} trigstatus_t;


#endif //TITRIG_TYPES_H
