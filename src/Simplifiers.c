//
// Created by Decimation on 4/28/2018.
//

#include <math.h>
#include <debug.h>
#include "Simplifiers.h"
#include "Library.h"

int IsEven(double d)
{
	double int_part;
	modf(d / 2.0, &int_part);
	return 2.0 * int_part == d;
}

real_t DecimalToRoot(real_t r)
{
	char         buf[20];
	LocalReal(2);
	real_t       root;

	os_RealToStr(buf, &r, 0, 0, 6);
	dbg_sprintf(dbgout, "%s = sqrt(", buf);

	root = os_RealPow(&r, &real2);
	os_RealToStr(buf, &root, 0, 0, 6);
	dbg_sprintf(dbgout, "%s)\n", buf);
	return root;
}

void SimplifyFraction(real_t num, real_t denom, real_t out[2])
{
	real_t gcd = os_RealGcd(&num, &denom);
	out[0] = os_RealDiv(&num, &gcd);
	out[1] = os_RealDiv(&denom, &gcd);
}

void SimplifyRadicalFromDecimal(real_t decimal, int24_t* out)
{
	char   cbuf[20];
	real_t buf;
	LocalReal(0);

	if (os_RealCompare(&decimal, &real0) == 0) {
		out[0] = 1;
		out[1] = 0;
		return;
	}

	lib_WriteBuffer(cbuf, &decimal);
	buf = DecimalToRoot(decimal);
	SimplifyRadical(os_RealToInt24(&buf), out);
}

void SimplifyRadical(int24_t insideRoot, int24_t out[2])
{
	int outside_root = 1;
	int d            = 2;
	while (d * d <= insideRoot) {
		if (insideRoot % (d * d) == 0) {
			insideRoot /= (d * d);
			outside_root *= d;
		}

		else
			d++;
	}
	out[0] = outside_root;
	out[1] = insideRoot;
}