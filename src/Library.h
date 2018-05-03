//
// Created by Decimation on 4/27/2018.
//

#ifndef TITRIG_LIBRARY_H
#define TITRIG_LIBRARY_H

#include <stddef.h>
#include <tice.h>

void lib_MemZero(char* ptr, int c);

void lib_StrReplace(char* in, char c, char r);

int lib_StrIndexOf(char* values, char find);

int lib_StrCut(char* str, int begin, int len);


#define Wait() while (!os_GetCSC());
extern const char char_Neg;

#endif //TITRIG_LIBRARY_H
