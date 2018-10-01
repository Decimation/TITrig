//
// Created by Decimation on 4/28/2018.
//

#ifndef TITRIG_SIMPLIFIERS_H
#define TITRIG_SIMPLIFIERS_H

#include <stdint.h>
#include <stdio.h>

void sp_SimplifyRadicalFromDecimal(real_t decimal, int24_t* out);

void sp_SimplifyRadical(int24_t insideRoot, int24_t* out);

bool sp_IsEven(double d);

void sp_SimplifyFraction(real_t num, real_t denom, real_t* out);

/**
 * Converts a decimal to a square root
 * i.e. sqrt(2) = 1.414213562
 * @param d
 * @return
 */
real_t sp_DecimalToRoot(real_t r);

#endif //TITRIG_SIMPLIFIERS_H
