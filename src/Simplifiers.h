//
// Created by Decimation on 4/28/2018.
//

#ifndef TITRIG_SIMPLIFIERS_H
#define TITRIG_SIMPLIFIERS_H

#include <stdint.h>
#include <stdio.h>

void SimplifyRadicalStrFromDecimal(real_t decimal, char* out);

void SimplifyRadicalFromDecimal(real_t decimal, int24_t* out);

void SimplifyRadicalStr(int24_t insideRoot, char* out);

void SimplifyRadical(int24_t insideRoot, int24_t out[2]);

int IsEven(double d);


double RoundIEEE754(double d);

/**
 * Converts a decimal to a square root
 * i.e. sqrt(2) = 1.414213562
 * @param d
 * @return
 */
float DecimalToRoot(float d);

real_t RealDecimalToRoot(real_t r);

float DecimalToPi(float f);

#endif //TITRIG_SIMPLIFIERS_H
