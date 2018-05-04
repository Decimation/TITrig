//
// Created by Decimation on 4/28/2018.
//

#include <math.h>
#include <debug.h>
#include "Simplifiers.h"

//static const float Step = 0.1;

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

real_t DecimalToRoot(real_t r)
{
	char         buf[20];
	const real_t pow = os_Int24ToReal(2);
	real_t       root;

	os_RealToStr(buf, &r, 0, 0, 6);
	dbg_sprintf(dbgout, "%s = sqrt(", buf);

	root = os_RealPow(&r, &pow);
	os_RealToStr(buf, &root, 0, 0, 6);
	dbg_sprintf(dbgout, "%s)\n", buf);
	return root;
}

/*float DecimalToRoot(float d)
{
	float x = 0;
	while (sqrt(x) != d)
	{
		x += Step;
		if (sqrt(x) >= d)
			return (float) RoundIEEE754(x);
	}
	return x;
}*/

void SimplifyRadicalFromDecimal(real_t decimal, int24_t* out)
{
	real_t       buf;
	const real_t realZero = os_Int24ToReal(0);
	if (os_RealCompare(&decimal, &realZero) == 0)
	{
		out[0] = 1;
		out[1] = 0;
		return;
	}
	buf = DecimalToRoot(decimal);
	SimplifyRadical(os_RealToInt24(&buf), out);
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