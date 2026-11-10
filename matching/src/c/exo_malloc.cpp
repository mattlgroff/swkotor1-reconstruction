// _malloc / _free thunks at 0x005E9350 and 0x005E9360 (b005e3_exo_file_wrappers):
// cdecl wrappers that compile to tail jumps into the CRT. Names descriptive.
// 0x005E9360 is missing from functions.csv (called by CExoKeyTable code).

#include <stdlib.h>

void* ExoMalloc(size_t nSize);
void ExoFree(void* pMemory);

// FUNCTION: SWKOTOR 0x005E9350 SYMBOL: ?ExoMalloc@@YAPAXI@Z
void* ExoMalloc(size_t nSize)
{
    return malloc(nSize);
}

// FUNCTION: SWKOTOR 0x005E9360 SYMBOL: ?ExoFree@@YAXPAX@Z
void ExoFree(void* pMemory)
{
    free(pMemory);
}
