// Lane W4 G13: client creature animation drive (AnimationUpdate, AnimateFireAndForget, Drive/Follow animation
// update, Move/Rotate) and the CSWCObject animation predicates / fire-and-forget queue they use.
// x87: every original FPU-stack intermediate is a double; every `fstp dword` an explicit (float).
#include "a5e2cda8f5-creature.hpp"
// L3 MATH-TRIG-CPP t1 (2026-10-24): built as a long double (LD) unit (recipe + /clang:-mno-sse /clang:-ffp-model=strict
// /clang:-mlong-double-80). 00611850 RotateCreatureToOrientation is written in the PLAN-v3 writing rule; math through
// kotor_math.h (no <cmath>). The other functions keep their double-rule source; under the LD flags their FP code is x87.
#include "../../math/kotor_math.h"

using namespace w4;
using namespace w4c;

namespace {
U8* B(void* p) { return static_cast<U8*>(p); }
W4Vector* V(void* p, U32 off) { return reinterpret_cast<W4Vector*>(B(p) + off); }
void* gso(void* c) { return kotor_m15a_object_get_server_object_0063d4b0(c, nullptr); }
void* screature(void* c) { void* s = gso(c); return s ? vcall<void*>(gso(c), 0x30) : nullptr; }   // GSO ? GSO->+30h : 0
void set_animation(void* c, U32 anim, U32 a, U32 b) { vcall<void, U32, U32, U32>(c, 0x9C, anim, a, b); }
void add_fire_forget(void* c, U32 anim, U32 a) { vcall<void, U32, U32>(c, 0xA0, anim, a); }
// L3 t1 a2 (writing rule): field access in the FP functions. The recipe is /Ob0, so the w4_native.hpp accessors are
// real calls (a float result held across one is a 4-byte x87 spill); `at` is __forceinline (honoured under /Ob0), so a
// field is a direct memory operand as in the original.
template <class T> __forceinline T& at(void* o, U32 off) { return *reinterpret_cast<T*>(static_cast<U8*>(o) + off); }
// A float whose bits the original copies with integer moves (mov to a stack slot) and later reads as an x87 memory
// operand: kept as its U32 bits, widened only at the use.
__forceinline long double fbits(U32 bits) { float f; std::memcpy(&f, &bits, 4); return f; }
// 0063C0E0 CSWCObject::GetAnimationLength (RET 4) leaves its result in ST0 (1.0f, or the anim base's +38h virtual).
// creature.hpp declares it `float`; it is called through its real return register as `double` (replaces the inline-asm
// helper w4c::anim_length_st0 of w4-creature_ai.obj, a CALL inside __asm); every use multiplies it at once.
__forceinline double anim_length_0063c0e0(void* o, U32 anim) {
    return reinterpret_cast<double(__fastcall*)(void*, void*, U32)>(reinterpret_cast<void*>(&kotor_fastcall_0063c0e0))(o, nullptr, anim);
}
// 2DA predicate shape shared by AnimationPause/Overlay/Attack/FireAndForget: anim base slot +E0h translates the
// id, then animations.2da ([[rules]+B8h]+3Ch) column `column` must be non-zero.
U32 anim_table_flag(void* o, U32 anim_arg, U32 column) {
    int value = 0;
    void* base = lp(o, 0x68);
    const U32 anim = base ? vcall<U32, U32>(base, 0xE0, anim_arg) : anim_arg;               // +E0h or arg
    if (c2da_current_get_int_entry_00413510(lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x3C), nullptr,
                                            static_cast<std::uint16_t>(anim), static_cast<int>(column), &value) == 0)
        return 0;
    return value != 0 ? 1 : 0;
}
}

// 0063C1E0 / 0063C410 / 0063C550 CSWCObject::AnimationPause / AnimationOverlay / AnimationAttack (RET 4).
extern "C" U32 __fastcall kotor_fastcall_0063c1e0(void* o, void*, U32 anim) { return anim_table_flag(o, anim, kotor_rules_cell32_007a215c); }
extern "C" U32 __fastcall kotor_fastcall_0063c410(void* o, void*, U32 anim) { return anim_table_flag(o, anim, kotor_rules_cell32_007a2150); }
extern "C" U32 __fastcall kotor_fastcall_0063c550(void* o, void*, U32 anim) { return anim_table_flag(o, anim, kotor_rules_cell32_007a217c); }
// 0063C380 CSWCObject::AnimationFireAndForget(anim) (RET 4).
extern "C" U32 __fastcall kotor_fastcall_0063c380(void* o, void*, U32 anim) {
    const U8 type = lb(o, 8);                                                                // 0063C380
    const std::uint16_t a = static_cast<std::uint16_t>(anim);
    if ((type == 5 || type == 9) &&
        ((a >= 0x3E8 && a <= 0x467) || (a >= 0x4B0 && a <= 0x52F))) return 1;               // 0063C383..0063C3AE
    return anim_table_flag(o, anim, kotor_rules_cell32_007a214c);                            // 0063C3B1..0063C401
}
// 0063C470 CSWCObject::AnimationPlayOutOfPlace(anim) (RET 4).
extern "C" U32 __fastcall kotor_fastcall_0063c470(void*, void*, U32 anim) {
    const std::uint16_t a = static_cast<std::uint16_t>(anim);
    return ((a >= 0x4B0 && a <= 0x52F) || (a >= 0x640 && a <= 0x6BF)) ? 1 : 0;               // 0063C470..0063C497
}

// 004AE570 CWorldTimer::GetTimeDifferenceFromWorldTime(day, tick) (thiscall, RET 8).
extern "C" U32 __fastcall kotor_fastcall_004ae570(void* timer, void*, U32 day, U32 tick) {
    U32 now_day = 0, now_tick = 0;                                                           // [esp+0Ch], [esp+8]
    frame_GetWorldTime_004ade40(timer, nullptr, &now_day, &now_tick);                       // 004AE577..004AE583
    const U32 per_day = lw(timer, 0x3C);                                                     // 004AE588
    if (now_tick < per_day && tick < per_day) {                                              // 004AE58F..004AE599
        if (now_day == day) { if (now_tick < tick) return 0; }                               // 004AE5A3..004AE5AD
        else if (now_day <= day) return 0;                                                   // 004AE5B7
    }
    const U32 diff = now_tick - tick;                                                        // 004AE5B9
    return diff < per_day ? diff : diff + per_day;                                           // 004AE5BB..004AE5C0
}

