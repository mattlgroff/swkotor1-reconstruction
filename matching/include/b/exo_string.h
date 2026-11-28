// CExoString (engine string), shared by worker B sources.
#ifndef B_EXO_STRING_H
#define B_EXO_STRING_H

#include <stddef.h>

// 0x004015A0: a bare `ret` shared by many empty functions (stripped debug
// output in release builds); called cdecl with an id argument.
void __cdecl ExoDebugNoOp(int nId);

#include "shared/exo_string.h"

#endif
