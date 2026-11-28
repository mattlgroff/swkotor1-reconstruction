// float_to_int.h -- the engine's float-to-int rounding helper
//
// FloatToInt rounds with the FPU's current rounding mode (fld, frndint,
// fistp through the output pointer). The same inlined sequence appears in
// functions across the engine, so it is modelled as one inline-assembly
// helper of the original (CODE_RULES.md, R18: listed helpers).
#ifndef SHARED_FLOAT_TO_INT_H
#define SHARED_FLOAT_TO_INT_H

static inline void FloatToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

#endif