// 0063C610 CSWCObject::GetTopFireForgetAnimation(anim*, speed*, duration*, data*) (thiscall, RET 10h).
extern "C" U32 __fastcall kotor_fastcall_0063c610(void* o, void*, std::uint16_t* anim, float* speed, U32* duration, U32* data) {
    void* queue = lp(o, 0x5C);                                                               // 0063C618
    const U32 qday = lw(queue, 8), qtick = lw(queue, 0xC);                                   // 0063C61B..0063C61F
    const U32 elapsed = kotor_fastcall_004ae570(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, qday, qtick); // 0063C622..0063C633
    auto defaults = [&](U32 data_value) {                                                    // 0063C642..0063C66C / 0063C6BD..0063C6E3
        *anim = 0x2710; *data = data_value; *speed = 1.0f; *duration = 0x1F4; return 0u;
    };
    if (lw(lp(o, 0x5C), 0) == 0) return defaults(0);                                         // 0063C638..0063C640
    if (elapsed == 0) {                                                                      // 0063C675..0063C677
        U32 d = 0, t = 0;                                                                    // [esp+14h], [esp+10h]
        frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &d, &t); // 0063C679..0063C692
        if (static_cast<I32>(recovered_event_time_compare_004adfa0(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr,
                                                                   d, t, qday, qtick)) == -1) // 0063C697..0063C6BB
            return defaults(elapsed);
    }
    void* q = lp(o, 0x5C);
    *anim = lh(q, 4);                                                                        // 0063C6EC..0063C6F7
    *data = lw(lp(o, 0x5C), 0x18);                                                           // 0063C6FA..0063C704
    const U32 total = lw(lp(o, 0x5C), 0x10);                                                 // 0063C706..0063C709
    I32 remaining = static_cast<I32>(total - elapsed);                                       // 0063C70C..0063C70E
    if (remaining < 0x32) remaining = 0x32;                                                  // 0063C710..0063C71E
    *duration = static_cast<U32>(remaining);                                                 // 0063C730
    // 0063C72C..0063C753: fild total (+2^32 if negative: the unsigned value) ; fmul dword [q+14h] ; fild remaining
    // (signed) ; fdivp ; fstp dword [speed].
    *speed = static_cast<float>(static_cast<long double>(total) * at<float>(lp(o, 0x5C), 0x14) /
                                static_cast<long double>(remaining));
    return 1;
}

// 0063C9C0 CSWCObject::RemoveTopFireForgetAnimation() (thiscall, RET 0). Entries are 1Ch bytes.
extern "C" void __fastcall kotor_fastcall_0063c9c0(void* o, void*) {
    for (U32 i = 1; i < lb(o, 0x60); ++i) {                                                  // 0063C9C0..0063CA33
        U8* dst = B(lp(o, 0x5C)) + (i - 1) * 0x1C;
        U8* src = B(lp(o, 0x5C)) + i * 0x1C;
        std::memcpy(dst + 0x00, src + 0x00, 4);
        std::memcpy(dst + 0x04, src + 0x04, 2);                                              // word copy only
        for (U32 f = 0x08; f <= 0x18; f += 4) std::memcpy(dst + f, src + f, 4);
    }
    const U8 count = lb(o, 0x60);                                                            // 0063CA36
    if (count < 1) return;                                                                   // 0063CA39..0063CA3B
    U8* last = B(lp(o, 0x5C)) + (count - 1u) * 0x1C;                                         // 0063CA3D..0063CAA7
    sw(last, 0x00, 0);
    const std::uint16_t idle = 0x2710; std::memcpy(last + 4, &idle, 2);
    sw(last, 0x08, 0); sw(last, 0x0C, 0); sw(last, 0x10, 0x1F4); sw(last, 0x14, 0x3F800000u); sw(last, 0x18, 0); // 0063CA95 mov dword, 3F800000h (1.0f)
    sb(o, 0x60, static_cast<U8>(count - 1));                                                 // 0063CAAB
}

// 0060B3C0 CSWCCreature::setStationaryOrentation(Vector*) (RET 4): +1D0h = normalize(dir).
extern "C" void __fastcall kotor_fastcall_0060b3c0(void* c, void*, W4Vector* direction) {
    W4Vector n;
    const W4Vector* r = vector_normalized_004ab180(&n, direction);                           // 0060B3CA..0060B3D0
    std::memcpy(B(c) + 0x1D0, r, 12);                                                        // 0060B3D5..0060B3EB
}

// 0060BA10 CSWCCreature::SetDesiredOrientationForLock() (RET 0).
extern "C" void __fastcall kotor_fastcall_0060ba10(void* c, void*) {
    void* array = kotor_fastcall_005ed570(client_app(), nullptr);                            // 0060BA10..0060BA1E
    if (array == nullptr) return;                                                            // 0060BA23..0060BA25
    void* target = nullptr;                                                                  // [esp+4]
    if (kotor_fastcall_004d8230(array, nullptr, lw(c, 0x2F4), &target) != 0) {               // 0060BA2B..0060BA44 (byte [0074666C] = 0)
        vcall<void, W4Vector*>(c, 0x88, V(c, 0x30));                                         // 0060BAB6..0060BABE
        return;
    }
    if (target == nullptr) return;                                                           // 0060BA46..0060BA4C
    W4Vector d;                                                                              // [esp+8]
    // L3 t1 a2 (writing rule): d.x / d.y are the original's fstp dword [esp+8] / [esp+0Ch], reloaded as fmul operands.
    // Each is one float conversion (the original store); the float is then copied into d unchanged.
    const float dx = static_cast<float>(static_cast<long double>(at<float>(target, 0x24)) - at<float>(c, 0x24)); // 0060BA4E..0060BA54
    const float dy = static_cast<float>(static_cast<long double>(at<float>(target, 0x28)) - at<float>(c, 0x28)); // 0060BA58..0060BA5E
    const long double dz = static_cast<long double>(at<float>(target, 0x2C)) - at<float>(c, 0x2C);               // 0060BA62..0060BA65 (ST)
    const long double sum = (static_cast<long double>(dx) * dx + static_cast<long double>(dy) * dy) + dz * dz;    // 0060BA68..0060BA7E
    if (!(sum > 1e-5f)) return;                                                              // 0060BA80..0060BA8D (test 41h ; jne)
    d.x = dx;
    d.y = dy;
    d.z = 0.0f;                                                                              // 0060BA99
    W4Vector n;
    kotor_fastcall_0060b3c0(c, nullptr, vector_normalized_004ab180(&n, &d));                 // 0060BA8F..0060BAAC
}

// 0060BDF0 CSWCCreature::MoveCreatureToPosition(Vector* target) (RET 4).
extern "C" void __fastcall kotor_fastcall_0060bdf0(void* c, void*, W4Vector* target) {
    const U32 remaining = lw(c, 0x1E8), delta = lw(c, 0x44);                                 // 0060BDF0..0060BDFA
    bool snap = true;
    if (delta < remaining) {                                                                 // 0060BDFD..0060BDFF
        const U32 left = remaining - delta;                                                  // 0060BE01
        sw(c, 0x1E8, left);                                                                  // 0060BE03
        // L3 t1 a2 (writing rule): register values long double; each fstp dword an explicit (float).
        const long double dx = static_cast<long double>(at<float>(c, 0x1DC)) - at<float>(c, 0x24);   // 0060BE09..0060BE0F
        const long double dy = static_cast<long double>(at<float>(c, 0x1E0)) - at<float>(c, 0x28);   // 0060BE12..0060BE18
        const long double dzf = static_cast<float>(static_cast<long double>(at<float>(c, 0x1E4)) - at<float>(c, 0x2C)); // 0060BE1B..0060BE24 fstp [esp+1Ch]
        const long double dyy = static_cast<float>(dy * dy);                                 // 0060BE28..0060BE2C fstp [esp+4]
        const long double dxx = dx * dx;                                                     // 0060BE32..0060BE34
        const long double sum = (dzf * dzf + dxx) + dyy;                                     // 0060BE36..0060BE40
        // 0060BE44..0060BE4F fcomp 1e-5 ; test 5 ; jp (snap when sum < 1e-5) ; 0060BE71..0060BE84 fld dyy ; fadd dxx ;
        // fcomp 36 ; test 1 ; je (snap when >= 36).
        if (!(sum < 1e-5f) && !(dyy + dxx >= 36.0f)) {
            snap = false;
            const long double frac = static_cast<long double>(left) / static_cast<long double>(left + delta); // 0060BE86..0060BEAC
            const long double ax = static_cast<long double>(at<float>(c, 0x24)) - target->x;  // 0060BEB7..0060BEBA
            const long double ay = static_cast<long double>(at<float>(c, 0x28)) - target->y;  // 0060BEBC..0060BEBF
            const long double azf = static_cast<float>(static_cast<long double>(at<float>(c, 0x2C)) - target->z); // 0060BEC2..0060BEC8
            const long double xf = static_cast<float>(ax * frac);                            // 0060BECC..0060BED0
            const long double yfrac = ay * frac;                                             // 0060BED4
            const long double zf = static_cast<float>(azf * frac);                           // 0060BED6..0060BEDC
            W4Vector out;                                                                    // [esp+14h]
            out.x = static_cast<float>(xf + target->x);                                      // 0060BEE0..0060BEE6
            out.y = static_cast<float>(yfrac + target->y);                                   // 0060BEEA..0060BEED
            out.z = static_cast<float>(zf + target->z);                                      // 0060BEF3..0060BEFC
            vcall<void, W4Vector*>(c, 0x8C, &out);                                           // 0060BEFA..0060BF00 SetPosition
        }
    }
    if (snap) {
        sw(c, 0x1E8, 0);                                                                     // 0060BE5A
        vcall<void, W4Vector*>(c, 0x8C, target);                                             // 0060BE53..0060BE64
    }
}

