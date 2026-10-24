#include "3f9b90578d96-minigame_inertia.hpp"
#include "da6c2cf73dcd-minigame_player_storage.hpp"
#include <cstdint>
#include <cstring>

// Extracted control flow from source-only pi-three-bodies DoInertia donor.
// Original local arithmetic uses x87 helpers; selected providers are reused.
extern "C" void* __fastcall kotor_fastcall_00670bb0(void*, void*, void*, void*);
extern "C" void* __cdecl vector_divide_00437a90(void*, const void*, float);
extern "C" void* __fastcall vector_subtract_assign_004a9770(void*, void*, const void*);
extern "C" void* __cdecl quaternion_negate_w_004a9b50(void*, const void*);
extern "C" void* __cdecl quaternion_rotate_004a9bb0(void*, const void*, const void*);

namespace {
template<class T> T load(const void* base, unsigned offset) {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, sizeof v); return v;
}
template<class T> void store(void* base, unsigned offset, T v) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &v, sizeof v);
}
__declspec(noinline) void difference(float* out, const void* a, const void* b) {
    __asm {
        mov eax, a
        mov ecx, b
        mov edx, out
        fld dword ptr [eax]
        fsub dword ptr [ecx]
        fstp dword ptr [edx]
        fld dword ptr [eax+4]
        fsub dword ptr [ecx+4]
        fstp dword ptr [edx+4]
        fld dword ptr [eax+8]
        fsub dword ptr [ecx+8]
        fstp dword ptr [edx+8]
    }
}
__declspec(noinline) void projection(float* out, const float* direction, const float* delta) {
    __asm {
        mov eax, direction
        mov ecx, delta
        mov edx, out
        fld dword ptr [eax+8]
        fmul dword ptr [ecx+8]
        fld dword ptr [eax+4]
        fmul dword ptr [ecx+4]
        faddp st(1), st(0)
        fld dword ptr [ecx]
        fmul dword ptr [eax]
        faddp st(1), st(0)
        fld dword ptr [eax]
        fmul st(0), st(1)
        fstp dword ptr [edx]
        fld dword ptr [eax+4]
        fmul st(0), st(1)
        fstp dword ptr [edx+4]
        fld dword ptr [eax+8]
        fmul st(0), st(1)
        fstp dword ptr [edx+8]
        fstp st(0)
    }
}
__declspec(noinline) void scale_result(void* out, const void* rotated, const float* scale) {
    __asm {
        mov eax, rotated
        mov ecx, scale
        mov edx, out
        fld dword ptr [ecx]
        fmul dword ptr [eax+8]
        fld dword ptr [ecx]
        fmul dword ptr [eax+4]
        fld dword ptr [ecx]
        fmul dword ptr [eax]
        fstp dword ptr [edx]
        fstp dword ptr [edx+4]
        fstp dword ptr [edx+8]
    }
}
}

extern "C" void* __fastcall kotor_fastcall_0066cd30(
    void* self, void*, void* out, const void* previous_position,
    const void* position, const void* quaternion, float dt) {
    float direction[3];
    kotor_fastcall_00670bb0(load<void*>(self, 0x60), nullptr, direction,
                          const_cast<void*>(quaternion));
    float diff[3];
    difference(diff, position, previous_position);
    float scratch[4];
    vector_divide_00437a90(scratch, diff, dt);
    float delta[3];
    difference(delta, static_cast<unsigned char*>(self) + 0x228, scratch);
    float projected[3];
    projection(projected, direction, delta);
    vector_subtract_assign_004a9770(delta, nullptr, projected);
    store(self, 0x228, load<std::uint32_t>(scratch, 0));
    store(self, 0x22C, load<std::uint32_t>(scratch, 4));
    store(self, 0x230, load<std::uint32_t>(scratch, 8));
    // One actual cell read at the original sequence point, before negation.
    const float scale = *static_cast<volatile float*>(&g_mini_007a2488);
    const void* const inverse = quaternion_negate_w_004a9b50(scratch, quaternion);
    float rotation_storage[3];
    const void* const rotated = quaternion_rotate_004a9bb0(rotation_storage, inverse, delta);
    scale_result(out, rotated, &scale);
    return out;
}
