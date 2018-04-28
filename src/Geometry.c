//
// Created by Decimation on 4/27/2018.
//

#include <stdint.h>
#include <tice.h>
#include <debug.h>
#include "Geometry.h"
#include "GraphicsExt.h"


void geo_RoundTriangle(triangle_t* triangle, uint8_t places)
{
	//char buf[20];
	int i = 0;
	real_t* mem = (real_t*) triangle;
	for (; i < sizeof(*triangle) / sizeof(real_t); i++)
	{
		/*int24_t iPart = os_RealToInt24(&mem[i]);
		real_t iPart_r = os_Int24ToReal(iPart);
		real_t fPart = os_RealSub(&iPart_r, &mem[i]);

		os_RealToStr(buf, &iPart_r,0,0,6);
		dbg_sprintf(dbgout, "[Geo] iPart: %s | fPart: ", buf);

		os_RealToStr(buf, &fPart,0,0,6);
		dbg_sprintf(dbgout, "%s\n", buf);*/

		mem[i] = os_RealRound(&mem[i], places);
		dbg_sprintf(dbgout, "[Geo] Rounded real_t memory @ 0x%p\n", &mem[i]);
	}
}