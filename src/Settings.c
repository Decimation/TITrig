//
// Created by Decimation on 4/28/2018.
//

#include <stdint.h>
#include <graphx.h>
#include <stdio.h>
#include <string.h>
#include "Settings.h"

#include "LabelPoint/LabelPoint.h"

uint8_t g_round = 0;


/**
 * @brief Max rounding places and max digits displayed
 */
const uint8_t g_digitThreshold = 9;