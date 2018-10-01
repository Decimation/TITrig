//
// Created by Decimation on 9/30/2018.
//

#ifndef TITRIG_DATAPOINT_H
#define TITRIG_DATAPOINT_H

#include <graphx.h>
#include "LabelPoint.h"


/**
 * @brief Typically used to display fractions or roots
 */
typedef struct
{
	labelpoint_t left;
	gfx_point_t data;
} datapoint_t;

void dp_Clear(const datapoint_t* dp);

#endif
