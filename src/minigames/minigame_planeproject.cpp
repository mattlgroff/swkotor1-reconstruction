#include "d69b5cbffe54-minigame_planeproject.hpp"

// Original 004A9850, cdecl (hidden Vec3 result, normal, distance, point).
// This has no calls. Keep the original x87 order and its asymmetric scratch
// widths: X/Y products store float before subtraction, Z remains on the stack.
// No FP control/state is forced. All input reads precede the output writes.
extern "C" __declspec(noinline) void* __cdecl kotor_cdecl_004a9850(
    void* result, const void* normal, float distance, const void* point) {
    float scaled_x;
    float scaled_y;
    __asm {
        mov eax, normal
        mov ecx, point
        fld dword ptr [eax + 4]
        fmul dword ptr [ecx + 4]
        fld dword ptr [eax]
        fmul dword ptr [ecx]
        faddp st(1), st(0)
        fld dword ptr [ecx + 8]
        fmul dword ptr [eax + 8]
        faddp st(1), st(0)
        fadd distance
        fld st(0)
        fmul dword ptr [eax]
        fstp scaled_x
        fld st(0)
        fmul dword ptr [eax + 4]
        fstp scaled_y
        fmul dword ptr [eax + 8]
        fsubr dword ptr [ecx + 8]
        fld dword ptr [ecx + 4]
        fsub scaled_y
        fld dword ptr [ecx]
        fsub scaled_x
        mov edx, result
        fstp dword ptr [edx]
        fstp dword ptr [edx + 4]
        fstp dword ptr [edx + 8]
    }
    return result;
}
