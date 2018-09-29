//
// Created by Decimation on 4/28/2018.
//

#include <stdint.h>
#include <graphx.h>
#include <stdio.h>
#include <string.h>
#include "Settings.h"
#include "GraphicsExt.h"

uint8_t gRound          = 0;
uint8_t gDigitThreshold = 6;

static superpoint_t spRound        = {{20, 20}, "Rounding digits: "};
static superpoint_t spDigThreshold = {{20, 40}, "Digit threshold: "};

//todo
void set_Menu()
{
	char babybuf[5];
	gfx_ZeroScreen();

	sprintf(babybuf, "%d", gRound);
	strncpy(spRound.label + 17, babybuf, strlen(babybuf));

	sprintf(babybuf, "%d", gDigitThreshold);
	strncpy(spDigThreshold.label + 17, babybuf, strlen(babybuf));

}
