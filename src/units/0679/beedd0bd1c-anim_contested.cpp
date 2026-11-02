// anim_contested.cpp - lane-3d-anim: addresses listed in lane-3d-model REMAINING.json group "animation" that
// lane-3d-render SUBLANES.json assigns to the per-frame core (00437CC0, 00438AF0, 004A9B80, 004A9D30, 004A9E00).
// Authored here as one separately stageable unit; the integrator stages it or drops it in favour of a core provider
// (single-owner rule, D35). 00485560 / 00485590 are authored by lane-3d-construct / lane-3d-emitter and are not here.
#include "3775ec35d8-anim3d.h"

using namespace kotor_anim3d;


// ---- L3 MATH-TRIG-CPP t1 (2026-10-24): 004A9E00 slerp in plain C++ ----
// This unit is built as a long double (LD) unit (clang LD recipe, PLAN-v3 1.1); kotor_math.h replaces <cmath>. The
// x87-trig a1 helpers x87trig_004a9e6b/_004a9e74/_004a9eb0/_004a9ec6/_004a9ee3 and the explicit 006FC0E0 transport are
// replaced by acos(long double) / sin(long double) (kotor_math.h, bound to the local L1 dependency).
#include "../../math/kotor_math.h"
#include <intrin.h>   // _AddressOfReturnAddress (00438AF0, a2)

extern "C" {
// 00437CC0 CExoArrayList<Quaternion>::Allocate(int capacity), RET 4. New elements are identity quaternions (w = 1).
void __fastcall quaternion_list_allocate_00437cc0(PointerList* list, void*, I capacity) {
    const U old = list->data;                                                // 00437CCC
    list->capacity = capacity;                                               // 00437CD2
    unsigned char* block = static_cast<unsigned char*>(live_operator_new(static_cast<U>(capacity) << 4));   // 00437CCE..00437CD5 (006FC0D2 -> 006FA7E6)
    if (block != nullptr) {                                                  // 00437CDF..00437CE1
        for (I i = 0; i <= capacity - 1; ++i) {                              // 00437CE3..00437D02
            unsigned char* e = block + 16 * i;
            const U one = 0x3F800000u, zero = 0u;
            std::memcpy(e + 12, &zero, 4); std::memcpy(e + 8, &zero, 4);    // 00437CF3 / 00437CF6
            std::memcpy(e + 4, &zero, 4); std::memcpy(e + 0, &one, 4);      // 00437CF8 / 00437CFB
        }
    }
    list->data = static_cast<U>(reinterpret_cast<std::uintptr_t>(block));   // 00437D06..00437D08
    for (I i = 0; i < list->size; ++i)                                       // 00437D0A..00437D3B (size reloaded)
        std::memcpy(reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(list->data)) + 16 * i,
                    reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(old)) + 16 * i, 16);   // 00437D15..00437D2F
    if (old != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(old)));   // 00437D3F..00437D44 (006FC0CD -> 006FA390)
}

// 00438AF0 CExoArrayList<Quaternion>::Add(Quaternion), RET 10h.
void __fastcall quaternion_list_add_00438af0(PointerList* list, void*, Quaternion value) {
    if (list->size == list->capacity) {                                      // 00438AF3..00438AF9
        const I grow = (list->capacity != 0) ? list->capacity * 2 : 0x10;    // 00438AFB..00438B03
        quaternion_list_allocate_00437cc0(list, nullptr, grow);              // 00438B08..00438B0B
    }
    // L3 t1 a2: the original copies the by-value Quaternion with four integer moves (00438B15..00438B32) from its
    // stack argument. clang's __fastcall ABI expands the struct into four float parameters, which it loads with
    // fld dword: that quiets a signalling NaN and spilled 4-byte x87 values across the Allocate call (S1). The words
    // are read as U from the argument's own stack slots instead (the first stack argument is the word above the
    // return address, as [esp+8] after the original's push esi), so the copy is bit-exact for every input.
    (void)value;
    const U* words = reinterpret_cast<const U*>(static_cast<unsigned char*>(_AddressOfReturnAddress()) + 4);   // [esp+8]..[esp+14h]
    U* dst = reinterpret_cast<U*>(static_cast<std::uintptr_t>(list->data) + 16u * static_cast<U>(list->size));   // 00438B10..00438B1C
    dst[0] = words[0]; dst[1] = words[1]; dst[2] = words[2]; dst[3] = words[3];   // 00438B22..00438B32
    list->size = list->size + 1;                                             // 00438B35
}

