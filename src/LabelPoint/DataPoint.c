//
// Created by Decimation on 9/30/2018.
//

#include "DataPoint.h"
#include "../GraphicsExt.h"


void dp_Clear(const datapoint_t* dp) {
	lp_Clear(&dp->left);
	gfx_ClearFraction(dp->data);
}