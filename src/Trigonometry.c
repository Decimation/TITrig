//
// Created by Decimation on 4/27/2018.
//


#include "Trigonometry.h"
#include <tice.h>
#include <debug.h>
#include <math.h>

static const float PI = 3.141592654f;

float CosineDeg(float f)
{
	return (float) (cos(f / 180 * PI));
}

float ASinDeg(float f)
{
	return (float) (asin(f) * 180 / PI);
}

float SinDeg(float f)
{
	return (float) (sin(f / 180 * PI));
}

real_t os_RealAcosDeg(real_t r)
{
	real_t rbuf;

	rbuf = os_FloatToReal((float) acos(os_RealToFloat(&r)));
	rbuf = os_FloatToReal(os_RealToFloat(&rbuf) * 180.0f / PI);
	return os_RealRound(&rbuf, 1);
}

real_t loc_Angle_x(real_t c, real_t a, real_t b)
{
	char   buf[20];
	//cos B = (c2 + a2 − b2) / 2ca
	real_t real2 = os_Int24ToReal(2);
	real_t csq, asq, bsq, res, term, term2, rbuf;

	os_RealToStr(buf, &c, 0, 0, 6);
	dbg_sprintf(dbgout, "c = %s\n", buf);
	os_RealToStr(buf, &a, 0, 0, 6);
	dbg_sprintf(dbgout, "a = %s\n", buf);
	os_RealToStr(buf, &b, 0, 0, 6);
	dbg_sprintf(dbgout, "b = %s\n", buf);

	bsq  = os_RealMul(&b, &b);
	csq  = os_RealMul(&c, &c);
	asq  = os_RealMul(&a, &a);
	term = os_RealAdd(&csq, &asq);
	term = os_RealSub(&term, &bsq);

	term2 = os_RealMul(&c, &a);
	term2 = os_RealMul(&real2, &term2);

	res = os_RealDiv(&term, &term2);
	os_RealToStr(buf, &res, 0, 0, 6);

	rbuf = os_RealAcosDeg(res);
	os_RealToStr(buf, &rbuf, 0, 0, 6);

	return rbuf;
}

real_t los_Side_x(real_t X, real_t y, real_t Y)
{
	//a = (sin(A) * c) / sin(C);
	float fbuf = (os_RealToFloat(&y) * SinDeg(os_RealToFloat(&X))) / SinDeg(os_RealToFloat(&Y));
	return os_FloatToReal(fbuf);
}

real_t los_Angle_x(real_t x, real_t Y, real_t y)
{
	// A = asin((a * sin(B) / b));
	// X = asin((x * sin(Y) / y))

	float xf, Yf, yf, sol;
	xf  = os_RealToFloat(&x);
	Yf  = os_RealToFloat(&Y);
	yf  = os_RealToFloat(&y);
	sol = ASinDeg((SinDeg(Yf) * xf) / yf);
	return os_FloatToReal(sol);
}

real_t loc_Side_x(real_t a, real_t b, real_t angle)
{
	//c = sqrt(a^2 + b^2 - 2*a*b*cos(c))
	float af, bf, anglef;
	af     = os_RealToFloat(&a);
	bf     = os_RealToFloat(&b);
	anglef = os_RealToFloat(&angle);
	return os_FloatToReal(sqrt((af * af) + (bf * bf) - 2 * af * bf * CosineDeg(anglef)));
}



real_t os_RealSinDeg(real_t r)
{
	const real_t real180 = os_Int24ToReal(180);
	const real_t realpi  = os_FloatToReal(PI);
	real_t       buf     = os_RealDiv(&r, &real180);
	real_t       buf2    = os_RealMul(&buf, &realpi);
	return os_RealSinRad(&buf2);
}