// ---- L3 t1 (2026-10-24): 00611850 in plain C++ (replaces the x87-trig a2 helpers x87trig_00611e24 / x87trig_00611e52) ----
namespace {
// (field access: `at`, defined at the top of the file)
// 004AA0F0 Yaw(Vector*) leaves its fmul result in ST0 (a PC-53 value: fpatan * [00742EF8]). creature.hpp declares it
// `float`, which would let the compiler treat the unrounded ST0 as an exact float; it is called here through its real
// return width (double), so an explicit (float) is the original's fstp dword and an arithmetic use sees the full value.
__forceinline double yaw_004aa0f0(const void* v) {
    return reinterpret_cast<double(__cdecl*)(const void*)>(reinterpret_cast<void*>(&kotor_m15a_yaw_004aa0f0))(v);
}
}  // namespace

// 00611850 CSWCCreature::RotateCreatureToOrientation(Vector* desired) (RET 4).
// L3 t1: PLAN-v3 writing rule. Values the original keeps on the x87 stack are long double (the turn angle, both
// two-live 64-bit cos/sin results of 00611E52/00611E56, the yaw difference); each fstp/fst dword is an explicit
// (float); comparisons are spelled from the fnstsw masks (ST0 against the operand: test 41h;jne == !(a > b),
// test 5;jp == !(a < b), test 1;jne == !(a >= b), test 41h;jp == !(a <= b), test 41h;je == a > b).
extern "C" void __fastcall kotor_fastcall_00611850(void* c, void*, W4Vector* desired) {
    if (gso(c) != nullptr && vcall<void*>(gso(c), 0x30) != nullptr) {                        // 00611856..00611873
        if (kotor_fastcall_005b4880(screature(c), nullptr) != 0) return;                     // 00611879..0061189F IsDebilitated
        if (vcall<U32>(screature(c), 0x94) != 0) return;                                     // 006118A5..006118CE
        if (recovered_creature_incapacitated_004ef890(screature(c), nullptr) != 0) return;   // 006118D4..006118FA GetDeadTemp
    }
    if (gso(c) == nullptr || vcall<void*>(gso(c), 0x30) == nullptr) return;                  // 00611900..0061191F
    bool combat_turn = false;
    if (gso(c) != nullptr && vcall<void*>(gso(c), 0x30) != nullptr && lw(screature(c), 0x4E0) != 0) { // 00611926..00611970
        if (kotor_fastcall_005188e0(kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr), nullptr, 1) == 0) { // 00611976..00611988
            bool hostile_a = false;
            if (lw(kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr), 0x50C) != 0x7F000000u) { // 0061198E..006119A2
                const U32 target = lw(kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr), 0x50C);
                hostile_a = recovered_object_get_reputation_0057cb80(kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr), nullptr, target, 0) <= 10; // 006119A4..006119C5
            }
            if (hostile_a) combat_turn = true;
            else if (lw(kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr), 0x524) != 0x7F000000u) { // 006119C7..006119D4
                const U32 target = lw(kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr), 0x524);
                combat_turn = recovered_object_get_reputation_0057cb80(kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr), nullptr, target, 0) <= 10; // 006119DA..006119FB
            }
        }
    }
    if (combat_turn) {
        void* sc = kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr);             // 00611A01..00611A08
        W4Vector current; std::memcpy(&current, B(sc) + 0x9C, 12);                           // 00611A0A..00611A25
        // 00611A29..00611A3C: Yaw(desired) -> fstp dword [esp+40h] ; Yaw(&current) ; fsubr dword [esp+44h].
        const long double yaw_desired = static_cast<float>(yaw_004aa0f0(desired));
        long double d = yaw_desired - static_cast<long double>(yaw_004aa0f0(&current));
        // [esp+3Ch] `slot` is the stored float itself (reloaded by fld at 00611A9A/00611AC0, pushed by mov at 00611ACF).
        float slot = static_cast<float>(d);                                                  // 00611A49 fst dword [esp+3Ch]
        if (d > 180.0f) { d = d - 360.0f; slot = static_cast<float>(d); }                    // 00611A43..00611A5A (test 41h ; jne)
        if (d < -180.0f) { d = d + 360.0f; slot = static_cast<float>(d); }                   // 00611A5E..00611A71 (test 5 ; jp)
        // 00611A7B..00611A92 / 00611AAD..00611ABA: fild [esi+44h] (+2^32 if negative: the unsigned value) ; fmul [007A25F4].
        const long double turn = static_cast<long double>(at<U32>(c, 0x44)) * kotor_cell_turn_rate_007a25f4;
        // Both clamps reach the one store 00611AA7 fstp dword [esp+3Ch] (the -turn path falls into it, the +turn path
        // jumps to it from 00611ACB), so the clamp value is selected first and converted once.
        long double clamp = turn;
        bool clamped;
        if (d < 0.0f) {                                                                      // 00611A75..00611A83 (fcomp 0 ; test 5 ; jp)
            clamp = -turn;                                                                   // 00611A98 fchs
            clamped = slot < clamp;                                                          // 00611A9A..00611AA5 (test 5 ; jp)
        } else {
            clamped = slot > clamp;                                                          // 00611AC0..00611ACB (test 41h ; je)
        }
        if (clamped) slot = static_cast<float>(clamp);                                       // 00611AA7 fstp dword [esp+3Ch]
        W4Quaternion q;                                                                      // [esp+28h]
        yaw_pitch_roll_004acac0(&q, slot, 0.0f, 0.0f);                                       // 00611ACF..00611AE2 (push the stored word)
        W4Vector rotated;                                                                    // [esp+10h]
        quaternion_rotate_004a9bb0(&rotated, &q, &current);                                  // 00611AEA..00611AF0
        w1x_object_set_orientation_004cd170(sc, nullptr, &rotated);                          // 00611AF8..00611AFF CSWSObject::SetOrientation
        vcall<void, W4Vector*>(c, 0x88, &rotated);                                           // 00611B04..00611B0D
        return;
    }
    // 00611B1B..00611EB7: turn toward `desired` in the horizontal plane.
    W4Vector* orientation = V(c, 0x30);                                                      // edi
    W4Vector flat;                                                                           // 00611B22..00611B40 (dword copies)
    std::memcpy(&flat, desired, 8);
    flat.z = 0.0f;
    W4Vector n_tmp;
    const W4Vector* nn = vector_normalized_004ab180(&n_tmp, &flat);                          // 00611B48
    W4Vector n;
    std::memcpy(&n, nn, 12);                                                                 // 00611B4D..00611B6B (stored over `flat`)
    W4Vector cross_out;
    const W4Vector* cr = vector_cross_004ab210(&cross_out, orientation, &n);                 // 00611B87
    long double sign = 1.0f;                                                                 // 00611B98 mov dword [esp+0Ch], 3F800000
    if (!(cr->z >= 0.0f)) sign = -1.0f;                                                      // 00611B8C..00611BA7 (test 1 ; je)
    // 00611BAF..00611BC8: (n.y*o.y + n.z*o.z) + n.x*o.x -> fstp dword [esp+44h].
    const long double dotf = static_cast<float>((static_cast<long double>(n.y) * orientation->y + static_cast<long double>(n.z) * orientation->z) +
                                                static_cast<long double>(n.x) * orientation->x);
    const long double yaw_o = static_cast<float>(yaw_004aa0f0(orientation));                // 00611BCC..00611BD5 fstp dword [esp+14h]
    const long double yaw_diff = fabs(yaw_o - static_cast<long double>(yaw_004aa0f0(&n)));   // 00611BDA..00611BE6 fsubr ; fabs
    if (dotf < 1.0f && yaw_diff > 15.0f) {                                                   // 00611BE8..00611C08 (test 5 ; jp / test 41h ; jne)
        const std::uint16_t anim = lh(c, 0x48);                                              // 00611C0E
        const bool turnable = anim == 0x2710 || anim == 0x2711 || anim == 0x2746 || anim == 0x2745 ||
                              (anim >= 0x2736 && anim <= 0x273A) || anim == 0x27AA || anim == 0x27AB || anim == 0x272E;
        if (turnable) {
            if (anim == 0x2746 || anim == 0x2745)                                            // 00611C5E..00611C68
                kotor_m15a_object_abort_fire_forget_animation_0063cad0(c, nullptr, 0);       // 00611C6A..00611C6E
            const std::uint16_t now = lh(c, 0x48);                                           // 00611C82
            const U32 turn_anim = !(sign < 0.0f) ? 0x16Fu : 0x170u;                          // 00611C73..00611C86 (test 5 ; jp)
            const bool standing = now == 0x2710 || (now >= 0x2736 && now <= 0x273A) || now == 0x27AA ||
                                  now == 0x27AB || now == 0x272E;
            if (standing) {                                                                  // 00611CE2 / 00611D82
                if (gob_get_property_0x68_0063c070(c, nullptr) != nullptr &&
                    lb(gob_get_property_0x68_0063c070(c, nullptr), 0xC6) == 1 &&
                    lh(c, 0x48) != 0x16F && lh(c, 0x48) != 0x170)
                    add_fire_forget(c, turn_anim, 0);                                        // 00611D1D..00611D28 / 00611DAD..00611DB8
            } else if (now == 0x2711) {
                add_fire_forget(c, turn_anim, 0);                                            // 00611CCC..00611CD7 / 00611D6F..00611D7A
            }
        }
    }
    // 00611DC2..00611E04: fld 1.0 ; fsub [esp+40h] (dotf) ; fmul [00740C84] 3 ; doubled for anim 2714h ; clamp.
    long double rate = (1.0f - dotf) * 3.0f;
    if (lh(c, 0x48) == 0x2714) rate = rate + rate;                                           // 00611DD8..00611DDC fld st(0) ; faddp
    if (rate <= 0.55f) rate = 0.55f;                                                         // 00611DDE..00611DED (fcom ; test 41h ; jp)
    else if (rate >= 1.0f) rate = 1.0f;                                                      // 00611DF5..00611E04 (fcom ; test 1 ; jne)
    // 00611E0A..00611E1C: fild [esi+44h] (+2^32 if negative) ; fmulp ; fmul [0074F4EC] 0.004f.
    const long double angle = rate * static_cast<long double>(at<U32>(c, 0x44)) * 0.004f;
    W4Vector out;                                                                            // [esp+14h]
    // 00611E22..00611E31: fld st(0) ; fcos ; fld dotf ; fcompp ; test 1 ; jne (rotate unless dotf >= cos(angle)).
    if (dotf >= cos(angle)) {
        std::memcpy(&out, desired, 12);                                                      // 00611E33..00611E46
    } else {
        // 00611E4C..00611E70: a = angle * sign ; cos(a) and sin(a) both live (two-live site) ;
        // x = cos*o.x - sin*o.y -> fstp dword [esp+14h] ; y = sin*o.x + cos*o.y -> fstp dword [esp+18h].
        const long double a = angle * sign;
        const long double ca = cos(a);
        const long double sa = sin(a);
        out.x = static_cast<float>(ca * orientation->x - sa * at<float>(c, 0x34));
        out.y = static_cast<float>(sa * orientation->x + ca * at<float>(c, 0x34));
    }
    out.z = 0.0f;                                                                            // 00611E7E
    W4Vector norm;
    const W4Vector* r = vector_normalized_004ab180(&norm, &out);                             // 00611E74..00611E86
    std::memcpy(&out, r, 12);                                                                // 00611E8B..00611E9D
    vcall<void, W4Vector*>(c, 0x88, &out);                                                   // 00611EA4..00611EAB SetOrientation
}

