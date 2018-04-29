//
// Created by Decimation on 4/28/2018.
//

#include <math.h>
#include "Simplifiers.h"

static const float Step = 0.1;

int IsEven(double d)
{
	double int_part;
	modf(d / 2.0, &int_part);
	return 2.0 * int_part == d;
}

double RoundIEEE754(double d)
{
	double i = floor(d);
	d -= i;
	if (d < 0.5)
		return i;
	if (d > 0.5)
		return i + 1.0;
	if (IsEven(i))
		return i;
	return i + 1.0;
}

real_t RealDecimalToRoot(real_t r)
{
	//todo
	return r;
}

float DecimalToRoot(float d)
{
	float x = 0;
	while (sqrt(x) != d)
	{
		x += Step;
		if (sqrt(x) >= d)
			return (float) RoundIEEE754(x);
	}
	return x;
}

void SimplifyRadicalFromDecimal(real_t decimal, int24_t* out)
{
	SimplifyRadical((int) DecimalToRoot(os_RealToFloat(&decimal)), out);
}

void SimplifyRadicalStrFromDecimal(real_t decimal, char* out)
{
	int24_t simp[2];
	SimplifyRadical((int) DecimalToRoot(os_RealToFloat(&decimal)), simp);
	sprintf(out, "%dsqrt(%d)", simp[0], simp[1]);
}

void SimplifyRadicalStr(int24_t insideRoot, char* out)
{
	int24_t simp[2];
	SimplifyRadical(insideRoot, simp);
	sprintf(out, "%dsqrt(%d)", simp[0], simp[1]);
}

void SimplifyRadical(int24_t insideRoot, int24_t out[2])
{
	int outside_root = 1;
	int d            = 2;
	while (d * d <= insideRoot)
	{
		if (insideRoot % (d * d) == 0)
		{
			insideRoot /= (d * d);
			outside_root *= d;
		}

		else
			d++;
	}
	out[0] = outside_root;
	out[1] = insideRoot;
}