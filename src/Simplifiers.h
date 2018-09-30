//
// Created by Decimation on 4/28/2018.
//

#ifndef TITRIG_SIMPLIFIERS_H
#define TITRIG_SIMPLIFIERS_H

#include <stdint.h>
#include <stdio.h>

void SimplifyRadicalFromDecimal(real_t decimal, int24_t* out);

void SimplifyRadical(int24_t insideRoot, int24_t out[2]);

int IsEven(double d);

void SimplifyFraction(real_t num, real_t denom, real_t out[2]);

/**
 * Converts a decimal to a square root
 * i.e. sqrt(2) = 1.414213562
 * @param d
 * @return
 */
real_t DecimalToRoot(real_t r);

float DecimalToPi(float f);

#endif //TITRIG_SIMPLIFIERS_H