// 0061A980 CSWCCreature::AnimationUpdate() (RET 0).
extern "C" void __fastcall kotor_fastcall_0061a980(void* c, void*) {
    if (kotor_m15a_object_animation_stationary_0063c170(c, nullptr, lh(c, 0x48)) == 0 &&
        gso(c) != nullptr && vcall<void*>(gso(c), 0x30) != nullptr) {                        // 0061A983..0061A9AE
        if (lw(screature(c), 0xA8C) == 0) return;                                            // 0061A9B0..0061A9D5
    }
    if (lw(c, 0x2F4) != 0x7F000000u /* [00746644] */) kotor_fastcall_0060ba10(c, nullptr);  // 0061A9DB..0061A9EB
    if (kotor_m15a_object_animation_stationary_0063c170(c, nullptr, lh(c, 0x48)) != 0)       // 0061A9F0..0061AA02
        kotor_fastcall_00611850(c, nullptr, V(c, 0x1D0));                                    // 0061AA04..0061AA14
    else
        kotor_fastcall_00611850(c, nullptr, V(c, 0x1C4));                                    // 0061AA0D..0061AA14
    kotor_fastcall_0060bdf0(c, nullptr, V(c, 0x1DC));                                        // 0061AA19..0061AA22
    if (kotor_m15a_object_animation_stationary_0063c170(c, nullptr, lh(c, 0x48)) == 0)       // 0061AA27..0061AA37
        set_animation(c, kotor_m15a_object_get_looping_animation_0063c100(c, nullptr) & 0xFFFFu, 0, 0); // 0061AA39..0061AA4C
    kotor_fastcall_0061a670(c, nullptr);                                                     // 0061AA52..0061AA54
    const U8 mode = lb(c, 0x2D2);
    if (lh(c, 0x48) == 0x2711 && mode != 2 && mode != 3 && mode != 0xE && lw(c, 0x138) != 1) // 0061AA59..0061AA7A
        set_animation(c, 0x2710, 0, 0);                                                      // 0061AA7C..0061AA89
    if (kotor_fastcall_0063c1e0(c, nullptr, lh(c, 0x48)) != 0) {                             // 0061AA8F..0061AA9F
        const U8 m = lb(c, 0x2D2);
        if (m == 2 || m == 3 || m == 0xE) set_animation(c, 0x2711, 0, 0);                    // 0061AAA1..0061AAC0
    }
    if (kotor_fastcall_0063c1e0(c, nullptr, lh(c, 0x48)) == 0) return;                       // 0061AAC6..0061AAD6
    if (lh(c, 0x48) == 0x27A6) return;                                                       // 0061AAD8..0061AADE
    if (recovered_server_is_injured_004aee40(server_app(), nullptr, lw(c, 4)) == 0) return;               // 0061AAE0..0061AAF4
    if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 5 &&
        c == frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0)) return; // 0061AAF6..0061AB22
    set_animation(c, 0x276C, 0, 0);                                                          // 0061AB24..0061AB31
}

