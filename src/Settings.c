//
// Created by Decimation on 4/28/2018.
//

#include <stdint.h>
#include <graphx.h>
#include <stdio.h>
#include <string.h>
#include "Settings.h"
#include "GraphicsExt.h"

uint8_t g_uRound          = 0;
uint8_t g_uDigitThreshold = 6;

static labelpoint_t s_Round          = {{20, 20}, "Rounding digits: "};
static labelpoint_t s_DigitThreshold = {{20, 40}, "Digit threshold: "};

//todo
void set_Menu()
{
	char babybuf[5];
	gfx_ZeroScreen();

	sprintf(babybuf, "%d", g_uRound);
	strncpy(s_Round.label + 17, babybuf, strlen(babybuf));

	sprintf(babybuf, "%d", g_uDigitThreshold);
	strncpy(s_DigitThreshold.label + 17, babybuf, strlen(babybuf));

}
