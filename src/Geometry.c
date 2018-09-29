//
// Created by Decimation on 4/27/2018.
//

#include <stdint.h>
#include <tice.h>
#include <debug.h>
#include "Geometry.h"

bool g_bEnableRounding;

void geo_RoundTriangle(triangle_t* triangle, uint8_t places)
{
	int i = 0;
	real_t* const mem = (real_t*) triangle;
	if (!g_bEnableRounding) {
		return;
	}
	for (; i < sizeof(*triangle) / sizeof(real_t); i++)
	{
		mem[i] = os_RealRound(&mem[i], places);
		dbg_sprintf(dbgout, "[Geo] Rounded real_t memory @ 0x%p\n", &mem[i]);
	}
}