// 0061A670 CSWCCreature::AnimateFireAndForget() (RET 0).
extern "C" void __fastcall kotor_fastcall_0061a670(void* c, void*) {
    if (kotor_fastcall_0063c380(c, nullptr, lh(c, 0x48)) == 1) {                             // 0061A676..0061A686
        const I32 left = static_cast<I32>(lw(c, 0x64) - lw(c, 0x44));                        // 0061A688..0061A68E
        sw(c, 0x64, static_cast<U32>(left));                                                 // 0061A690
        if (left < 0) {                                                                      // 0061A693
            if (kotor_fastcall_0063c410(c, nullptr, lh(c, 0x48)) != 0)                       // 0061A695..0061A6A5
                vcall<void, U32, U32>(lp(c, 0x68), 0x44, lh(c, 0x48), 0);                    // 0061A6A7..0061A6B5
            sw(c, 0x64, 0);                                                                  // 0061A6C0
            set_animation(c, kotor_m15a_object_get_looping_animation_0063c100(c, nullptr), 0, 0); // 0061A6B8..0061A6CF
        }
    }
    if (kotor_m15a_object_animation_looping_0063c300(c, nullptr, lh(c, 0x48)) != 1) return;  // 0061A6D5..0061A6E6
    U32 anim_slot = 0;          // [esp+8]: only the low word is written by the callee (high word: see receipt)
    U32 data = 0, duration = 0; // [esp+0Ch], [esp+18h]
    // [esp+10h]: the callee's float; the original passes its bits on with `mov eax, [esp+14h] ; push eax` (0061A909),
    // no x87 (L3 t1 a2: was a float local copied through fld/fstp by the f_bits call, which quiets a signalling NaN).
    U32 speed = 0;
    if (kotor_fastcall_0063c610(c, nullptr, reinterpret_cast<std::uint16_t*>(&anim_slot), reinterpret_cast<float*>(&speed), &duration, &data) != 1) return; // 0061A6EC..0061A70A
    const std::uint16_t anim = static_cast<std::uint16_t>(anim_slot);
    void* free_a = nullptr;     // ebp
    void* free_b = nullptr;     // [esp+14h]
    const U32 attack = kotor_fastcall_0063c550(c, nullptr, anim_slot);                       // 0061A710..0061A71E
    if (attack != 0 || anim == 0x2711) {                                                     // 0061A723..0061A730
        free_a = pt(data);                                                                   // 0061A732..0061A738
        if (data != 0) kotor_fastcall_0060b6b0(lp(c, 0x2B8), nullptr, pt(data));             // 0061A73A..0061A743
    }
    if (anim == 0x2728 || anim == 0x272A || anim == 0x272B || anim == 0x2729)                // 0061A750..0061A76A
        kotor_fastcall_0061a2d0(c, nullptr);                                                 // 0061A76C..0061A76E
    if (anim == 0x2721 || anim == 0x2722 || anim == 0x2723 || anim == 0x2724 || anim == 0x274D || anim == 0x28A1) { // 0061A77B..0061A7A3
        free_b = pt(data);                                                                   // 0061A7A7
        if (data != 0) kotor_fastcall_0060b6b0(lp(c, 0x2BC), nullptr, pt(data));             // 0061A7AB..0061A7B4
    }
    kotor_m15e_object_test_and_reequip_weapons_0063d850(c, nullptr, lh(c, 0x48), anim_slot); // 0061A7BD..0061A7C5
    std::memcpy(B(c) + 0x48, &anim_slot, 2);                                                 // 0061A7D1
    if (kotor_m15e_object_animation_hide_equipped_items_0063c5b0(c, nullptr, anim_slot) != 0) { // 0061A7CE..0061A7DC
        U8 ref_a[16], ref_b[16];                                                             // [esp+30h], [esp+20h] (CResRef)
        void* pa = kotor_fastcall_00405ed0(ref_a, nullptr);                                  // 0061A7DE..0061A7E2
        void* pb = kotor_fastcall_00405ed0(ref_b, nullptr);                                  // 0061A7E8..0061A7EC
        kotor_fastcall_00614630(c, nullptr, pb, pa);                                         // 0061A7E7..0061A7F4
    }
    U32 out_of_place = 0;                                                                    // edi
    if (lw(c, 0x50) != 0) {                                                                  // 0061A7F9..0061A800
        W4Vector position; std::memcpy(&position, B(c) + 0x24, 12);                          // 0061A802..0061A828
        W4Vector orient; std::memcpy(&orient, B(c) + 0x30, 12);
        sw(c, 0x50, 0);                                                                      // 0061A833
        vcall<void, W4Vector*>(lp(c, 0x68), 0x68, &position);                                // 0061A83A..0061A83D
        vcall<void, W4Vector*>(lp(c, 0x68), 0x64, &orient);                                  // 0061A840..0061A84A
        vcall<void, U32>(lp(c, 0x68), 0x6C, 0);                                              // 0061A84D..0061A853
        vcall<void, U32>(lp(c, 0x68), 0x70, 0);                                              // 0061A856..0061A85C
        kotor_m15a_object_abort_fire_forget_animation_0063cad0(c, nullptr, 0);               // 0061A85F..0061A862
    }
    if (kotor_fastcall_0063c470(c, nullptr, anim_slot) != 0) {                               // 0061A867..0061A875
        out_of_place = 1;                                                                    // 0061A87A
        sw(c, 0x50, 1);                                                                      // 0061A883
        W4Vector zero = {0.0f, 0.0f, 0.0f};                                                  // [esp+30h]
        W4Quaternion identity = {1.0f, 0.0f, 0.0f, 0.0f};                                    // [esp+20h] (memory order as written)
        vcall<void, W4Vector*>(lp(c, 0x68), 0x68, &zero);                                    // 0061A8BE..0061A8C1
        vcall<void, W4Quaternion*>(lp(c, 0x68), 0x60, &identity);                            // 0061A8C4..0061A8CE
        vcall<void, U32>(lp(c, 0x68), 0x6C, 1);                                              // 0061A8D1..0061A8D7
        vcall<void, U32>(lp(c, 0x68), 0x70, 1);                                              // 0061A8DA..0061A8E0
        sw(c, 0x54, 1);                                                                      // 0061A8E3
    }
    if (kotor_fastcall_0063c410(c, nullptr, anim_slot) != 0)                                 // 0061A8E6..0061A8F4
        vcall<void, U32, U32>(lp(c, 0x68), 0x44, anim_slot, 1);                              // 0061A8FB..0061A902
    else
        vcall<void, U32, U32, U32, U32>(lp(c, 0x68), 0x54, anim_slot, speed, out_of_place ? 3u : 1u, 0); // 0061A907..0061A91D
    sw(c, 0x64, duration);                                                                   // 0061A920..0061A92A
    if (data != 0) kotor_fastcall_00617470(c, nullptr, pt(data), 1);                         // 0061A92D..0061A934
    kotor_fastcall_0063c9c0(c, nullptr);                                                     // 0061A939..0061A93B
    if (free_a != nullptr) live_free(free_a);                                                // 0061A940..0061A945
    if (free_b != nullptr) live_free(free_b);                                                // 0061A94D..0061A957
}

