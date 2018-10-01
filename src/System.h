//
// Created by Decimation on 4/27/2018.
//


#ifndef TITRIG_LIBRARY_H
#define TITRIG_LIBRARY_H

#include "Types.h"
#include <stddef.h>
#include <string.h>
#include <tice.h>
#include <assert.h>

/**
 * Macro that generates a const real_t named "real##i" inline at the current line number.
 */
#define LocalReal(i) const real_t real##i = os_Int24ToReal(i)

#define CountOf(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define THROW(Msg)                                \
    dbg_sprintf(dbgout,"THROW [%s]", Msg);        \
    abort();                                      \



void sys_WriteBuffer(char* buffer, real_t* value);

void sys_MemZero(char* ptr, int c);

void syc_StrReplace(char* in, char c, char r);

int sys_StrIndexOf(char* values, char find);

int sys_StrCut(char* str, int begin, int len);

#define Wait() while (!os_GetCSC());

extern const char char_Neg;

#endif //TITRIG_LIBRARY_H