// 004A9B80 quaternionScalarMult(Quaternion* out, Quaternion* q, float s) cdecl -> out.
Quaternion* __cdecl quaternion_scalar_multiply_004a9b80(Quaternion* out, const Quaternion* q, float s) {
    // L3 t1 a2: PLAN-v3 writing rule (register values long double, stores float): the four products stay on the x87
    // stack and are stored w, x, y, z as the original (no qword spill of a product).
    const long double z = static_cast<long double>(s) * q->z;                // 004A9B84..004A9B88
    const long double y = static_cast<long double>(s) * q->y;                // 004A9B8B..004A9B8F
    const long double x = static_cast<long double>(s) * q->x;                // 004A9B92..004A9B96
    const long double w = static_cast<long double>(s) * q->w;                // 004A9B99..004A9B9D
    out->w = static_cast<float>(w); out->x = static_cast<float>(x);         // 004A9BA3..004A9BA5
    out->y = static_cast<float>(y); out->z = static_cast<float>(z);         // 004A9BA8..004A9BAB
    return out;                                                              // 004A9B9F
}

// 004A9D30 quaternionDotProduct(Quaternion* a, Quaternion* b) cdecl -> ST0.
double __cdecl quaternion_dot_004a9d30(const Quaternion* a, const Quaternion* b) {
    return ((static_cast<double>(a->z) * b->z + static_cast<double>(a->y) * b->y) + static_cast<double>(a->x) * b->x)
           + static_cast<double>(a->w) * b->w;                               // 004A9D38..004A9D52
}

// 004A9E00 slerp(Quaternion* out, Quaternion from, Quaternion* to, float t) cdecl -> out.
// L3 t1: PLAN-v3 writing rule. test 5;jp == !(a < b); test 1;je continues when a < b or unordered; test 44h;jp == !(a == b).
Quaternion* __cdecl quaternion_slerp_004a9e00(Quaternion* out, Quaternion from, const Quaternion* to, float t) {
    long double cosine = quaternion_dot_004a9d30(&from, to);                // 004A9E04..004A9E0E (ST0)
    if (cosine < kZero_0073d700) {                                           // 004A9E13..004A9E21 fcom ; test 5 ; jp
        from.x = static_cast<float>(-static_cast<long double>(from.x));     // 004A9E23..004A9E47 fld ; fchs ; fstp dword
        from.y = static_cast<float>(-static_cast<long double>(from.y));
        from.z = static_cast<float>(-static_cast<long double>(from.z));
        from.w = static_cast<float>(-static_cast<long double>(from.w));
        if (cosine < kZero_0073d700) cosine = -cosine;                       // 004A9E4B..004A9E58 fcom ; test 5 ; jp ; fchs
    }
    const float c = static_cast<float>(cosine);                              // 004A9E5A fst dword [esp+4] (ST0 kept)
    if (!(cosine >= kOneDouble_0073ede8)) {                                  // 004A9E5E..004A9E69 fcomp qword 1.0 ; test 1 ; je
        const long double arc = acos(static_cast<long double>(c));          // 004A9E6B..004A9E6F fld [esp+4] ; call 006FC0E0
        const long double angle = static_cast<float>(arc);                   // 004A9E74 fst dword [esp+4] (ST0 kept)
        if (!(arc == kZero_0073d700)) {                                      // 004A9E78..004A9E83 fcomp 0 ; test 44h ; jp
            const long double sine = static_cast<float>(sin(angle));         // 004A9EAB..004A9EB6 fld ; fsin ; fstp dword [esp+10h]
            const long double angle_t = angle * t;                           // 004A9EBA..004A9EBE fld ; fmul [t]
            const long double at = static_cast<float>(angle_t);              // 004A9EC2 fst dword [esp+0Ch] (ST0 kept)
            const float s1 = static_cast<float>(sin(angle_t) / sine);        // 004A9EC6..004A9ECC fsin ; fdiv ; fstp dword
            Quaternion toward;                                               // [esp+20h]
            const Quaternion* a = quaternion_scalar_multiply_004a9b80(&toward, to, s1);   // 004A9ECF..004A9ED1
            const float s0 = static_cast<float>(sin(angle - at) / sine);     // 004A9ED6..004A9EF1 fld ; fsub ; fsin ; fdiv ; fstp
            Quaternion away;                                                 // [esp+30h]
            const Quaternion* b = quaternion_scalar_multiply_004a9b80(&away, &from, s0); // 004A9EF4..004A9EF6
            Quaternion sum;                                                  // [esp+10h]
            const Quaternion* r = quaternion_add_004a9d00(&sum, b, a);       // 004A9EFE..004A9F04
            std::memcpy(out, r, sizeof *out);                                // 004A9F09..004A9F2A (integer moves)
            return out;
        }
    }
    std::memcpy(out, &from, sizeof *out);                                    // 004A9E85..004A9EA3 (integer moves)
    return out;
}

} // extern "C"
