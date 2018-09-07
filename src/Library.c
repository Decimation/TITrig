//
// Created by Decimation on 4/27/2018.
//

#include <debug.h>
#include <string.h>
#include "Library.h"

const char char_Neg = (char) 26;

int lib_StrCut(char* str, int begin, int len)
{
	int l = strlen(str);

	if (len < 0) len         = l - begin;
	if (begin + len > l) len = l - begin;
	memmove(str + begin, str + begin + len, l - len + 1);

	return len;
}

void os_RealToStrAuto(char* buf, real_t* p) {
	os_RealToStr(buf,p,0,0,6);
}
void dbg_WeakAssert(bool cond) {
	if (!cond) {
		dbg_sprintf(dbgout, "[WARNING] Debug assertion failed\n");
	}
}

int lib_StrIndexOf(char* values, char find)
{
	int index;
	const char* ptr = strchr(values, find);
	if (ptr)
	{
		index = (int) (ptr - values);
	}
	else index = -1;
	return index;
}

void lib_StrReplace(char* in, char c, char r)
{
	const int index = lib_StrIndexOf(in, c);

	if (index != -1) in[index] = r;
}

void lib_MemZero(char* ptr, int c)
{
	int i = 0;
	dbg_sprintf(dbgout, "[lib_MemZero] Zeroing memory from 0x%p to 0x%p\n", (void*) ptr, &ptr + c);
	for (; i < c; i++)
	{
		ptr[i] = (char) NULL;
	}
}