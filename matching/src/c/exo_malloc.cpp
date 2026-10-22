// _malloc thunk at 0x005E9350 (b005e3_exo_file_wrappers): a cdecl wrapper
// that compiles to a tail jump into _malloc. Name unknown.

#include <stdlib.h>

void* ExoMalloc(size_t nSize);

// FUNCTION: SWKOTOR 0x005E9350 SYMBOL: ?ExoMalloc@@YAPAXI@Z
void* ExoMalloc(size_t nSize)
{
    return malloc(nSize);
}
