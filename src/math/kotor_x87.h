#pragma once
inline __declspec(noinline) void FloatToInt(int* dst, float f) {
    __asm {
        fld f
        mov edx, dst
        frndint
        fistp dword ptr [edx]
    }
}