// 00615960 CSWCCreature::DriveAnimationUpdate() (RET 0).
extern "C" void __fastcall kotor_fastcall_00615960(void* c, void*) {
    if (kotor_fastcall_0063c4a0(c, nullptr, lh(c, 0x48)) != 0) return;                      // 00615962..00615972
    if (server_app() != nullptr && gso(c) != nullptr) {                                      // 00615978..0061598F
        void* s = vcall<void*>(gso(c), 0x30);                                                // 00615991..0061599F
        if (s != nullptr) {
            if (vcall<U32>(s, 0x94) != 0) return;                                            // 006159A5..006159B1
            // 006159B7..006159C0: call 0063E7F0 (return_zero) with ECX = s: never returns.
            if (recovered_creature_incapacitated_004ef890(s, nullptr) != 0) return;          // 006159C6..006159CF
            if (kotor_fastcall_005b4880(s, nullptr) != 0) return;                            // 006159D5..006159DE
            if ((lb(s, 0x9F0) & 2) == 0) return;                                             // 006159E4..006159EB
        }
    }
    if (kotor_m15a_object_animation_stationary_0063c170(c, nullptr, lh(c, 0x48)) == 0 &&
        kotor_fastcall_0060f0f0(c, nullptr) != 0) {                                          // 006159F1..00615A0C
        vcall<void, U32>(lp(c, 0x68), 0xD0, 0);                                              // 00615A0E..00615A15
        return;
    }
    // L3 t1 a2 (writing rule). [esp+8] `scale` is copied with integer moves (00615A25..00615A2E, 00615BBE/00615C39
    // for the walk/run speed) and read as an fdiv/fdivr memory operand: kept as its bits. Each test is
    // fld x ; fcomp 0 ; test ah,M ; jcc: test 41h;jne falls through for x > 0, test 5;jp for x < 0, test 44h;jp for
    // x == 0 (NaN takes the jump in all three).
    U32 scale = lw(lp(c, 0x21C), 0x5C);                                                      // 00615A1F..00615A2E [esp+8]
    (void)kotor_fastcall_005ed8b0(client_app(), nullptr);                                    // 00615A28..00615A35 (unused)
    if (lb(c, 0x2D4) & 1) {                                                                  // 00615A3A..00615A41
        void* stats = lp(c, 0x21C);
        if (at<float>(c, 0x3B0) > at<float>(stats, 0x60)) sw(c, 0x3B0, lw(stats, 0x60));     // 00615A43..00615A5C (test 41h ; jne)
    }
    // The eight tests select the animation (each test re-reads +3B0h / +3B4h); 2776h and 2777h are reached from two
    // tests each and share one store (00615D8B / 00615AB2), as in the original.
    U32 play = 0;                                                                            // 0: stand (00615E0D)
    if (at<float>(c, 0x3B0) > 0.0f && at<float>(c, 0x3B4) > 0.0f) play = 0x2776;            // 00615A62..00615A86 -> 00615D8B
    else if (at<float>(c, 0x3B0) > 0.0f && at<float>(c, 0x3B4) < 0.0f) play = 0x2777;       // 00615A8C..00615AB0 -> 00615AB2
    else if (at<float>(c, 0x3B0) > 0.0f && at<float>(c, 0x3B4) == 0.0f) play = 1;           // 00615AE6..00615B0E -> 00615B14
    else if (at<float>(c, 0x3B0) == 0.0f && at<float>(c, 0x3B4) < 0.0f) play = 0x2779;      // 00615C8D..00615CB1 -> 00615CB3
    else if (at<float>(c, 0x3B0) == 0.0f && at<float>(c, 0x3B4) > 0.0f) play = 0x2778;      // 00615CE7..00615D0B -> 00615D0D
    else if (at<float>(c, 0x3B0) < 0.0f && at<float>(c, 0x3B4) > 0.0f) play = 0x2777;       // 00615D3B..00615D5F -> 00615AB2
    else if (at<float>(c, 0x3B0) < 0.0f && at<float>(c, 0x3B4) < 0.0f) play = 0x2776;       // 00615D65..00615D89 -> 00615D8B
    else if (at<float>(c, 0x3B0) < 0.0f && at<float>(c, 0x3B4) == 0.0f) play = 0x2714;      // 00615DB9..00615DDD -> 00615DDF
    if (play == 1) {
        // 00615B14..00615C8C: walk or run forward.
        (void)kotor_m15e_creature_get_pause_ready_animation_0060f020(c, nullptr);            // 00615B19 (overwritten)
        // 00615B1E..00615B39: fld [007A22F4] ; fmul [scale] ; fcomp [esi+3B0h] ; test 5 ; jp (run when ratio*scale < fwd).
        const bool run = static_cast<long double>(kotor_cell_drive_run_ratio_007a22f4) * fbits(scale) < at<float>(c, 0x3B0);
        auto injured_anim = [&]() {
            return lw(c, 0x3A4) != 0 && kotor_fastcall_00614b60(c, nullptr) != 0 &&
                   !(lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 5 &&
                     c == frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0));
        };
        U32 anim;
        if (run) {
            anim = injured_anim() ? 0x276Eu : ((lb(c, 0x2D4) & 1) ? 0x2795u : 0x2714u);      // 00615B4E..00615BAF
            sb(c, 0x3E0, 1);                                                                 // 00615BB7
            scale = lw(lp(c, 0x21C), 0x64);                                                  // 00615BB1..00615BC1
        } else {
            anim = injured_anim() ? 0x276Du : ((lb(c, 0x2D4) & 1) ? 0x2795u : 0x2712u);      // 00615BC7..00615C2A
            sb(c, 0x3E0, 0);                                                                 // 00615C32
            scale = lw(lp(c, 0x21C), 0x60);                                                  // 00615C2C..00615C3C
        }
        // 00615C40..00615C57: GetAnimationLength (ST0) ; fmul [0073D708] 0.001 ; fdivr [scale] ; fdivr [esi+3B0h].
        const long double len = static_cast<long double>(anim_length_0063c0e0(c, anim)) * 0.001f;
        const long double value = at<float>(c, 0x3B0) / (fbits(scale) / len);
        const U32 blend = lw(c, 0x3BC) == 0x276F ? 0u : 1u;                                  // 00615C4E/00615C63..00615C70
        at<float>(c, 0x4C) = static_cast<float>(value);                                      // 00615C74
        sw(c, 0x3BC, anim);                                                                  // 00615C7D
        set_animation(c, anim, blend, 0);                                                    // 00615C72..00615C83
        return;
    }
    // play: +3BCh = anim ; fstp dword [esi+4Ch] ; SetAnimation(anim, 0, 0) (vtable +9Ch).
    switch (play) {
    case 0x2777:   // 00615AB2..00615AD9: fld -1.0 ; fdiv [scale] ; fmul [esi+3B4h] ; fstp [esi+4Ch]
        sw(c, 0x3BC, 0x2777); at<float>(c, 0x4C) = static_cast<float>(-1.0f / fbits(scale) * at<float>(c, 0x3B4)); set_animation(c, 0x2777, 0, 0); return;
    case 0x2779:   // 00615CB3..00615CDA: the same form
        sw(c, 0x3BC, 0x2779); at<float>(c, 0x4C) = static_cast<float>(-1.0f / fbits(scale) * at<float>(c, 0x3B4)); set_animation(c, 0x2779, 0, 0); return;
    case 0x2778:   // 00615D0D..00615D2E: fld [esi+3B4h] ; fdiv [scale] ; fstp [esi+4Ch]
        sw(c, 0x3BC, 0x2778); at<float>(c, 0x4C) = static_cast<float>(at<float>(c, 0x3B4) / fbits(scale)); set_animation(c, 0x2778, 0, 0); return;
    case 0x2776:   // 00615D8B..00615DAC: the same form
        sw(c, 0x3BC, 0x2776); at<float>(c, 0x4C) = static_cast<float>(at<float>(c, 0x3B4) / fbits(scale)); set_animation(c, 0x2776, 0, 0); return;
    case 0x2714:   // 00615DDF..00615E00: fld [esi+3B0h] ; fdiv [scale] ; fstp [esi+4Ch]
        sw(c, 0x3BC, 0x2714); at<float>(c, 0x4C) = static_cast<float>(at<float>(c, 0x3B0) / fbits(scale)); set_animation(c, 0x2714, 0, 0); return;
    default: break;
    }
    // 00615E0D..00615E79: stand.
    sw(c, 0x4C, 0x3F800000u);                                                                // 00615E0F mov dword [esi+4Ch], 3F800000h (1.0f)
    const U32 ready = kotor_m15e_creature_get_pause_ready_animation_0060f020(c, nullptr);    // 00615E16
    const std::uint16_t di = static_cast<std::uint16_t>(ready);
    if (di != 0x2711 && di != 0x276C && lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) != 5) { // 00615E1D..00615E3D
        if (di != 0x2710) return;                                                            // 00615E3F..00615E44
        if (kotor_fastcall_0063c1e0(c, nullptr, lh(c, 0x48)) != 0) return;                   // 00615E46..00615E56
    }
    sw(c, 0x3BC, di);                                                                        // 00615E58..00615E5D
    set_animation(c, lh(c, 0x3BC), 0, 0);                                                    // 00615E63..00615E73
}

// 00615E80 CSWCCreature::FollowAnimationUpdate() (RET 0).
extern "C" void __fastcall kotor_fastcall_00615e80(void* c, void*) {
    if (gso(c) != nullptr && vcall<void*>(gso(c), 0x30) != nullptr) {                        // 00615E86..00615EA3
        if (recovered_creature_incapacitated_004ef890(screature(c), nullptr) != 0) return;   // 00615EA9..00615ECF
        if (vcall<U32>(screature(c), 0x94) != 0) return;                                     // 00615ED5..00615EFE
        if (kotor_fastcall_005b4880(screature(c), nullptr) != 0) return;                     // 00615F04..00615F2A
    }
    const I32 index = recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, lw(c, 4)); // 00615F30..00615F4B
    if (index != 1 && index != 2) return;                                                    // 00615F50..00615F59
    U8* member = B(kotor_fastcall_005ed8b0(client_app(), nullptr)) + static_cast<U32>(index) * 0x88 + 0x24; // 00615F5F..00615F7A (ebp)
    // L3 t1 a2 (writing rule). [esp+10h] follow speed: copied with `mov eax, [ebp+48h] ; mov [esp+10h], eax`
    // (00615F7E..00615F81) and read as an fld / fdivr memory operand: kept as its bits.
    const U32 follow_speed = lw(member, 0x48);                                               // 00615F7E..00615F81 [esp+10h]
    // 00615F6E/00615F93: fld [esi+210h] ; fmul [007A22F4] (stays in ST until the compares).
    const long double threshold = static_cast<long double>(at<float>(c, 0x210)) * kotor_cell_drive_run_ratio_007a22f4;
    void* stats = lp(c, 0x21C);
    const float walk_f = static_cast<float>(static_cast<long double>(at<float>(stats, 0x60)) * 1000.0f); // 00615F99..00615FA2 fstp [esp+1Ch]
    const long double walk_ms = walk_f;
    const long double run_ms = static_cast<float>(static_cast<long double>(at<float>(stats, 0x64)) * 1000.0f);  // 00615FA6..00615FAF fstp [esp+18h]
    U32 anim;
    long double value;
    bool apply = true;                                                                       // ebx
    if (fbits(follow_speed) <= 0.0001f) {                                                    // 00615FB3..00615FC2 (fcomp ; test 41h ; jp)
        if (lw(pt(frame_R4_005ed690(client_app(), nullptr)), 0xB4) != 0 &&
            kotor_fastcall_0063c4a0(c, nullptr, lh(c, 0x48)) != 0) {                         // 00615FC4..00615FEE
            value = 0.0f;                                                                    // 00615FF0 fld [esp+14h] (= 0)
            std::memcpy(&anim, &walk_f, 4);                                                  // 00615FF4 mov edi, [esp+1Ch] (sic: the float bits used as the id)
            apply = false;                                                                   // 00615FF8
        } else {
            anim = kotor_m15e_creature_get_pause_ready_animation_0060f020(c, nullptr);       // 00615FFF..00616001
            value = 1.0f;                                                                    // 00616006 fld [0073D764]
        }
    } else {
        auto injured = [&]() {
            return lw(c, 0x3A4) != 0 && kotor_fastcall_00614b60(c, nullptr) != 0 &&
                   !(lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 5 &&
                     c == frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0));
        };
        // 00616013..00616026: fld [esp+10h] ; fcomp st(1) ; test 1 ; jne (walk when below the threshold, or NaN).
        if (!(fbits(follow_speed) >= threshold)) {
            anim = injured() ? 0x276Du : ((lb(c, 0x2D4) & 1) ? 0x2795u : 0x2712u);           // 0061609B..006160F8
            // 006160FA..0061610C: GetAnimationLength ; fmul 0.001 ; fdivr [esp+1Ch] ; fdivr [esp+10h].
            value = fbits(follow_speed) / (walk_ms / (static_cast<long double>(anim_length_0063c0e0(c, anim)) * 0.001f));
        } else {
            anim = injured() ? 0x276Eu : ((lb(c, 0x2D4) & 1) ? 0x2795u : 0x2714u);           // 00616028..00616085
            // 00616087..0061610C: GetAnimationLength ; fmul 0.001 ; fdivr [esp+18h] ; fdivr [esp+10h].
            value = fbits(follow_speed) / (run_ms / (static_cast<long double>(anim_length_0063c0e0(c, anim)) * 0.001f));
        }
    }
    at<float>(c, 0x4C) = static_cast<float>(value);                                          // 00616112
    sw(c, 0x3BC, anim);                                                                      // 00616115
    if (apply) set_animation(c, anim, 0, 0);                                                 // 00616110..00616126
    kotor_fastcall_00611850(c, nullptr, reinterpret_cast<W4Vector*>(member + 0x4C));         // 0061612C..00616132
}

// 0063BFD0 fire-and-forget queue entry constructor (unlabelled; thiscall, RET 0; returns this).
extern "C" void* __fastcall kotor_fastcall_0063bfd0(void* e, void*) {
    sw(e, 0x00, 0);                                                                          // 0063BFD4
    const std::uint16_t idle = 0x2710; std::memcpy(B(e) + 4, &idle, 2);                      // 0063BFD6
    sw(e, 0x08, 0); sw(e, 0x0C, 0);                                                          // 0063BFDC..0063BFDF
    sw(e, 0x10, 0x1F4);                                                                      // 0063BFE2
    sw(e, 0x14, 0x3F800000u);                                                                // 0063BFE9
    sw(e, 0x18, 0);                                                                          // 0063BFF0
    return e;                                                                                // eax = this
}

// 004ADF50 CWorldTimer::AddWorldTimes(d1, t1, d2, t2, out_day*, out_tick*) (thiscall, RET 18h).
extern "C" U32 __fastcall kotor_fastcall_004adf50(void* timer, void*, U32 d1, U32 t1, U32 d2, U32 t2, U32* out_day, U32* out_tick) {
    const U32 per_day = lw(timer, 0x3C);                                                     // 004ADF50
    if (t1 >= per_day || t2 >= per_day) return 0xFFFFFFFEu;                                  // 004ADF54..004ADF62, 004ADF90
    *out_day = d1 + d2;                                                                      // 004ADF69..004ADF79
    *out_tick = t2 + t1;                                                                     // 004ADF73..004ADF7B
    if (*out_tick >= lw(timer, 0x3C)) { ++*out_day; *out_tick -= lw(timer, 0x3C); }          // 004ADF7D..004ADF87
    return 0;                                                                                // 004ADF8A
}

// 0063C760 CSWCObject::InsertFireForgetAnimation(anim, duration, speed, data, delay) (thiscall, RET 14h).
// Queue entries are 1Ch bytes at [+5Ch], count byte [+60h], capacity word [+5Ah] (grows by 8).
extern "C" void __fastcall kotor_fastcall_0063c760(void* o, void*, U32 anim, U32 duration, float speed, U32 data, U32 delay) {
    (void)kotor_fastcall_0063c380(o, nullptr, anim);                                         // 0063C780..0063C783 (result unused)
    U32 day = 0, tick = 0;                                                                   // [esp+18h], [esp+14h]
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &day, &tick); // 0063C788..0063C7A1
    if (static_cast<std::uint16_t>(lb(o, 0x60)) == lh(o, 0x5A)) {                           // 0063C7A6..0063C7B2 (full)
        const U32 capacity = static_cast<U32>(lh(o, 0x5A)) + 8;                              // 0063C7B8..0063C7BB
        U8* grown = static_cast<U8*>(live_operator_new(capacity * 0x1C));                     // 0063C7BE..0063C7C9
        if (grown != nullptr)
            vector_constructor_iterator_00401130(grown, 0x1C, capacity, reinterpret_cast<void*>(&kotor_fastcall_0063bfd0)); // 0063C7DE..0063C7E7
        for (U32 i = 0; i < lh(o, 0x5A); ++i) {                                              // 0063C7F0..0063C858
            U8* dst = grown + i * 0x1C; const U8* src = static_cast<const U8*>(lp(o, 0x5C)) + i * 0x1C;
            std::memcpy(dst + 0x00, src + 0x00, 4);
            std::memcpy(dst + 0x04, src + 0x04, 2);                                          // word copy only
            for (U32 f = 0x08; f <= 0x18; f += 4) std::memcpy(dst + f, src + f, 4);
        }
        live_free(lp(o, 0x5C));                                                              // 0063C85A..0063C85E
        const std::uint16_t cap16 = static_cast<std::uint16_t>(lh(o, 0x5A) + 8);
        std::memcpy(B(o) + 0x5A, &cap16, 2);                                                 // 0063C866
        sw(o, 0x5C, ad(grown));                                                              // 0063C86B
    }
    // AddWorldTimes' outputs are the `anim` argument slot [esp+30h] (reused; anim is already in ebp) and the local
    // [esp+10h], which nothing writes before. When 004ADF50 fails (tick or delay >= ticks per day, 004ADF90) it writes
    // neither: the original then stores the anim argument as the start day (reproduced) and stack garbage as the
    // start tick (indeterminate; 0 here). L3 t1 a2: was start_day = 0.
    U32 start_day = anim, start_tick = 0;                                                    // [esp+30h], [esp+10h]
    (void)kotor_fastcall_004adf50(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, day, tick, 0, delay,
                                  &start_day, &start_tick);                                  // 0063C86E..0063C899
    I32 i = static_cast<I32>(lb(o, 0x60)) - 1;                                               // 0063C89E..0063C8A2
    for (; i >= 0; --i) {                                                                    // 0063C8A3..0063C946
        U8* q = static_cast<U8*>(lp(o, 0x5C)) + static_cast<U32>(i) * 0x1C;
        if (static_cast<I32>(recovered_event_time_compare_004adfa0(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr,
                start_day, start_tick, lw(q, 8), lw(q, 0xC))) != -1) break;                 // 0063C8B0..0063C8DF
        std::memcpy(q + 0x1C, q + 0x00, 4);                                                  // 0063C8E1..0063C943 shift up by one
        std::memcpy(q + 0x20, q + 0x04, 2);
        for (U32 f = 0x08; f <= 0x18; f += 4) std::memcpy(q + 0x1C + f, q + f, 4);
    }
    U8* e = static_cast<U8*>(lp(o, 0x5C)) + static_cast<U32>(i + 1) * 0x1C;                  // 0063C94C..0063C954
    sw(e, 0x00, 1);                                                                          // 0063C957
    const std::uint16_t a16 = static_cast<std::uint16_t>(anim); std::memcpy(e + 4, &a16, 2); // 0063C961
    sw(e, 0x08, start_day);                                                                  // 0063C96D
    sw(e, 0x0C, start_tick);                                                                 // 0063C978
    sw(e, 0x10, duration);                                                                   // 0063C983
    // 0063C94F fld dword [esp+38h] (the speed argument) ... 0063C98F fstp dword [ebx+ecx+14h]: an x87 copy (it quiets a
    // signalling NaN), written as the widening and the one float store (L3 t1 a2: was sf(), a float held across calls).
    at<float>(e, 0x14) = static_cast<float>(static_cast<long double>(speed));
    sw(e, 0x18, data);                                                                       // 0063C99A
    sb(o, 0x60, static_cast<U8>(lb(o, 0x60) + 1));                                           // 0063C99E
}
